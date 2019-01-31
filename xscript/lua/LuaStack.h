//------------------------------------------------------------------------
// * @filename: LuaStack.h
// *
// * @brief: Lua堆栈
// *
// * @author: XGM
// * @date: 2018/12/04
//------------------------------------------------------------------------
#pragma once

#include <string>
#include <stdint.h>

struct lua_State;

class LuaStack
{
public:
	LuaStack();
	~LuaStack();

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	bool Init();
	bool InitWithLuaState(lua_State* L);

	//------------------------------------------------------------------------
	// 获得lua环境
	//------------------------------------------------------------------------
	lua_State* GetLuaState();

	//------------------------------------------------------------------------
	// Set the stack top index 0
	//------------------------------------------------------------------------
	void Clean();

	//------------------------------------------------------------------------
	// 将值压入堆栈
	//------------------------------------------------------------------------
	void PushNil();
	void PushBool(bool bValue);
	void PushInt(int32_t nValue);
	void PushInt64(int64_t nValue);
	void PushFloat(float fValue);
	void PushDouble(double fValue);
	void PushString(const char* pszValue);
	void PushString(const char* pszValue, size_t nLength);
	void PushString(const std::string& strValue);

	//	template<class collection_type>
	//	void PushCollection(const collection_type& coll)
	//	{
	//		for (auto& it : coll)
	//		{
	//			PushValue(*it);
	//		}
	//	}
	//
	//protected:
	//	void PushValue(nullptr_t);
	//	void PushValue(int32_t nValue);
	//	void PushValue(int64_t nValue);
	//	void PushValue(float fValue);
	//	void PushValue(double fValue);
	//	void PushValue(const char* pszValue);
	//	void PushValue(const std::string& strValue);

	//------------------------------------------------------------------------
	// 执行全局函数
	//------------------------------------------------------------------------
	int32_t ExecuteGlobalFunction(const char* pszFuncName);

	//------------------------------------------------------------------------
	// 执行函数
	//------------------------------------------------------------------------
	int32_t ExecuteFunction(int32_t nArgs);

	//------------------------------------------------------------------------
	// 执行代码块
	//------------------------------------------------------------------------
	int32_t ExecuteString(const char* pszCodes);

	//------------------------------------------------------------------------
	// 执行脚本文件
	//------------------------------------------------------------------------
	int32_t ExecuteScriptFile(const char* pszFileName);

	//------------------------------------------------------------------------
	// 加载代码块
	//------------------------------------------------------------------------
	int32_t LoadBuffer(lua_State* L, const char* pszChunk, int32_t nChunkSize, const char* pszChunkName);

private:
	// lua环境
	lua_State* m_pLuaState = nullptr;

	int32_t m_nCallFromLua = 0;
};
