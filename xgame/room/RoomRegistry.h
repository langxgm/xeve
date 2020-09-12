//------------------------------------------------------------------------
// * @filename: RoomRegistry.h
// *
// * @brief: 房间容器注册表
// *
// * @author: XGM
// * @date: 2020/09/10
//------------------------------------------------------------------------
#pragma once

#include <map>
#include <string>
#include <memory>

template<typename key, typename value>
class RoomRegistry
{
public:
	typedef key key_type;
	typedef value value_type;
	typedef typename std::add_pointer<value_type>::type value_ptr_type;
	typedef typename std::remove_pointer<value_type>::type value_nonptr_type;
	typedef std::map<key_type, value_ptr_type> map_type;
public:
	RoomRegistry() {}
	virtual ~RoomRegistry()
	{
		for (auto it : m_mapContainer)
		{
			value_ptr_type ptr = it.second;
			delete ptr;
		}
		m_mapContainer.clear();
	}

	//------------------------------------------------------------------------
	// 注册表名称
	//------------------------------------------------------------------------
	virtual const std::string& GetName() const = 0;

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init() {}

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update() = 0;

	//------------------------------------------------------------------------
	// 注册容器
	//------------------------------------------------------------------------
	virtual bool RegisterContainer(const key_type& key, value_ptr_type value)
	{
		auto ret = m_mapContainer.insert(std::make_pair(key, value));
		return ret.second;
	}

	//------------------------------------------------------------------------
	// 注销容器
	//------------------------------------------------------------------------
	virtual bool UnregisterContainer(const key_type& key)
	{
		auto it = m_mapContainer.find(key);
		if (it != m_mapContainer.end())
		{
			delete it->second;
			m_mapContainer.erase(it);
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	// 获得容器
	//------------------------------------------------------------------------
	virtual value_ptr_type GetContainer(const key_type& key)
	{
		auto it = m_mapContainer.find(key);
		if (it != m_mapContainer.end())
		{
			return it->second;
		}
		return value_ptr_type();
	}

protected:
	// 容器表
	map_type m_mapContainer;
};
