//------------------------------------------------------------------------
// * @filename: Object.h
// *
// * @brief: 游戏基础对象
// *
// * @author: XGM
// * @date: 2017/01/10
//------------------------------------------------------------------------
#pragma once

#include "GameType.h"
#include "GameObjectType.h"

class GameObjectAttribute;

class GameObject
{
public:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
public:
	GameObject() {}
	virtual ~GameObject() {}

	ObjID_t GetObjID() const { return m_nObjID; }
	void SetObjID(ObjID_t nID) { m_nObjID = nID; }

	virtual GameObjectType GetType() = 0;

	virtual void HeartBeat(uint32_t nElapsed, uint32_t nTime) = 0;

	//------------------------------------------------------------------------
	// 获取属性
	//------------------------------------------------------------------------
	virtual GameObjectAttribute* GetAttr() = 0;

private:
	// 对象的id
	ObjID_t m_nObjID;
};
