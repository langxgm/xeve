
#include "RedisExecutor.h"
#include "RedisAccess.h"
#include "RedisSubscriber.h"

#ifdef OPEN_REDIS_CLIENT

std::mutex s_redis_executor_lock;

RedisExecutor::RedisExecutor()
{

}

RedisExecutor::RedisExecutor(bool bUnique)
{
	if (bUnique)
	{
		m_UniqueLock = std::unique_lock<std::mutex>(s_redis_executor_lock);
	}
}

RedisExecutor::~RedisExecutor()
{

}

cpp_redis::client* RedisExecutor::operator->()
{
	return RedisAccess::Me()->GetAccessor();
}

cpp_redis::subscriber* RedisExecutor::GetSubscriber()
{
	return RedisSubscriber::Me()->GetSubscriber();
}

#endif
