
#include "DBHandlerManager.h"

#include <assert.h>

void DBHandlerManager::ClearRef()
{
	m_mapObject.clear();
}

DBHandlerManager::iterator DBHandlerManager::begin()
{
	return iterator(this);
}

DBHandlerManager::iterator DBHandlerManager::end()
{
	return iterator(nullptr);
}

DBHandlerManager::iterator::iterator(DBHandlerManager* pMgr)
	: m_pMgr(pMgr)
{
	if (pMgr != nullptr)
	{
		m_itCurrent = pMgr->m_mapObject.begin();
	}
}

bool operator==(const DBHandlerManager::iterator& lhs, const DBHandlerManager::iterator& rhs)
{
	return lhs.m_pMgr == rhs.m_pMgr
		&& lhs.m_itCurrent == rhs.m_itCurrent;
}

bool operator!=(const DBHandlerManager::iterator& lhs, const DBHandlerManager::iterator& rhs)
{
	return !(lhs == rhs);
}

const DBHandlerManager::iterator::value_type& DBHandlerManager::iterator::operator*() const
{
	assert(m_itCurrent != m_pMgr->m_mapObject.end());
	return m_itCurrent->second;
}

DBHandlerManager::iterator& DBHandlerManager::iterator::operator++()
{
	assert(m_itCurrent != m_pMgr->m_mapObject.end());
	if (++m_itCurrent == m_pMgr->m_mapObject.end())
	{
		m_pMgr = nullptr;
		m_itCurrent = decltype(m_itCurrent)();
	}
	return *this;
}

void DBHandlerManager::iterator::operator++(int)
{
	operator++();
}
