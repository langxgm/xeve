
#include "EntityManager.h"
#include "GameObject_Entity.h"

#include <assert.h>

EntityManager::~EntityManager()
{
	for (auto v : m_EntityMap)
	{
		delete (v.second);
	}
	m_EntityMap.clear();
}

void EntityManager::RegisterEntity(GameObject_Entity* pEntity)
{
	assert(pEntity);
	auto it = m_EntityMap.find(pEntity->GetObjID());
	if (it != m_EntityMap.end())
	{
		assert(false && "[EntityManager::RegisterEntity] object id exist");
	}
	m_EntityMap.insert(std::make_pair(pEntity->GetObjID(), pEntity));
}

GameObject_Entity* EntityManager::GetEntityByID(ObjID_t nID)
{
	auto it = m_EntityMap.find(nID);
	if (it != m_EntityMap.end())
	{
		return it->second;
	}
	return nullptr;
}
