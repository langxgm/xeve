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

#ifdef ERROR
#undef ERROR // 跟glog冲突
#endif

class RedisExecutor
{
public:
	RedisExecutor();
	virtual ~RedisExecutor();
public:
	//------------------------------------------------------------------------
	// 使用访问者
	//------------------------------------------------------------------------
	cpp_redis::client* operator->();
};

#endif
