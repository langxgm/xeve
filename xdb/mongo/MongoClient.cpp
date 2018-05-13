
#include "MongoClient.h"
#include "MongoAccess.h"

#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>

#include <iostream>

class noop_logger : public mongocxx::logger
{
public:
	virtual void operator()(mongocxx::log_level, mongocxx::stdx::string_view,
		mongocxx::stdx::string_view) noexcept
	{
	}
};

MongoClient::MongoClient()
{
	MongoAccess::InitInstance();
}

MongoClient::~MongoClient()
{
	MongoAccess::DestroyInstance();
}

bool MongoClient::Init(std::string strHost, std::string strUserName, std::string strPassword)
{
	//auto uri = mongocxx::uri{ "mongodb://127.0.0.1:27017" };
	auto uri = mongocxx::uri{ strHost };

	auto instance = mongocxx::stdx::make_unique<mongocxx::instance>(mongocxx::stdx::make_unique<noop_logger>());
	auto pool = mongocxx::stdx::make_unique<mongocxx::pool>(std::move(uri));

	MongoAccess::Me()->Configure(std::move(instance), std::move(pool));
	return true;
}

bool MongoClient::Start()
{
	auto conn = MongoAccess::Me()->GetConnection();
	auto admin = (*conn)["admin"];

	bsoncxx::builder::stream::document ismaster;
	ismaster << "isMaster" << 1;

	try
	{
		std::cout << "connect mongodb. . ." << std::endl;
		auto result = admin.run_command(ismaster.view());
		std::cout << bsoncxx::to_json(result) << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool MongoClient::Stop()
{
	return true;
}
