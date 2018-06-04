
#include "RedisClient.h"
#include "RedisAccess.h"

#ifdef OPEN_REDIS_CLIENT

#include <cpp_redis/cpp_redis>

#include <iostream>

RedisClient::RedisClient()
{
	RedisAccess::InitInstance();
}

RedisClient::~RedisClient()
{
	RedisAccess::DestroyInstance();
}

bool RedisClient::Init(std::string strHost, std::string strUserName, std::string strPassword)
{
	cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);
	RedisAccess::Me()->Configure(strHost, strUserName, strPassword);
	return true;
}

bool RedisClient::Start()
{
	std::cout << "connect redis. . ." << std::endl;
	return RedisAccess::Me()->Connect();
}

bool RedisClient::Stop()
{
	return true;
}

#endif
