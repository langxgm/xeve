//------------------------------------------------------------------------
// * @filename: MongoExecutor.h
// *
// * @brief: Mongo命令执行者
// *
// * @author: XGM
// * @date: 2017/08/23
//------------------------------------------------------------------------
#pragma once

#include "MongoAccess.h"

#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/collection.hpp>

#include <string>

class MongoExecutor
{
public:
	MongoExecutor();
	explicit MongoExecutor(const std::string& strDBName);
	virtual ~MongoExecutor();

public:
	//------------------------------------------------------------------------
	// 获得集合
	//------------------------------------------------------------------------
	virtual mongocxx::collection GetCollection(mongocxx::stdx::string_view strCollName);

	//------------------------------------------------------------------------
	// 增加一个值
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<mongocxx::result::insert_one> InsertOne(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aDocument,
		const mongocxx::options::insert& aOptions = mongocxx::options::insert());

	//------------------------------------------------------------------------
	// 删除一个值
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<mongocxx::result::delete_result> DeleteOne(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		const mongocxx::options::delete_options& aOptions = mongocxx::options::delete_options());

	//------------------------------------------------------------------------
	// 删除多个值
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<mongocxx::result::delete_result> DeleteMany(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		const mongocxx::options::delete_options& aOptions = mongocxx::options::delete_options());

	//------------------------------------------------------------------------
	// 更新一个值
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<mongocxx::result::update> UpdateOne(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		bsoncxx::document::view_or_value aUpdate,
		const mongocxx::options::update& aOptions = mongocxx::options::update());

	//------------------------------------------------------------------------
	// 更新多个值
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<mongocxx::result::update> UpdateMany(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		bsoncxx::document::view_or_value aUpdate,
		const mongocxx::options::update& aOptions = mongocxx::options::update());

	//------------------------------------------------------------------------
	// 替换一个值
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<mongocxx::result::replace_one> ReplaceOne(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		bsoncxx::document::view_or_value aReplacement,
		const mongocxx::options::update& aOptions = mongocxx::options::update());

	//------------------------------------------------------------------------
	// 查询多个值
	//------------------------------------------------------------------------
	virtual mongocxx::cursor Find(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		const mongocxx::options::find& aOptions = mongocxx::options::find());

	//------------------------------------------------------------------------
	// 查询并获得第一个值 (无效返回值:mongocxx::stdx::nullopt)
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<bsoncxx::document::value> FindOne(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		const mongocxx::options::find& aOptions = mongocxx::options::find());

	//------------------------------------------------------------------------
	// 查找并且删除
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<bsoncxx::document::value> FindOneAndDelete(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		const mongocxx::options::find_one_and_delete& aOptions = mongocxx::options::find_one_and_delete());

	//------------------------------------------------------------------------
	// 查找并且替换
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<bsoncxx::document::value> FindOneAndReplace(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		bsoncxx::document::view_or_value aReplacement,
		const mongocxx::options::find_one_and_replace& aOptions = mongocxx::options::find_one_and_replace());

	//------------------------------------------------------------------------
	// 查找并且更新
	//------------------------------------------------------------------------
	virtual mongocxx::stdx::optional<bsoncxx::document::value> FindOneAndUpdate(
		mongocxx::stdx::string_view strCollName,
		bsoncxx::document::view_or_value aFilter,
		bsoncxx::document::view_or_value aUpdate,
		const mongocxx::options::find_one_and_update& aOptions = mongocxx::options::find_one_and_update());

private:
	// 数据库名称
	std::string m_strDBName;
	// 数据库连接
	MongoAccess::Connection m_pConnection = nullptr;
};

using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;
