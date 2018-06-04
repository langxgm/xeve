
#include "test_common.h"

#include "xdb/redis/RedisClient.h"

#include <glog/logging.h>

#include <memory>

#ifdef OPEN_REDIS_CLIENT
//------------------------------------------------------------------------
// 测试Redis连接
//------------------------------------------------------------------------
TEST_UNIT(TestRedisConnect)
{
	RedisClient::InitInstance();
	RedisClient::Me()->Init("127.0.0.1:6379", "", "");
	H_EXPECT_TRUE(RedisClient::Me()->Start());
	RedisClient::Me()->Stop();
	RedisClient::DestroyInstance();
}
#endif
