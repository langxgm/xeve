//------------------------------------------------------------------------
// * @filename: StateManager.h
// *
// * @brief: 状态管理器
// *
// * @author: XGM
// * @date: 2018/05/31
//------------------------------------------------------------------------
#pragma once

#include "xbase/Manager.h"
#include "State.h"

#include <stdint.h>

template<class k_type, class v_type>
class StateManager : protected Manager<k_type, v_type>
{
public:
	typedef Manager<k_type, v_type> mybase_manager;
	typedef typename mybase_manager::key_type key_type;
	typedef typename mybase_manager::value_type value_type;
public:
	//------------------------------------------------------------------------
	// 构造
	//------------------------------------------------------------------------
	StateManager() {}

	//------------------------------------------------------------------------
	// 析构
	//------------------------------------------------------------------------
	virtual ~StateManager() {}

public:
	//------------------------------------------------------------------------
	// 注册
	//------------------------------------------------------------------------
	virtual bool RegState(const key_type& key, const value_type& value)
	{
		return mybase_manager::RegObject(key, value);
	}

	//------------------------------------------------------------------------
	// 移除注册
	//------------------------------------------------------------------------
	virtual bool UnregState(const key_type& key)
	{
		return mybase_manager::UnregObject(key);
	}

	//------------------------------------------------------------------------
	// 获取
	//------------------------------------------------------------------------
	virtual value_type GetState(const key_type& key)
	{
		return mybase_manager::GetObject(key);
	}
};
