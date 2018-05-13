
#include "MongoAccess.h"

MongoAccess::MongoAccess()
{

}

MongoAccess::~MongoAccess()
{
	m_pPool.reset();
	m_pInstance.reset();
}

void MongoAccess::Configure(std::unique_ptr<mongocxx::instance> pInstance,
	std::unique_ptr<mongocxx::pool> pPool)
{
	m_pInstance = std::move(pInstance);
	m_pPool = std::move(pPool);
}

MongoAccess::Connection MongoAccess::GetConnection()
{
	return m_pPool->acquire();
}

mongocxx::stdx::optional<MongoAccess::Connection> MongoAccess::TryGetConnection()
{
	return m_pPool->try_acquire();
}
