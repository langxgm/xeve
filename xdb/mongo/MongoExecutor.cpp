
#include "MongoExecutor.h"
#include "MongoClient.h"

#include <mongocxx/client.hpp>

#include <assert.h>

MongoExecutor::MongoExecutor()
{
	auto pClient = MongoClient::Me();
	assert(pClient);
	m_strDBName = pClient->GetDefaultDBName();

	auto pAccess = MongoAccess::Me();
	assert(pAccess);
	m_pConnection = pAccess->GetConnection();
}

MongoExecutor::MongoExecutor(const std::string& strDBName)
	: m_strDBName(strDBName)
{
	auto pAccess = MongoAccess::Me();
	assert(pAccess);
	m_pConnection = pAccess->GetConnection();
}

MongoExecutor::~MongoExecutor()
{
	m_pConnection.reset();
}

mongocxx::collection MongoExecutor::GetCollection(mongocxx::stdx::string_view strCollName)
{
	auto db = m_pConnection->database(m_strDBName);
	return db[strCollName];
}

mongocxx::stdx::optional<mongocxx::result::insert_one> MongoExecutor::InsertOne(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aDocument,
	const mongocxx::options::insert& aOptions /*= mongocxx::options::insert()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.insert_one(aDocument, aOptions);
}

mongocxx::stdx::optional<mongocxx::result::delete_result> MongoExecutor::DeleteOne(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	const mongocxx::options::delete_options& aOptions /*= mongocxx::options::delete_options()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.delete_one(aFilter, aOptions);
}

mongocxx::stdx::optional<mongocxx::result::delete_result> MongoExecutor::DeleteMany(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	const mongocxx::options::delete_options& aOptions /*= mongocxx::options::delete_options()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.delete_many(aFilter, aOptions);
}

mongocxx::stdx::optional<mongocxx::result::update> MongoExecutor::UpdateOne(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	bsoncxx::document::view_or_value aUpdate,
	const mongocxx::options::update& aOptions /*= mongocxx::options::update()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.update_one(aFilter, aUpdate, aOptions);
}

mongocxx::stdx::optional<mongocxx::result::update> MongoExecutor::UpdateMany(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	bsoncxx::document::view_or_value aUpdate,
	const mongocxx::options::update& aOptions /*= mongocxx::options::update()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.update_many(aFilter, aUpdate, aOptions);
}

mongocxx::stdx::optional<mongocxx::result::replace_one> MongoExecutor::ReplaceOne(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	bsoncxx::document::view_or_value aReplacement,
	const mongocxx::options::update& aOptions /*= mongocxx::options::update()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.replace_one(aFilter, aReplacement, aOptions);
}

mongocxx::cursor MongoExecutor::Find(mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	const mongocxx::options::find& aOptions /*= mongocxx::options::find()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.find(aFilter, aOptions);
}

mongocxx::stdx::optional<bsoncxx::document::value> MongoExecutor::FindOne(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	const mongocxx::options::find& aOptions /*= mongocxx::options::find()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.find_one(aFilter, aOptions);
}

mongocxx::stdx::optional<bsoncxx::document::value> MongoExecutor::FindOneAndDelete(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	const mongocxx::options::find_one_and_delete& aOptions /*= mongocxx::options::find_one_and_delete()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.find_one_and_delete(aFilter, aOptions);
}

mongocxx::stdx::optional<bsoncxx::document::value> MongoExecutor::FindOneAndReplace(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	bsoncxx::document::view_or_value aReplacement,
	const mongocxx::options::find_one_and_replace& aOptions /*= mongocxx::options::find_one_and_replace()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.find_one_and_replace(aFilter, aReplacement, aOptions);
}

mongocxx::stdx::optional<bsoncxx::document::value> MongoExecutor::FindOneAndUpdate(
	mongocxx::stdx::string_view strCollName,
	bsoncxx::document::view_or_value aFilter,
	bsoncxx::document::view_or_value aUpdate,
	const mongocxx::options::find_one_and_update& aOptions /*= mongocxx::options::find_one_and_update()*/)
{
	auto db = m_pConnection->database(m_strDBName);
	auto coll = db[strCollName];
	return coll.find_one_and_update(aFilter, aUpdate, aOptions);
}
