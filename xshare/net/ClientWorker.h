//------------------------------------------------------------------------
// * @filename: ClientWorker.h
// *
// * @brief: 客户端
// *
// * @author: XGM
// * @date: 2017/08/09
//------------------------------------------------------------------------
#pragma once

#include "MessageReceiver.h"

#include <atomic>

namespace evpp {
	class TCPClient;
	class EventLoopThreadPool;
}

class ClientWorker : public MessageReceiver
{
public:
	ClientWorker();
	virtual ~ClientWorker();

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(evpp::EventLoop* loop,
		const std::string& remote_addr/*ip:port*/,
		const std::string& name,
		uint32_t thread_num,
		uint32_t session_num);

	//------------------------------------------------------------------------
	// 创建连接的工厂
	//------------------------------------------------------------------------
	virtual SessionFactory* CreateSessionFactory() override;

	//------------------------------------------------------------------------
	// 启动
	//------------------------------------------------------------------------
	virtual bool Start() override;

	//------------------------------------------------------------------------
	// 停止
	//------------------------------------------------------------------------
	virtual void Stop() override;

public:
	//------------------------------------------------------------------------
	// 接收连接/断开
	//------------------------------------------------------------------------
	virtual void OnConnectionEx(const evpp::TCPConnPtr& conn) final;

	//------------------------------------------------------------------------
	// 连接成功(异步的)
	//------------------------------------------------------------------------
	virtual void OnConnected(int64_t nSessionID) {}

	//------------------------------------------------------------------------
	// 发送消息给唯一的连接
	//------------------------------------------------------------------------
	int Send1(const ::google::protobuf::Message* pMsg, const MessageMeta& rMeta = MessageMeta());

	//------------------------------------------------------------------------
	// 发送消息给唯一的连接
	//------------------------------------------------------------------------
	int Send1(const void* pMsg, size_t nLen);

protected:
	// 事件管理器池
	std::shared_ptr<evpp::EventLoopThreadPool> m_pLoopPool;

	// TCP客户端连接列表
	std::vector<evpp::TCPClient*> m_vecClient;

	// 唯一的连接
	std::atomic<int64_t> m_nUniqueSessionID = { -1 };
};
