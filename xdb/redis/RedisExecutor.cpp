
#include "RedisExecutor.h"
#include "RedisAccess.h"

#ifdef OPEN_REDIS_CLIENT

RedisExecutor::RedisExecutor()
{

}

RedisExecutor::~RedisExecutor()
{

}

cpp_redis::client* RedisExecutor::operator->()
{
	return RedisAccess::Me()->GetAccessor();
}

#endif
