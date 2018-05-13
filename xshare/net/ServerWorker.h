//------------------------------------------------------------------------
// * @filename: ServerWorker.h
// *
// * @brief: 服务器
// *
// * @author: XGM
// * @date: 2017/08/09
//------------------------------------------------------------------------
#pragma once

#include "MessageReceiver.h"

class ServerWorker : public MessageReceiver
{
public:
	ServerWorker();
	virtual ~ServerWorker();

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(evpp::EventLoop* loop,
		const std::string& listen_addr/*ip:port*/,
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

protected:
	// TCP服务器
	evpp::TCPServer* m_pServer = nullptr;
};
