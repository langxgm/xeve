
#include "EntityManager.h"
#include "GameObject_Entity.h"

#include <assert.h>

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{
	CleanUp();
}

void EntityManager::CleanUp()
{
	for (auto v : m_EntityMap)
	{
		delete (v.second);
	}
	m_EntityMap.clear();
}

bool EntityManager::RegisterEntity(GameObject_Entity* pEntity)
{
	assert(pEntity);
	auto ret = m_EntityMap.insert(std::make_pair(pEntity->GetID(), pEntity));
	assert(ret.second && "object exist");
	return ret.second;
}

bool EntityManager::UnregisterEntity(ObjectID_t nObjectID)
{
	auto it = m_EntityMap.find(nObjectID);
	if (it != m_EntityMap.end())
	{
		delete it->second;
		m_EntityMap.erase(it);
		return true;
	}
	return false;
}

GameObject_Entity* EntityManager::GetEntityByID(ObjectID_t nObjectID)
{
	auto it = m_EntityMap.find(nObjectID);
	if (it != m_EntityMap.end())
	{
		return it->second;
	}
	return nullptr;
}
