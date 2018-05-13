
#include "test_common.h"

#include "xdb/mongo/MongoClient.h"

#include <glog/logging.h>

#include <memory>

//------------------------------------------------------------------------
// 测试MongoDB连接
//------------------------------------------------------------------------
TEST_UNIT(TestMongoConnect)
{
	MongoClient::InitInstance();
	MongoClient::Me()->Init("mongodb://127.0.0.1:27017", "", "");
	H_EXPECT_TRUE(MongoClient::Me()->Start());
	MongoClient::Me()->Stop();
	MongoClient::DestroyInstance();
}
