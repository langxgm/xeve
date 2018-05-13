//------------------------------------------------------------------------
// * @filename: SessionManager.h
// *
// * @brief: 连接集合的管理
// *
// * @author: XGM
// * @date: 2017/08/13
//------------------------------------------------------------------------
#pragma once

#include <map>
#include <mutex>
#include <memory>

class Session;
typedef std::shared_ptr<Session> SessionPtr;
typedef std::map<int64_t, SessionPtr> SessionPtrMap;

// 遍历列表的函数
typedef std::function<void(const SessionPtr&)> ForeachSession_F;
typedef std::function<void(const SessionPtrMap&)> VisitSessions_F;

class SessionManager
{
public:
	SessionManager();
	~SessionManager();

	//------------------------------------------------------------------------
	// 添加Session
	//------------------------------------------------------------------------
	bool Add(int64_t nSessionID, const SessionPtr& pSession);

	//------------------------------------------------------------------------
	// 获取Session
	//------------------------------------------------------------------------
	const SessionPtr& Get(int64_t nSessionID);

	//------------------------------------------------------------------------
	// 移除Session
	//------------------------------------------------------------------------
	void Remove(int64_t nSessionID);

	//------------------------------------------------------------------------
	// 获得Session数量
	//------------------------------------------------------------------------
	uint64_t Size();

	//------------------------------------------------------------------------
	// 获得锁
	//------------------------------------------------------------------------
	std::recursive_mutex& GetLock() { return m_aMutex; }

	//------------------------------------------------------------------------
	// 遍历集合
	//------------------------------------------------------------------------
	void Foreach(const ForeachSession_F& func);

	//------------------------------------------------------------------------
	// 访问集合
	//------------------------------------------------------------------------
	void Visit(const VisitSessions_F& func);

protected:
	// 同步锁
	std::recursive_mutex m_aMutex;
	// 连接集合<SessionID, Session指针>
	SessionPtrMap m_mapSession;
};
