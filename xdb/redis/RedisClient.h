//------------------------------------------------------------------------
// * @filename: RedisClient.h
// *
// * @brief: Redis客户端
// *
// * @author: XGM
// * @date: 2018/06/04
//------------------------------------------------------------------------
#pragma once

#include "RedisMacro.h"

#ifdef OPEN_REDIS_CLIENT

#include "xbase/Singleton.h"
#include "xdb/DBClient.h"

class RedisClient : public DBClient, public Singleton<RedisClient>
{
protected:
	friend class Singleton<RedisClient>;
	RedisClient();
	virtual ~RedisClient();
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
};

#endif
