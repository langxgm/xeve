//------------------------------------------------------------------------
// * @filename: PlaySystem.h
// *
// * @brief: 游戏玩法系统
// *
// * @author: XGM
// * @date: 2018/06/07
//------------------------------------------------------------------------
#pragma once

#include <map>
#include <memory>

template<typename gameplay_key_type, typename gameplay_value_type>
class PlaySystem
{
public:
	typedef gameplay_key_type key_type;
	typedef gameplay_value_type value_type;
	typedef typename std::add_pointer<value_type>::type value_ptr_type;
	typedef typename std::remove_pointer<value_type>::type value_nonptr_type;
	typedef std::shared_ptr<value_nonptr_type> value_sptr_type;
	typedef std::map<key_type, value_sptr_type> map_type;
public:
	PlaySystem() {}
	virtual ~PlaySystem() {}

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init() {}

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update() = 0;

	//------------------------------------------------------------------------
	// 注册游戏玩法类型
	//------------------------------------------------------------------------
	virtual bool RegGamePlay(const key_type& key, const value_sptr_type& value)
	{
		auto ret = m_mapGamePlay.insert(std::make_pair(key, value));
		return ret.second;
	}

	//------------------------------------------------------------------------
	// 移除注册
	//------------------------------------------------------------------------
	virtual bool UnregGamePlay(const key_type& key)
	{
		auto it = m_mapGamePlay.find(key);
		if (it != m_mapGamePlay.end())
		{
			m_mapGamePlay.erase(it);
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	// 获得游戏玩法
	//------------------------------------------------------------------------
	virtual value_sptr_type GetGamePlay(const key_type& key)
	{
		auto it = m_mapGamePlay.find(key);
		if (it != m_mapGamePlay.end())
		{
			return it->second;
		}
		return value_sptr_type();
	}

protected:
	// 所有游戏玩法
	map_type m_mapGamePlay;
};
