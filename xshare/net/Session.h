//------------------------------------------------------------------------
// * @filename: Session.h
// *
// * @brief: 连接
// *
// * @author: XGM
// * @date: 2017/08/13
//------------------------------------------------------------------------
#pragma once

#include "MessagePacket.h"

#include <list>
#include <string>

namespace evpp {
	class TCPConn;
	typedef std::shared_ptr<TCPConn> TCPConnPtr;
}

class SessionStatistics;
typedef std::unique_ptr<SessionStatistics> SessionStatPtr;

class Session
{
public:
	Session(evpp::TCPConnPtr conn);
	~Session();

	//------------------------------------------------------------------------
	// 获取ID
	//------------------------------------------------------------------------
	uint64_t GetID();

	//------------------------------------------------------------------------
	// 获取连接
	//------------------------------------------------------------------------
	evpp::TCPConnPtr GetConn() { return m_pConn; }

	//------------------------------------------------------------------------
	// 连接是否有效
	//------------------------------------------------------------------------
	bool Connected() { return m_bConnected; }
	void SetConnected(bool b) { m_bConnected = b; }

	//------------------------------------------------------------------------
	// 建立连接的时间
	//------------------------------------------------------------------------
	int64_t GetConnectedTime() const { return m_nConnectedTime; }
	void SetConnectedTime(int64_t nTime) { m_nConnectedTime = nTime; }

	//------------------------------------------------------------------------
	// 是否认证
	//------------------------------------------------------------------------
	bool IsAuth() const { return m_bAuth; }
	void SetAuth(bool bAuth) { m_bAuth = bAuth; }

	//------------------------------------------------------------------------
	// 消息统计
	//------------------------------------------------------------------------
	void SetStat(SessionStatPtr&& pStat);
	SessionStatistics* GetStat() { return m_pStat.get(); }

	//------------------------------------------------------------------------
	// 添加缓存消息
	//------------------------------------------------------------------------
	virtual void AddCacheMessage(MessagePacket& aMsgPacket);

	//------------------------------------------------------------------------
	// 取出缓存消息
	//------------------------------------------------------------------------
	virtual void PopCacheMessage(MessagePacket& aMsgPacket);

	//------------------------------------------------------------------------
	// 取出指定数量的消息
	//------------------------------------------------------------------------
	virtual void PopCacheMessages(std::list<MessagePacket>& rList, uint32_t nMsgNum);

	//------------------------------------------------------------------------
	// 获得内部状态信息
	//------------------------------------------------------------------------
	virtual std::string GetStatusInfo();

protected:
	// 连接
	evpp::TCPConnPtr m_pConn;
	// 连接有效
	bool m_bConnected = false;
	// 建立连接的时间
	int64_t m_nConnectedTime = 0;
	// 认证
	bool m_bAuth = false;

	// 缓存的消息
	std::list<MessagePacket> m_listMessage;

	// 消息统计
	SessionStatPtr m_pStat;
};
