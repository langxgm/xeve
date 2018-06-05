//------------------------------------------------------------------------
// * @filename: RedisAccess.h
// *
// * @brief: Redis的访问
// *
// * @author: XGM
// * @date: 2018/06/04
//------------------------------------------------------------------------
#pragma once

#include "RedisMacro.h"

#ifdef OPEN_REDIS_CLIENT

#include "xbase/Singleton.h"

#include <string>
#include <memory>

namespace cpp_redis {
	class client;
}

class RedisAccess : public Singleton<RedisAccess>
{
protected:
	friend class Singleton<RedisAccess>;
	RedisAccess();
	virtual ~RedisAccess();
public:
	//------------------------------------------------------------------------
	// 配置
	//------------------------------------------------------------------------
	void Configure(std::string strHost, std::string strUserName, std::string strPassword);

	//------------------------------------------------------------------------
	// 连接
	//------------------------------------------------------------------------
	bool Connect();

	//------------------------------------------------------------------------
	// 断开
	//------------------------------------------------------------------------
	void Disconnect();

public:
	//------------------------------------------------------------------------
	// 获得访问者
	//------------------------------------------------------------------------
	cpp_redis::client* GetAccessor();

private:
	// redis客户端
	std::unique_ptr<cpp_redis::client> m_pClient;
	// 目标地址
	std::string m_strHost;
};

#endif
