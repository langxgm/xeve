//------------------------------------------------------------------------
// * @filename: SessionConcreteFactory.h
// *
// * @brief: 创建连接的具体工厂
// *
// * @author: XGM
// * @date: 2018/02/27
//------------------------------------------------------------------------
#pragma once

#include "SessionFactory.h"

class SessionDefaultFactory : public SessionFactory
{
public:
	virtual Session* NewSession(evpp::TCPConnPtr conn) override;
	virtual SessionStatistics* NewSessionStatistics() override;
	virtual SessionManager* NewSessionManager() override;
};
