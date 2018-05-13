//------------------------------------------------------------------------
// * @filename: SessionFactory.h
// *
// * @brief: 创建连接的抽象工厂
// *
// * @author: XGM
// * @date: 2018/02/27
//------------------------------------------------------------------------
#pragma once

#include <memory>

namespace evpp {
	class TCPConn;
	typedef std::shared_ptr<TCPConn> TCPConnPtr;
}

class Session;
class SessionStatistics;
class SessionManager;

class SessionFactory
{
public:
	virtual Session* NewSession(evpp::TCPConnPtr conn) = 0;
	virtual SessionStatistics* NewSessionStatistics() = 0;
	virtual SessionManager* NewSessionManager() = 0;
};
