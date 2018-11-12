//------------------------------------------------------------------------
// * @filename: RedisSubscriber.h
// *
// * @brief: Redis订阅
// *
// * @author: XGM
// * @date: 2018/09/06
//------------------------------------------------------------------------
#pragma once

#include "RedisMacro.h"

#ifdef OPEN_REDIS_CLIENT

#include "xbase/Singleton.h"

#include <string>
#include <memory>

namespace cpp_redis {
	class subscriber;
}

class RedisSubscriber : public Singleton<RedisSubscriber>
{
protected:
	friend class Singleton<RedisSubscriber>;
	RedisSubscriber();
	virtual ~RedisSubscriber();
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
	// 获得订阅者
	//------------------------------------------------------------------------
	cpp_redis::subscriber* GetSubscriber();

protected:
	// redis订阅者
	std::unique_ptr<cpp_redis::subscriber> m_pSubscriber;
	// 目标地址
	std::string m_strHost;
};

#endif
