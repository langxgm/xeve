//------------------------------------------------------------------------
// * @filename: MongoClient.h
// *
// * @brief: Mongo客户端
// *
// * @author: XGM
// * @date: 2017/08/16
//------------------------------------------------------------------------
#pragma once

#include "xbase/Singleton.h"
#include "xdb/DBClient.h"

class MongoClient : public DBClient, public Singleton<MongoClient>
{
protected:
	friend class Singleton<MongoClient>;
	MongoClient();
	virtual ~MongoClient();
public:
	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual bool Init(std::string strHost,
		std::string strUserName,
		std::string strPassword) override;

	//------------------------------------------------------------------------
	// 启动服务
	//------------------------------------------------------------------------
	virtual bool Start() override;

	//------------------------------------------------------------------------
	// 停止服务
	//------------------------------------------------------------------------
	virtual bool Stop() override;

	//------------------------------------------------------------------------
	// 默认操作的DB名称
	//------------------------------------------------------------------------
	const std::string& GetDefaultDBName() const { return m_strDefaultDBName; }
	void SetDefaultDBName(const std::string& name) { m_strDefaultDBName = name; }

private:
	// 默认操作的DB名称
	std::string m_strDefaultDBName;
};
