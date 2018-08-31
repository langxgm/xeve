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
#include <functional>

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
class MessagePacker;
typedef std::unique_ptr<MessagePacker> MessagePackerPtr;

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
	// 创建消息包装器
	//------------------------------------------------------------------------
	virtual MessagePacker* CreateMessagePacker();

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
	virtual void OnMissMessage(uint32_t nMsgID, const void* pMsg, size_t nLen, int64_t nSessionID, const MessageMetaPtr& pMeta) {}

	//------------------------------------------------------------------------
	// 转发buffer(异步的)
	//------------------------------------------------------------------------
	virtual void OnTransfer(uint32_t nMsgID, const void* pBuf, size_t nLen, int64_t nSessionID, const MessageMetaPtr& pMeta) {}

	//------------------------------------------------------------------------
	// 获得连接
	//------------------------------------------------------------------------
	evpp::TCPConnPtr GetConnPtr(int64_t nSessionID);

	//------------------------------------------------------------------------
	// 关闭连接
	//------------------------------------------------------------------------
	void CloseConn(int64_t nSessionID);

	//------------------------------------------------------------------------
	// 获得连接数
	//------------------------------------------------------------------------
	uint64_t GetSessionNum();

	//------------------------------------------------------------------------
	// 设置认证标记
	//------------------------------------------------------------------------
	void SetAuth(int64_t nSessionID, bool isPass);

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
	int Send(int64_t nSessionID, const ::google::protobuf::Message* pMsg, const MessageMeta& rMeta = MessageMeta());

	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	int Send(const std::vector<int64_t>& vecSessionID, const ::google::protobuf::Message* pMsg, const MessageMeta& rMeta = MessageMeta());

	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	int Send(const std::function<int64_t(void)>& funcNext, const ::google::protobuf::Message* pMsg, const MessageMeta& rMeta = MessageMeta());

	//------------------------------------------------------------------------
	// 发送给集合
	//------------------------------------------------------------------------
	template<class collection_type>
	int SendTo(const collection_type& coll, const ::google::protobuf::Message* pMsg, const MessageMeta& rMeta = MessageMeta())
	{
		static_assert(std::is_base_of<std::input_iterator_tag, typename collection_type::iterator::iterator_category>::value, "not collection type");
		auto itBegin = coll.begin(), itEnd = coll.end();
		return Send([&itBegin, &itEnd]() { return itBegin != itEnd ? *itBegin++ : 0; }, pMsg, rMeta);
	}

	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	int Send(int64_t nSessionID, const void* pMsg, size_t nLen);

	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	int Send(const std::vector<int64_t>& vecSessionID, const void* pMsg, size_t nLen);

	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	void Send(const evpp::TCPConnPtr& conn, const void* pMsg, size_t nLen);

	//------------------------------------------------------------------------
	// 发送给所有连接
	//------------------------------------------------------------------------
	void SendToAll(const ::google::protobuf::Message* pMsg, const MessageMeta& rMeta = MessageMeta());

	//------------------------------------------------------------------------
	// 消息统计接口
	//------------------------------------------------------------------------
	virtual MessageCounter* Counter() { return nullptr; }

public:
	//------------------------------------------------------------------------
	// 获得发送缓冲区分配器
	//------------------------------------------------------------------------
	BufferAllocator& GetBufferAllocator() { return m_aWriteBufferAllocator; }

	//------------------------------------------------------------------------
	// 消息写入buffer
	//------------------------------------------------------------------------
	void WriteBuffer(evpp::Buffer* pWriteBuffer, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta);

	//------------------------------------------------------------------------
	// 消息写入buffer
	//------------------------------------------------------------------------
	void WriteBuffer(evpp::Buffer* pWriteBuffer, const void* pMsg, size_t nMsgLen, const MessageMeta* pMeta);

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
	const MessagePackerPtr& GetMessagePacker() { return m_pMessagePacker; }
	uint32_t& MaxBufLen() { return m_nMaxBufLen; }
	uint32_t& MaxMessageLen() { return m_nMaxMessageLen; }
	uint32_t& MaxDealPerFrame() { return m_nMaxDealPerFrame; }
	int64_t& AuthTimeout() { return m_nAuthTimeout; }

protected:
	// 事件管理器
	evpp::EventLoop* m_pLoop = nullptr;

	// 创建连接的工厂
	SessionFactoryPtr m_pSessionFactory;

	// 消息包装器
	MessagePackerPtr m_pMessagePacker;

	// 分段的连接集合
	std::vector<SessionMgrPtr> m_vecSessions;
	// 段数
	uint32_t m_nSectionNum = 0;

	// 消息buff长度限制,超出就被关闭连接(默认限制20MB)
	uint32_t m_nMaxBufLen = 1048576 * 20;

	// 消息长度限制,超出就被关闭连接
	uint32_t m_nMaxMessageLen = 65535;
	// 每个连接,每帧处理的消息数量
	uint32_t m_nMaxDealPerFrame = 100;
	// 连接认证超时时间(单位:毫秒,0:不会超时)
	int64_t m_nAuthTimeout = 0;

	// 发送缓冲区分配器,避免每次new
	BufferAllocator m_aWriteBufferAllocator = { 1 };
};
