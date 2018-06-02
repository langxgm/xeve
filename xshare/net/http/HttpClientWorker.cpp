
#include "HttpClientWorker.h"

#include <evpp/event_loop_thread_pool.h>

#include <evpp/httpc/conn_pool.h>
#include <evpp/httpc/request.h>

#if defined(EVPP_HTTP_CLIENT_SUPPORTS_SSL)
#include <evpp/httpc/ssl.h>
void x_ssl_init_once()
{
	static std::once_flag flag;
	std::call_once(flag, []() {
		evpp::httpc::InitSSL();
	});
}

void x_ssl_clean()
{
	evpp::httpc::CleanSSL();
}

void* x_ssl_ctx()
{
	return evpp::httpc::GetSSLCtx();
}

int x_ssl_certificate(const char *CAfile)
{
	return SSL_CTX_load_verify_locations(evpp::httpc::GetSSLCtx(), CAfile, nullptr);
}
#endif

HttpClientWorker::HttpClientWorker()
{

}

HttpClientWorker::~HttpClientWorker()
{
	m_pConnPool.reset();

	assert(m_pLoopPool->IsStopped());
	m_pLoopPool.reset();
}

void HttpClientWorker::Init(evpp::EventLoop* loop, const std::string& host, int port,
#if defined(EVPP_HTTP_CLIENT_SUPPORTS_SSL)
	bool enable_ssl,
#endif
	uint32_t thread_num, uint32_t max_conn_pool, double timeout /*= 2.0*/)
{
	m_pLoop = loop;

	m_pLoopPool.reset(new evpp::EventLoopThreadPool(loop, thread_num));

	m_pConnPool.reset(new evpp::httpc::ConnPool(
		host,
		port,
#if defined(EVPP_HTTP_CLIENT_SUPPORTS_SSL)
		enable_ssl,
#endif
		evpp::Duration(timeout),
		max_conn_pool));
}

bool HttpClientWorker::Start()
{
	m_pLoopPool->Start(true);
	assert(m_pLoopPool->IsRunning());
	return true;
}

void HttpClientWorker::Stop()
{
	m_pConnPool->Clear();

	// 这里为什么要休眠500毫秒?因为测试用例里是这么用的,暂时没研究原因
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	m_pLoopPool->Stop(true);
	assert(m_pLoopPool->IsStopped());
	m_pLoopPool->Join();
}

void HttpClientWorker::DoGet(const evpp::httpc::Handler& h, const std::string& uri_with_param)
{
	auto l = m_pLoopPool->GetNextLoop();
	auto r = new evpp::httpc::GetRequest(m_pConnPool.get(), l, uri_with_param);
	r->Execute(h);
}

void HttpClientWorker::DoGet(const HttpHandler& h, const std::string& uri_with_param)
{
	auto l = m_pLoopPool->GetNextLoop();
	auto r = new evpp::httpc::GetRequest(m_pConnPool.get(), l, uri_with_param);
	r->Execute(std::bind(h, std::placeholders::_1, l));
}

void HttpClientWorker::DoGetWithUrl(const evpp::httpc::Handler& h, const std::string& url, double timeout)
{
	auto l = m_pLoopPool->GetNextLoop();
	auto r = new evpp::httpc::GetRequest(l, url, evpp::Duration(timeout));
	r->Execute(h);
}

void HttpClientWorker::DoGetWithUrl(const HttpHandler& h, const std::string& url, double timeout)
{
	auto l = m_pLoopPool->GetNextLoop();
	auto r = new evpp::httpc::GetRequest(l, url, evpp::Duration(timeout));
	r->Execute(std::bind(h, std::placeholders::_1, l));
}

void HttpClientWorker::DoPost(const evpp::httpc::Handler& h, const std::string& uri_with_param, const std::string& body)
{
	auto l = m_pLoopPool->GetNextLoop();
	auto r = new evpp::httpc::PostRequest(m_pConnPool.get(), l, uri_with_param, body);
	r->Execute(h);
}

void HttpClientWorker::DoPost(const HttpHandler& h, const std::string& uri_with_param, const std::string& body)
{
	auto l = m_pLoopPool->GetNextLoop();
	auto r = new evpp::httpc::PostRequest(m_pConnPool.get(), l, uri_with_param, body);
	r->Execute(std::bind(h, std::placeholders::_1, l));
}

void HttpClientWorker::DoPostWithUrl(const evpp::httpc::Handler& h, const std::string& url, const std::string& body, double timeout)
{
	auto l = m_pLoopPool->GetNextLoop();
	auto r = new evpp::httpc::PostRequest(l, url, body, evpp::Duration(timeout));
	r->Execute(h);
}

void HttpClientWorker::DoPostWithUrl(const HttpHandler& h, const std::string& url, const std::string& body, double timeout)
{
	auto l = m_pLoopPool->GetNextLoop();
	auto r = new evpp::httpc::PostRequest(l, url, body, evpp::Duration(timeout));
	r->Execute(std::bind(h, std::placeholders::_1, l));
}

const std::string& HttpClientWorker::GetHost() const
{
	return m_pConnPool->host();
}

int HttpClientWorker::GetPort() const
{
	return m_pConnPool->port();
}

double HttpClientWorker::GetTimeout() const
{
	return m_pConnPool->timeout().Seconds();
}
