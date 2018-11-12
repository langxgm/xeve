
#include "RedisClient.h"
#include "RedisAccess.h"
#include "RedisSubscriber.h"

#ifdef OPEN_REDIS_CLIENT

#include <cpp_redis/cpp_redis>

#include <iostream>

RedisClient::RedisClient()
{
	RedisAccess::InitInstance();
	RedisSubscriber::InitInstance();
}

RedisClient::~RedisClient()
{
	RedisSubscriber::DestroyInstance();
	RedisAccess::DestroyInstance();
}

bool RedisClient::Init(std::string strHost, std::string strUserName, std::string strPassword)
{
	cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);
	RedisAccess::Me()->Configure(strHost, strUserName, strPassword);
	RedisSubscriber::Me()->Configure(strHost, strUserName, strPassword);
	return true;
}

bool RedisClient::Start()
{
	bool bRet = true;

	if (bRet && (m_nFlag & init_client))
	{
		std::cout << "connect redis. . ." << std::endl;
		bRet &= RedisAccess::Me()->Connect();
	}

	if (bRet && (m_nFlag & init_subscriber))
	{
		std::cout << " connect redis subscriber. . ." << std::endl;
		bRet &= RedisSubscriber::Me()->Connect();
	}
	return bRet;
}

bool RedisClient::Stop()
{
	RedisSubscriber::Me()->Disconnect();
	RedisAccess::Me()->Disconnect();
	return true;
}

void RedisClient::SetInitFlag(int32_t nFlag)
{
	m_nFlag = nFlag;
}

#endif
