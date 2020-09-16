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

struct GAME_OBJECT_INIT
{
	virtual void CleanUp() {}
};

class GameObject
{
public:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
public:
	GameObject() {}
	virtual ~GameObject() {}

	virtual int32_t GetObjectType() const = 0;

	virtual GameObject* Clone() = 0;

	virtual void CopyTo(GameObject& rhs) {
		rhs.m_nObjectID = m_nObjectID;
		rhs.m_bActive = m_bActive;
	}

	virtual void CleanUp() {
		m_nObjectID = INVALID_ID;
		m_bActive = false;
	}

	virtual bool Init(const GAME_OBJECT_INIT* pInit) { return true; }

	virtual void Tick(int64_t nTime, int64_t nElapsed) = 0;

	virtual GameObjectAttribute* GetAttrInterface() { return nullptr; }

public:
	ObjectID_t GetID() const { return m_nObjectID; }
	void SetID(ObjectID_t nID) { m_nObjectID = nID; }

	virtual bool IsActiveObj() const { return m_bActive; }
	virtual void SetActiveFlag(bool bFlag) { m_bActive = bFlag; }

protected:
	ObjectID_t m_nObjectID = INVALID_ID;
	bool m_bActive = false;
};
