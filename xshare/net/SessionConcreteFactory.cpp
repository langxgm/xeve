
#include "SessionConcreteFactory.h"
#include "Session.h"
#include "SessionStatistics.h"
#include "SessionManager.h"

#include <evpp/tcp_conn.h>

Session* SessionDefaultFactory::NewSession(evpp::TCPConnPtr conn)
{
	return new Session(conn);
}

SessionStatistics* SessionDefaultFactory::NewSessionStatistics()
{
	return new SessionStatistics();
}

SessionManager* SessionDefaultFactory::NewSessionManager()
{
	return new SessionManager();
}
