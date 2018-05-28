//------------------------------------------------------------------------
// * @filename: EntityManager.h
// *
// * @brief: 游戏实体管理
// *
// * @author: XGM
// * @date: 2017/01/24
//------------------------------------------------------------------------
#pragma once

#include "xbase/Singleton.h"
#include "GameType.h"

#include <map>

class GameObject_Entity;

class EntityManager : public Singleton<EntityManager>
{
public:
	typedef std::map<ObjID_t, GameObject_Entity*> EntityMap_t;
protected:
	friend class Singleton<EntityManager>;
	EntityManager()
	{
	}
	~EntityManager();

public:
	//------------------------------------------------------------------------
	// 存储对象
	//------------------------------------------------------------------------
	void RegisterEntity(GameObject_Entity* pEntity);

	//------------------------------------------------------------------------
	// 获得对象
	//------------------------------------------------------------------------
	GameObject_Entity* GetEntityByID(ObjID_t nID);

private:
	// 存储对象,key=nID
	EntityMap_t m_EntityMap;
};
