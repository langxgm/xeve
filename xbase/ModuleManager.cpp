
#include "ModuleManager.h"
#include "Module.h"

#include <assert.h>

ModuleManager::ModuleManager()
{

}

ModuleManager::~ModuleManager()
{

}

bool ModuleManager::RegModule(const std::string& key, Module* value)
{
	if (mybase_manager::RegObject(key, value) == true)
	{
		m_Modules.push_back(value);
		return true;
	}
	assert(false && "Module key exist");
	return false;
}

Module* ModuleManager::GetModule(const std::string& key)
{
	return mybase_manager::GetObject(key);
}

Module* ModuleManager::GetModuleByIndex(uint32_t index)
{
	if (index < m_Modules.size())
	{
		return m_Modules[index];
	}
	return nullptr;
}

uint32_t ModuleManager::GetSize()
{
	return static_cast<uint32_t>(m_Modules.size());
}

void ModuleManager::PrintAll(std::ostream& out)
{
	out << "//------------------------------------------------------------------------" << "\n";
	out << "// ModuleManager:" << "\n";
	for (uint32_t i = 0; i < m_Modules.size(); ++i)
	{
		Module* m = m_Modules[i];
		out << "// [" << i << "]: " << m->GetName() << "\n";
	}
	out << "//------------------------------------------------------------------------" << "\n";
}
