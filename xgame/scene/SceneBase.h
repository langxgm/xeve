//------------------------------------------------------------------------
// * @filename: SceneBase.h
// *
// * @brief: 场景
// *
// * @author: XGM
// * @date: 2020/09/14
//------------------------------------------------------------------------
#pragma once

#include "xgame/object/GameType.h"

class GameObject;

struct GAME_SCENE_INIT
{
	virtual void CleanUp() {}
};

class SceneBase
{
public:
	SceneBase(const SceneBase&) = delete;
	SceneBase& operator=(const SceneBase&) = delete;
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	virtual int32_t GetSceneType() const = 0;

	virtual SceneBase* Clone() = 0;

	virtual void CopyTo(SceneBase& rhs) {
		rhs.m_nSceneID = m_nSceneID;
		rhs.m_nObjectNumber = m_nObjectNumber;
	}

	virtual void CleanUp() {
		m_nSceneID = 0;
		m_nObjectNumber = 0;
	}

	virtual bool Init(const GAME_SCENE_INIT* pInit) { return true; }

	virtual void Tick(int64_t nTime) = 0;

public:
	virtual bool ObjectEnter(GameObject* pObject) { return false; }
	virtual void ObjectLeave(GameObject* pObject) {}

public:
	int32_t GetSceneID() const { return m_nSceneID; }
	void SetSceneID(int32_t nSceneID) { m_nSceneID = nSceneID; }

	virtual ObjectID_t NextObjectID() { return m_nObjectNumber++; }
	virtual GameObject* NewObject(int32_t nObjectType) { return nullptr; }
	virtual void RecycleObject(GameObject* pObject) {}
	virtual bool RemoveObjectByID(ObjectID_t nObjectID) { return false; }
	virtual GameObject* GetObject(ObjectID_t nObjectID) { return nullptr; }

protected:
	// 场景ID
	int32_t m_nSceneID = 0;
	// 自增的ObjectID
	ObjectID_t m_nObjectNumber = 0;
};
