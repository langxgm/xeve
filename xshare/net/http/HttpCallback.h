//------------------------------------------------------------------------
// * @filename: HttpCallback.h
// *
// * @brief: HTTP回调函数
// *
// * @author: XGM
// * @date: 2017/11/23
//------------------------------------------------------------------------
#pragma once

#include <memory>
#include <functional>

namespace evpp {
	class EventLoop;
	namespace http {
		class Server;
		struct Context;
		typedef std::shared_ptr<Context> ContextPtr;
		typedef std::function<void(const std::string& response_data)> HTTPSendResponseCallback;
		typedef std::function<void(EventLoop* loop, const ContextPtr& ctx,
			const HTTPSendResponseCallback& respcb)> HTTPRequestCallback;
	}
}
