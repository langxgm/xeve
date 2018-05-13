//------------------------------------------------------------------------
// * @filename: ModuleManager.h
// *
// * @brief: 模块管理
// *
// * @author: XGM
// * @date: 2017/03/08
//------------------------------------------------------------------------
#pragma once

#include "Manager.h"
#include "Singleton.h"

#include <string>
#include <vector>

class Module;

class ModuleManager : protected Manager<std::string, Module*>, public Singleton<ModuleManager>
{
public:
	typedef Manager<std::string, Module*> mybase_manager;
	typedef typename mybase_manager::key_type key_type;
	typedef typename mybase_manager::value_type value_type;
protected:
	friend class Singleton<ModuleManager>;
	//------------------------------------------------------------------------
	// 构造
	//------------------------------------------------------------------------
	ModuleManager();

	//------------------------------------------------------------------------
	// 析构
	//------------------------------------------------------------------------
	virtual ~ModuleManager();
public:
	//------------------------------------------------------------------------
	// 注册模块
	//------------------------------------------------------------------------
	virtual bool RegModule(const std::string& key, Module* value);

	//------------------------------------------------------------------------
	// 获取模块
	//------------------------------------------------------------------------
	virtual Module* GetModule(const std::string& key);

	//------------------------------------------------------------------------
	// 通过下标获取模块
	//------------------------------------------------------------------------
	Module* GetModuleByIndex(uint32_t index);

	//------------------------------------------------------------------------
	// 获取模块数量
	//------------------------------------------------------------------------
	uint32_t GetSize();

	//------------------------------------------------------------------------
	// 打印所有模块
	//------------------------------------------------------------------------
	virtual void PrintAll(std::ostream& out) override;
private:
	std::vector<Module*> m_Modules;
};
