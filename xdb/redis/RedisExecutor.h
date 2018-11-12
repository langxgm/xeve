//------------------------------------------------------------------------
// * @filename: RedisExecutor.h
// *
// * @brief: Redis命令执行者
// *
// * @author: XGM
// * @date: 2018/06/04
//------------------------------------------------------------------------
#pragma once

#include "RedisMacro.h"

#ifdef OPEN_REDIS_CLIENT

#include <cpp_redis/cpp_redis>

#include <mutex>

#ifdef ERROR
#undef ERROR // 跟glog冲突
#endif

class RedisExecutor
{
public:
	RedisExecutor();
	explicit RedisExecutor(bool bUnique);
	virtual ~RedisExecutor();
public:
	//------------------------------------------------------------------------
	// 使用访问者
	//------------------------------------------------------------------------
	cpp_redis::client* operator->();

	//------------------------------------------------------------------------
	// 获得订阅者
	//------------------------------------------------------------------------
	cpp_redis::subscriber* GetSubscriber();

protected:
	std::unique_lock<std::mutex> m_UniqueLock;
};

#endif
