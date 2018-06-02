//------------------------------------------------------------------------
// * @filename: HttpClientWorker.h
// *
// * @brief: HTTP客户端
// *
// * @author: XGM
// * @date: 2017/11/15
//------------------------------------------------------------------------
#pragma once

#include <string>
#include <memory>
#include <functional>
#include <stdint.h>

namespace evpp {
	class EventLoop;
	class EventLoopThreadPool;
	namespace httpc {
		class ConnPool;
		class Response;
		typedef std::function<void(const std::shared_ptr<Response>&)> Handler;
	}
}

typedef std::function<void(const std::shared_ptr<evpp::httpc::Response>&, evpp::EventLoop*)> HttpHandler;

// HTTPS的支持
#if defined(EVPP_HTTP_CLIENT_SUPPORTS_SSL)
extern void x_ssl_init_once();
extern void x_ssl_clean();
extern void* x_ssl_ctx();
extern int x_ssl_certificate(const char *CAfile);
#endif

class HttpClientWorker
{
public:
	HttpClientWorker();
	virtual ~HttpClientWorker();

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(evpp::EventLoop* loop,
		const std::string& host, int port,
#if defined(EVPP_HTTP_CLIENT_SUPPORTS_SSL)
		bool enable_ssl,
#endif
		uint32_t thread_num,
		uint32_t max_conn_pool,
		double timeout = 2.0);

	//------------------------------------------------------------------------
	// 启动
	//------------------------------------------------------------------------
	virtual bool Start();

	//------------------------------------------------------------------------
	// 停止
	//------------------------------------------------------------------------
	virtual void Stop();

	//------------------------------------------------------------------------
	// 每帧执行消息
	//------------------------------------------------------------------------
	virtual void Update() {}

	//------------------------------------------------------------------------
	// 执行HTTP-GET请求(使用连接池)
	//------------------------------------------------------------------------
	virtual void DoGet(const evpp::httpc::Handler& h, const std::string& uri_with_param);
	virtual void DoGet(const HttpHandler& h, const std::string& uri_with_param);

	//------------------------------------------------------------------------
	// 执行HTTP-GET请求(新建连接)
	//------------------------------------------------------------------------
	virtual void DoGetWithUrl(const evpp::httpc::Handler& h, const std::string& url, double timeout);
	virtual void DoGetWithUrl(const HttpHandler& h, const std::string& url, double timeout);

	//------------------------------------------------------------------------
	// 执行HTTP-POST请求(使用连接池)
	//------------------------------------------------------------------------
	virtual void DoPost(const evpp::httpc::Handler& h, const std::string& uri_with_param, const std::string& body);
	virtual void DoPost(const HttpHandler& h, const std::string& uri_with_param, const std::string& body);

	//------------------------------------------------------------------------
	// 执行HTTP-POST请求(新建连接)
	//------------------------------------------------------------------------
	virtual void DoPostWithUrl(const evpp::httpc::Handler& h, const std::string& url, const std::string& body, double timeout);
	virtual void DoPostWithUrl(const HttpHandler& h, const std::string& url, const std::string& body, double timeout);

public:
	//------------------------------------------------------------------------
	// 获得目标地址
	//------------------------------------------------------------------------
	const std::string& GetHost() const;

	//------------------------------------------------------------------------
	// 获得目标端口
	//------------------------------------------------------------------------
	int GetPort() const;

	//------------------------------------------------------------------------
	// 获得连接超时时间
	//------------------------------------------------------------------------
	double GetTimeout() const;

protected:
	// 事件管理器
	evpp::EventLoop* m_pLoop = nullptr;
	// 事件管理器池
	std::shared_ptr<evpp::EventLoopThreadPool> m_pLoopPool;
	// 连接池
	std::shared_ptr<evpp::httpc::ConnPool> m_pConnPool;
};
