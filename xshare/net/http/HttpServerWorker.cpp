
#include "HttpServerWorker.h"

#include <evpp/http/http_server.h>

#include <thread>

HttpServerWorker::HttpServerWorker()
{

}

HttpServerWorker::~HttpServerWorker()
{
	delete m_pServer;
}

void HttpServerWorker::Init(evpp::EventLoop* loop,
	const std::string& listen_ports/*like "80,8080,443"*/,
	uint32_t thread_num)
{
	m_pLoop = loop;

	m_pServer = new evpp::http::Server(thread_num);
	m_pServer->Init(listen_ports);

	RegisterCallback();
}

void HttpServerWorker::RegisterHandler(const std::string& uri, evpp::http::HTTPRequestCallback callback)
{
	m_pServer->RegisterHandler(uri, callback);
}

void HttpServerWorker::RegisterDefaultHandler(evpp::http::HTTPRequestCallback callback)
{
	m_pServer->RegisterDefaultHandler(callback);
}

bool HttpServerWorker::Start()
{
	return m_pServer->Start();
}

void HttpServerWorker::Stop()
{
	m_pServer->Stop();
	std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep a while to release the listening address and port
}
