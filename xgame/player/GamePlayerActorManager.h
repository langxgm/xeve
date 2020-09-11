//------------------------------------------------------------------------
// * @filename: GamePlayerActorManager.h
// *
// * @brief: 玩家扮演的角色管理
// *
// * @author: XGM
// * @date: 2018/05/18
//------------------------------------------------------------------------
#pragma once

#include "GamePlayerActor.h"

#include "xbase/Manager.h"

class GamePlayerActorManager : protected Manager<int32_t, GamePlayerActor*>
{
public:
	typedef Manager<int32_t, GamePlayerActor*> mybase_manager;
	typedef typename mybase_manager::key_type key_type;
	typedef typename mybase_manager::value_type value_type;
public:
	//------------------------------------------------------------------------
	// 构造
	//------------------------------------------------------------------------
	GamePlayerActorManager() {}

	//------------------------------------------------------------------------
	// 析构
	//------------------------------------------------------------------------
	virtual ~GamePlayerActorManager() {}

public:
	//------------------------------------------------------------------------
	// 注册
	//------------------------------------------------------------------------
	virtual bool RegActor(const key_type& key, const value_type& value)
	{
		return mybase_manager::RegObject(key, value);
	}

	//------------------------------------------------------------------------
	// 获取
	//------------------------------------------------------------------------
	virtual value_type GetActor(const key_type& key)
	{
		return mybase_manager::GetObject(key);
	}

	template<class T>
	T* GetActor_cast(const key_type& key)
	{
		return static_cast<T*>(GetActor(key));
	}
};
