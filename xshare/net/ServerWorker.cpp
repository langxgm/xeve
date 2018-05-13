
#include "ServerWorker.h"
#include "SessionConcreteFactory.h"

#include <evpp/tcp_server.h>

ServerWorker::ServerWorker()
{

}

ServerWorker::~ServerWorker()
{
	delete m_pServer;
}

void ServerWorker::Init(evpp::EventLoop* loop, const std::string& listen_addr,
	const std::string& name, uint32_t thread_num, uint32_t session_num)
{
	// 预计连接数/50,对连接分片管理
	MessageReceiver::Init(loop, session_num / 50);

	m_pServer = new evpp::TCPServer(loop, listen_addr, name, thread_num);
	m_pServer->SetConnectionCallback(std::bind(&MessageReceiver::OnConnection, this, std::placeholders::_1));
	m_pServer->SetMessageCallback(std::bind(&MessageReceiver::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
	m_pServer->Init();
}

SessionFactory* ServerWorker::CreateSessionFactory()
{
	return new SessionDefaultFactory();
}

bool ServerWorker::Start()
{
	return m_pServer->Start();
}

void ServerWorker::Stop()
{
	m_pServer->Stop();

	// 等待关闭
	while (!m_pServer->IsStopped())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
