
#include "ClientWorker.h"
#include "SessionConcreteFactory.h"

#include <evpp/tcp_conn.h>
#include <evpp/tcp_client.h>
#include <evpp/event_loop_thread_pool.h>

ClientWorker::ClientWorker()
{

}

ClientWorker::~ClientWorker()
{
	for (auto& client : m_vecClient)
	{
		delete client;
	}
	m_vecClient.clear();

	while (!m_pLoopPool->IsStopped())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	m_pLoopPool.reset();
}

void ClientWorker::Init(evpp::EventLoop* loop, const std::string& remote_addr,
	const std::string& name, uint32_t thread_num, uint32_t session_num)
{
	// addr,addr,...
	std::vector<std::string> vecAddr;
	evpp::StringSplit(remote_addr, ",", 0, vecAddr);

	// 预计连接数/50,对连接分片管理
	MessageReceiver::Init(loop, vecAddr.size() / 50);

	m_pLoopPool.reset(new evpp::EventLoopThreadPool(loop, thread_num));
	m_pLoopPool->Start(true);

	for (uint32_t i = 0; i < vecAddr.size(); ++i)
	{
		std::string strConnName = name + "." + std::to_string(i);
		evpp::TCPClient* pClient = new evpp::TCPClient(m_pLoopPool->GetNextLoop(), vecAddr[i], strConnName);
		pClient->SetConnectionCallback(std::bind(&ClientWorker::OnConnectionEx, this, std::placeholders::_1));
		pClient->SetMessageCallback(std::bind(&MessageReceiver::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
		pClient->set_auto_reconnect(true);
		pClient->set_reconnect_interval(evpp::Duration(3.0));
		pClient->set_connecting_timeout(evpp::Duration(3.0));
		m_vecClient.push_back(pClient);
	}
}

SessionFactory* ClientWorker::CreateSessionFactory()
{
	return new SessionDefaultFactory();
}

bool ClientWorker::Start()
{
	for (auto& client : m_vecClient)
	{
		client->Connect();
	}
	return true;
}

void ClientWorker::Stop()
{
	for (auto& client : m_vecClient)
	{
		client->Disconnect();
	}
	m_pLoopPool->Stop(true);
}

void ClientWorker::OnConnectionEx(const evpp::TCPConnPtr& conn)
{
	MessageReceiver::OnConnection(conn);

	// 连接/断开
	if (conn->IsConnected())
	{
		m_nUniqueSessionID = conn->id();
		OnConnected(conn->id());
	}
	else
	{
		m_nUniqueSessionID = -1;
	}
}

int ClientWorker::Send1(const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta)
{
	return Send(m_nUniqueSessionID, pMsg, pMeta);
}

int ClientWorker::Send1(const void* pMsg, size_t nLen)
{
	return Send(m_nUniqueSessionID, pMsg, nLen);
}
