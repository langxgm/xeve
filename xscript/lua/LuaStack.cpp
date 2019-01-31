
#include "LuaStack.h"

#include <xbase/IOUtil.h>

#include <lua.hpp>
#include <glog/logging.h>

int get_string_for_print(lua_State* L, std::string* out)
{
	int n = lua_gettop(L);  /* number of arguments */
	int i;

	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		size_t sz;
		s = lua_tolstring(L, -1, &sz);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1) out->append("\t");
		out->append(s, sz);
		lua_pop(L, 1);  /* pop result */
	}
	return 0;
}

int lua_print(lua_State* L)
{
#ifdef _DEBUG
	std::string t;
	get_string_for_print(L, &t);
	LOG(INFO) << "[LUA-print] %s" << t;
#endif
	return 0;
}

int lua_release_print(lua_State* L)
{
	std::string t;
	get_string_for_print(L, &t);
	LOG(INFO) << "[LUA-print] %s" << t;
	return 0;
}

LuaStack::LuaStack()
{

}

LuaStack::~LuaStack()
{
	if (m_pLuaState)
	{
		lua_close(m_pLuaState);
	}
}

bool LuaStack::Init()
{
	m_pLuaState = luaL_newstate();
	if (m_pLuaState == nullptr)
	{
		return false;
	}

	luaL_openlibs(m_pLuaState);

	// Register our version of the global "print" function
	const luaL_Reg global_functions[] = {
		{ "print", lua_print },
		{ "release_print", lua_release_print },
		{ nullptr, nullptr }
	};
	lua_newtable(m_pLuaState);
	luaL_setfuncs(m_pLuaState, global_functions, 0);

	return true;
}

bool LuaStack::InitWithLuaState(lua_State* L)
{
	m_pLuaState = L;
	return true;
}

lua_State* LuaStack::GetLuaState()
{
	return m_pLuaState;
}

void LuaStack::Clean()
{
	lua_settop(m_pLuaState, 0);
}

void LuaStack::PushNil()
{
	lua_pushnil(m_pLuaState);
}

void LuaStack::PushBool(bool bValue)
{
	lua_pushboolean(m_pLuaState, bValue);
}

void LuaStack::PushInt(int32_t nValue)
{
	lua_pushinteger(m_pLuaState, nValue);
}

void LuaStack::PushInt64(int64_t nValue)
{
	lua_pushinteger(m_pLuaState, nValue);
}

void LuaStack::PushFloat(float fValue)
{
	lua_pushnumber(m_pLuaState, fValue);
}

void LuaStack::PushDouble(double fValue)
{
	lua_pushnumber(m_pLuaState, fValue);
}

void LuaStack::PushString(const char* pszValue)
{
	lua_pushstring(m_pLuaState, pszValue);
}

void LuaStack::PushString(const char* pszValue, size_t nLength)
{
	lua_pushlstring(m_pLuaState, pszValue, nLength);
}

void LuaStack::PushString(const std::string& strValue)
{
	lua_pushstring(m_pLuaState, strValue.c_str());
}

int32_t LuaStack::ExecuteGlobalFunction(const char* pszFuncName)
{
	lua_getglobal(m_pLuaState, pszFuncName);       /* query function by name, stack: function */
	if (!lua_isfunction(m_pLuaState, -1))
	{
		LOG(INFO) << "[LUA ERROR] name '" << pszFuncName << "' does not represent a Lua function";
		lua_pop(m_pLuaState, 1);
		return 0;
	}
	return ExecuteFunction(0);
}

int32_t LuaStack::ExecuteFunction(int32_t nArgs)
{
	int functionIndex = -(nArgs + 1);
	if (!lua_isfunction(m_pLuaState, functionIndex))
	{
		LOG(INFO) << "value at stack [" << functionIndex << "] is not function";
		lua_pop(m_pLuaState, nArgs + 1); // remove function and arguments
		return 0;
	}

	int traceback = 0;
	lua_getglobal(m_pLuaState, "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
	if (!lua_isfunction(m_pLuaState, -1))
	{
		lua_pop(m_pLuaState, 1);                                            /* L: ... func arg1 arg2 ... */
	}
	else
	{
		lua_insert(m_pLuaState, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
		traceback = functionIndex - 1;
	}

	int error = 0;
	++m_nCallFromLua;
	error = lua_pcall(m_pLuaState, nArgs, 1, traceback);                  /* L: ... [G] ret */
	--m_nCallFromLua;
	if (error)
	{
		if (traceback == 0)
		{
			LOG(INFO) << "[LUA ERROR] " << lua_tostring(m_pLuaState, -1);        /* L: ... error */
			lua_pop(m_pLuaState, 1); // remove error message from stack
		}
		else                                                            /* L: ... G error */
		{
			lua_pop(m_pLuaState, 2); // remove __G__TRACKBACK__ and error message from stack
		}
		return 0;
	}

	// get return value
	int ret = 0;
	if (lua_isnumber(m_pLuaState, -1))
	{
		ret = (int)lua_tointeger(m_pLuaState, -1);
	}
	else if (lua_isboolean(m_pLuaState, -1))
	{
		ret = (int)lua_toboolean(m_pLuaState, -1);
	}
	// remove return value from stack
	lua_pop(m_pLuaState, 1);                                                /* L: ... [G] */

	if (traceback)
	{
		lua_pop(m_pLuaState, 1); // remove __G__TRACKBACK__ from stack      /* L: ... */
	}

	return ret;
}

int32_t LuaStack::ExecuteString(const char* pszCodes)
{
	luaL_loadstring(m_pLuaState, pszCodes);
	return ExecuteFunction(0);
}

int32_t LuaStack::ExecuteScriptFile(const char* pszFileName)
{
	static const std::string BYTECODE_FILE_EXT = ".luac";
	static const std::string NOT_BYTECODE_FILE_EXT = ".lua";

	std::string buf(pszFileName);
	//
	// remove .lua or .luac
	//
	size_t pos = buf.rfind(BYTECODE_FILE_EXT);
	if (pos != std::string::npos)
	{
		buf = buf.substr(0, pos);
	}
	else
	{
		pos = buf.rfind(NOT_BYTECODE_FILE_EXT);
		if (pos == buf.length() - NOT_BYTECODE_FILE_EXT.length())
		{
			buf = buf.substr(0, pos);
		}
	}

	//
	// 1. check .luac suffix
	// 2. check .lua suffix
	//

	std::string tmpfilename = buf + BYTECODE_FILE_EXT;
	int32_t nLength = 0;
	auto pBuffer = IOUtil::ReadFile(tmpfilename.c_str(), nLength);
	if (pBuffer)
	{
		buf = tmpfilename;
	}
	else
	{
		tmpfilename = buf + NOT_BYTECODE_FILE_EXT;
		pBuffer = IOUtil::ReadFile(tmpfilename.c_str(), nLength);
		if (pBuffer)
		{
			buf = tmpfilename;
		}
	}

	int rn = 0;
	if (pBuffer)
	{
		if (LoadBuffer(m_pLuaState, pBuffer.get(), nLength, pszFileName) == 0)
		{
			rn = ExecuteFunction(0);
		}
	}
	return rn;
}

namespace {

	void SkipBOM(const char*& pszChunk, int32_t& nChunkSize)
	{
		// UTF-8 BOM? skip
		if (static_cast<unsigned char>(pszChunk[0]) == 0xEF &&
			static_cast<unsigned char>(pszChunk[1]) == 0xBB &&
			static_cast<unsigned char>(pszChunk[2]) == 0xBF)
		{
			pszChunk += 3;
			nChunkSize -= 3;
		}
	}

} // end anonymous namespace

int32_t LuaStack::LoadBuffer(lua_State* L, const char* pszChunk, int32_t nChunkSize, const char* pszChunkName)
{
	SkipBOM(pszChunk, nChunkSize);
	int32_t r = luaL_loadbuffer(L, pszChunk, nChunkSize, pszChunkName);

#if _DEBUG
	if (r)
	{
		switch (r)
		{
		case LUA_ERRSYNTAX:
			LOG(INFO) << "[LUA ERROR] load \"" << pszChunkName << "\", error: syntax error during pre-compilation.";
			break;
		case LUA_ERRMEM:
			LOG(INFO) << "[LUA ERROR] load \"" << pszChunkName << "\", error: memory allocation error.";
			break;
		case LUA_ERRFILE:
			LOG(INFO) << "[LUA ERROR] load \"" << pszChunkName << "\", error: cannot open/read file.";
			break;
		default:
			LOG(INFO) << "[LUA ERROR] load \"" << pszChunkName << "\", error: unknown.";
			break;
		}
	}
#endif
	return r;
}
