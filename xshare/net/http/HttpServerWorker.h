//------------------------------------------------------------------------
// * @filename: HttpServerWorker.h
// *
// * @brief: HTTP服务器
// *
// * @author: XGM
// * @date: 2017/11/23
//------------------------------------------------------------------------
#pragma once

#include "HttpCallback.h"

#include <string>
#include <stdint.h>

class HttpServerWorker
{
public:
	HttpServerWorker();
	virtual ~HttpServerWorker();

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(evpp::EventLoop* loop,
		const std::string& listen_ports/*like "80,8080,443"*/,
		uint32_t thread_num);

	//------------------------------------------------------------------------
	// 注册回调
	//------------------------------------------------------------------------
	virtual void RegisterCallback() = 0;

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

public:
	//------------------------------------------------------------------------
	// 注册访问回调
	//------------------------------------------------------------------------
	virtual void RegisterHandler(const std::string& uri, evpp::http::HTTPRequestCallback callback);

	//------------------------------------------------------------------------
	// 注册默认访问回调
	//------------------------------------------------------------------------
	virtual void RegisterDefaultHandler(evpp::http::HTTPRequestCallback callback);

protected:
	// 事件管理器
	evpp::EventLoop* m_pLoop = nullptr;
	// HTTP服务器
	evpp::http::Server* m_pServer = nullptr;
};

//------------------------------------------------------------------------
// 注册回调
//------------------------------------------------------------------------
#ifndef REG_CALLBACK
#define REG_CALLBACK(func, owner, uri) \
	RegisterHandler(uri, std::bind(func, owner, std::placeholders::_1, std::placeholders::_2,\
		std::placeholders::_3))
#endif

//------------------------------------------------------------------------
// 在目标处注册回调
//------------------------------------------------------------------------
#ifndef REG_CALLBACK_TO
#define REG_CALLBACK_TO(registry, func, owner, uri) \
	registry->RegisterHandler(uri, std::bind(func, owner, std::placeholders::_1, std::placeholders::_2,\
		std::placeholders::_3))
#endif

//------------------------------------------------------------------------
// 注册默认回调
//------------------------------------------------------------------------
#ifndef REG_DCALLBACK
#define REG_DCALLBACK(func, owner) \
	RegisterDefaultHandler(std::bind(func, owner, std::placeholders::_1, std::placeholders::_2,\
		std::placeholders::_3))
#endif

//------------------------------------------------------------------------
// 在目标处注册默认回调
//------------------------------------------------------------------------
#ifndef REG_DCALLBACK_TO
#define REG_DCALLBACK_TO(registry, func, owner) \
	registry->RegisterDefaultHandler(std::bind(func, owner, std::placeholders::_1, std::placeholders::_2,\
		std::placeholders::_3))
#endif
