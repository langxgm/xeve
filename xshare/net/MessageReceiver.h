//------------------------------------------------------------------------
// * @filename: ServerHandler.h
// *
// * @brief: 消息接收者
// *
// * @author: XGM
// * @date: 2017/08/04
//------------------------------------------------------------------------
#pragma once

#include "MessageRegistry.h"
#include "BufferAllocator.h"

#include <vector>

namespace evpp {
	class EventLoop;
	class TCPServer;
	class Buffer;
	class TCPConn;
	typedef std::shared_ptr<TCPConn> TCPConnPtr;
}

class MessageCounter;
class SessionManager;
typedef std::shared_ptr<SessionManager> SessionMgrPtr;
class SessionFactory;
typedef std::unique_ptr<SessionFactory> SessionFactoryPtr;

class MessageReceiver : public MessageRegistry
{
public:
	MessageReceiver();
	virtual ~MessageReceiver();

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(evpp::EventLoop* pLoop, uint32_t nSectionNum);

	//------------------------------------------------------------------------
	// 创建连接的工厂
	//------------------------------------------------------------------------
	virtual SessionFactory* CreateSessionFactory() = 0;

	//------------------------------------------------------------------------
	// 启动
	//------------------------------------------------------------------------
	virtual bool Start() = 0;

	//------------------------------------------------------------------------
	// 停止
	//------------------------------------------------------------------------
	virtual void Stop() = 0;

	//------------------------------------------------------------------------
	// 接收连接/断开
	//------------------------------------------------------------------------
	virtual void OnConnection(const evpp::TCPConnPtr& conn) final;

	//------------------------------------------------------------------------
	// 接收消息
	//------------------------------------------------------------------------
	virtual void OnMessage(const evpp::TCPConnPtr& conn, evpp::Buffer* pBuf) final;

	//------------------------------------------------------------------------
	// 添加缓存消息
	//------------------------------------------------------------------------
	virtual void CacheMessage(MessagePacket& aMsgPacket, int64_t nSessionID);

	//------------------------------------------------------------------------
	// 每帧执行消息
	//------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------
	// 断开连接
	//------------------------------------------------------------------------
	virtual void OnDisconnect(int64_t nSessionID) {}

	//------------------------------------------------------------------------
	// 处理未注册的消息(异步的)
	//------------------------------------------------------------------------
	virtual void OnMissMessage(uint32_t nMsgID, const void* pMsg, size_t nLen, int64_t nSessionID) {}

	//------------------------------------------------------------------------
	// 获得连接
	//------------------------------------------------------------------------
	evpp::TCPConnPtr GetConnPtr(int64_t nSessionID);

	//------------------------------------------------------------------------
	// 获得连接数
	//------------------------------------------------------------------------
	uint64_t GetSessionNum();

private:
	//------------------------------------------------------------------------
	// 获取分段下标
	//------------------------------------------------------------------------
	uint32_t GetSectionIndex(int64_t nSessionID);

	//------------------------------------------------------------------------
	// 获取分段的连接集合
	//------------------------------------------------------------------------
	const SessionMgrPtr& GetSessionMgrPtr(int64_t nSessionID);

public:
	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	int Send(int64_t nSessionID, const ::google::protobuf::Message* pMsg);

	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	void Send(const evpp::TCPConnPtr& conn, const void* pMsg, size_t nLen);

	//------------------------------------------------------------------------
	// 发送给所有连接
	//------------------------------------------------------------------------
	void SendToAll(const ::google::protobuf::Message* pMsg);

	//------------------------------------------------------------------------
	// 消息统计接口
	//------------------------------------------------------------------------
	virtual MessageCounter* Counter() { return nullptr; }

protected:
	//------------------------------------------------------------------------
	// 消息写入buffer
	//------------------------------------------------------------------------
	void WriteBuffer(evpp::Buffer* pWriteBuffer, const ::google::protobuf::Message* pMsg);

	//------------------------------------------------------------------------
	// 重置buffer
	//------------------------------------------------------------------------
	void ResetBuffer(evpp::Buffer* pWriteBuffer);

public:
	//------------------------------------------------------------------------
	// 获得内部状态信息
	//------------------------------------------------------------------------
	virtual std::string GetStatusInfo();

public:
	evpp::EventLoop* GetEventLoop() { return m_pLoop; }
	uint32_t& MaxBufLen() { return m_nMaxBufLen; }
	uint32_t& MaxMessageLen() { return m_nMaxMessageLen; }
	uint32_t& MaxDealPerFrame() { return m_nMaxDealPerFrame; }

protected:
	// 事件管理器
	evpp::EventLoop* m_pLoop = nullptr;

	// 创建连接的工厂
	SessionFactoryPtr m_pSessionFactory;

	// 分段的连接集合
	std::vector<SessionMgrPtr> m_vecSessions;
	// 段数
	uint32_t m_nSectionNum = 0;

	// 消息buff长度限制,超出就被关闭连接(默认限制20MB)
	uint32_t m_nMaxBufLen = 1048576 * 20;

	// 消息长度占用的字节数
	uint32_t m_nHeaderLen = 4;
	// 消息长度限制,超出就被关闭连接
	uint32_t m_nMaxMessageLen = 65535;
	// 消息ID长度占用的字节数
	uint32_t m_nMsgIDLen = 4;
	// 每个连接,每帧处理的消息数量
	uint32_t m_nMaxDealPerFrame = 100;

	// 发送缓冲区分配器,避免每次new
	BufferAllocator m_aWriteBufferAllocator = { 1 };
};
