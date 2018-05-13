//------------------------------------------------------------------------
// * @filename: MongoAccess.h
// *
// * @brief: Mongo使用实例,代码参考 mongo-cxx-driver/examples/mongocxx/instance_management.cpp
// *
// * @author: XGM
// * @date: 2017/08/18
//------------------------------------------------------------------------
#pragma once

#include "xbase/Singleton.h"

#include <bsoncxx/stdx/optional.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

namespace mongocxx {
	namespace v_noabi {
		class instance;
	}
}

class MongoAccess : public Singleton<MongoAccess>
{
protected:
	friend class Singleton<MongoAccess>;
	MongoAccess();
	virtual ~MongoAccess();
public:
	using Connection = mongocxx::pool::entry;
public:
	//------------------------------------------------------------------------
	// 配置
	//------------------------------------------------------------------------
	void Configure(std::unique_ptr<mongocxx::instance> pInstance,
		std::unique_ptr<mongocxx::pool> pPool);

	//------------------------------------------------------------------------
	// 取得一个连接 没有可用连接时会阻塞
	//------------------------------------------------------------------------
	Connection GetConnection();

	//------------------------------------------------------------------------
	// 取得一个连接 失败返回: stdx::nullopt
	//------------------------------------------------------------------------
	mongocxx::stdx::optional<Connection> TryGetConnection();

private:
	// MongoDB driver
	std::unique_ptr<mongocxx::instance> m_pInstance = nullptr;
	// 连接池
	std::unique_ptr<mongocxx::pool> m_pPool = nullptr;
};
