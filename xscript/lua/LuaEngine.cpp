
#include "LuaEngine.h"
#include "LuaStack.h"

LuaEngine::LuaEngine()
{

}

LuaEngine::~LuaEngine()
{
	if (m_pStack)
	{
		delete m_pStack;
	}
}

bool LuaEngine::Init()
{
	m_pStack = new LuaStack();
	m_pStack->Init();
	return true;
}

LuaStack* LuaEngine::GetLuaStack()
{
	return m_pStack;
}

int32_t LuaEngine::ExecuteString(const char* pszCodes)
{
	int32_t ret = m_pStack->ExecuteString(pszCodes);
	m_pStack->Clean();
	return ret;
}

int32_t LuaEngine::ExecuteScriptFile(const char* pszFileName)
{
	int32_t ret = m_pStack->ExecuteScriptFile(pszFileName);
	m_pStack->Clean();
	return ret;
}

int32_t LuaEngine::ExecuteGlobalFunction(const char* pszFuncName)
{
	int32_t ret = m_pStack->ExecuteGlobalFunction(pszFuncName);
	m_pStack->Clean();
	return ret;
}
