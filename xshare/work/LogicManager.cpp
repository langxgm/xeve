
#include "LogicManager.h"

#include <assert.h>

LogicManager::LogicManager()
{

}

LogicManager::~LogicManager()
{
	while (m_Logics.size())
	{
		delete m_Logics.back();
		m_Logics.pop_back();
	}

	m_mapObject.clear();
}

bool LogicManager::RegLogic(const key_type& key, value_type value)
{
	if (mybase_manager::RegObject(key, value) == true)
	{
		m_Logics.push_back(value);
		return true;
	}
	assert(false && "Logic key exist");
	return false;
}

LogicManager::value_type LogicManager::GetLogic(const key_type& key)
{
	return mybase_manager::GetObject(key);
}

LogicManager::value_type LogicManager::GetLogicByIndex(uint32_t index)
{
	if (index < m_Logics.size())
	{
		return m_Logics[index];
	}
	return nullptr;
}

uint32_t LogicManager::GetSize()
{
	return static_cast<uint32_t>(m_Logics.size());
}

void LogicManager::PrintAll(std::ostream& out)
{
	out << "//------------------------------------------------------------------------" << "\n";
	out << "// LogicManager:" << "\n";
	for (uint32_t i = 0; i < m_Logics.size(); ++i)
	{
		auto v = m_Logics[i];
		out << "// [" << i << "]: " << v->GetLogicName() << "\n";
	}
	out << "//------------------------------------------------------------------------" << "\n";
}

LogicManager::iterator LogicManager::begin()
{
	return iterator(this);
}

LogicManager::iterator LogicManager::end()
{
	return iterator(nullptr);
}

LogicManager::iterator::iterator(LogicManager* pMgr)
	: m_pMgr(pMgr)
{
	if (pMgr != nullptr)
	{
		m_itCurrent = pMgr->m_Logics.begin();
	}
}

bool operator==(const LogicManager::iterator& lhs, const LogicManager::iterator& rhs)
{
	return lhs.m_pMgr == rhs.m_pMgr
		&& lhs.m_itCurrent == rhs.m_itCurrent;
}

bool operator!=(const LogicManager::iterator& lhs, const LogicManager::iterator& rhs)
{
	return !(lhs == rhs);
}

const LogicManager::iterator::value_type& LogicManager::iterator::operator*() const
{
	assert(m_itCurrent != m_pMgr->m_Logics.end());
	return *m_itCurrent;
}

LogicManager::iterator& LogicManager::iterator::operator++()
{
	assert(m_itCurrent != m_pMgr->m_Logics.end());
	if (++m_itCurrent == m_pMgr->m_Logics.end())
	{
		m_pMgr = nullptr;
		m_itCurrent = decltype(m_itCurrent)();
	}
	return *this;
}

void LogicManager::iterator::operator++(int)
{
	operator++();
}
