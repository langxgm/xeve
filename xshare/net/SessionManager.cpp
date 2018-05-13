
#include "SessionManager.h"

SessionManager::SessionManager()
{

}

SessionManager::~SessionManager()
{

}

bool SessionManager::Add(int64_t nSessionID, const SessionPtr& pSession)
{
	auto ret = m_mapSession.insert(std::make_pair(nSessionID, pSession));
	return ret.second;
}

const SessionPtr& SessionManager::Get(int64_t nSessionID)
{
	auto itFind = m_mapSession.find(nSessionID);
	if (itFind != m_mapSession.end())
	{
		return itFind->second;
	}
	static SessionPtr null;
	return null;
}

void SessionManager::Remove(int64_t nSessionID)
{
	m_mapSession.erase(nSessionID);
}

uint64_t SessionManager::Size()
{
	return m_mapSession.size();
}

void SessionManager::Foreach(const ForeachSession_F& func)
{
	for (auto& it : m_mapSession)
	{
		func(it.second);
	}
}

void SessionManager::Visit(const VisitSessions_F& func)
{
	func(m_mapSession);
}
