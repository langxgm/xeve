//------------------------------------------------------------------------
// * @filename: EntityManager.h
// *
// * @brief: 游戏实体管理
// *
// * @author: XGM
// * @date: 2017/01/24
//------------------------------------------------------------------------
#pragma once

#include "GameType.h"

#include <map>

class GameObject_Entity;

class EntityManager
{
public:
	typedef std::map<ObjectID_t, GameObject_Entity*> EntityMap_t;
public:
	EntityManager();
	~EntityManager();

	void CleanUp();

	//------------------------------------------------------------------------
	// 存储对象
	//------------------------------------------------------------------------
	bool RegisterEntity(GameObject_Entity* pEntity);

	//------------------------------------------------------------------------
	// 销毁对象
	//------------------------------------------------------------------------
	bool UnregisterEntity(ObjectID_t nObjectID);

	//------------------------------------------------------------------------
	// 获得对象
	//------------------------------------------------------------------------
	GameObject_Entity* GetEntityByID(ObjectID_t nObjectID);

	//------------------------------------------------------------------------
	// 获得列表
	//------------------------------------------------------------------------
	EntityMap_t& GetEntities() { return m_EntityMap; }

private:
	// 存储对象,key=nID
	EntityMap_t m_EntityMap;
};
