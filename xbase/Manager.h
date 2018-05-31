//------------------------------------------------------------------------
// * @filename: Manager.h
// *
// * @brief: 对象管理器模板
// *
// * @author: XGM
// * @date: 2017/03/08
//------------------------------------------------------------------------
#pragma once

#include "Print.h"

#include <map>
#include <ostream>

template<class _Key_t, class _Value_t, class _Output_t = std::ostream>
class Manager : public Print<_Output_t>
{
public:
	typedef _Key_t key_type;
	typedef _Value_t value_type;
	typedef _Output_t output_type;
	typedef Print<output_type> myprint_type;
	typedef std::map<key_type, value_type> map_type;
	typedef typename map_type::iterator map_iterator;
	typedef typename map_type::const_iterator map_const_iterator;
	static_assert(std::is_pointer<value_type>::value, "value_type not pointer");
public:
	//------------------------------------------------------------------------
	// 构造
	//------------------------------------------------------------------------
	Manager()
	{
	}

	//------------------------------------------------------------------------
	// 析构
	//------------------------------------------------------------------------
	virtual ~Manager()
	{
		for (auto p : m_mapObject)
		{
			delete p.second;
		}
		m_mapObject.clear();
	}

	//------------------------------------------------------------------------
	// 注册对象
	//------------------------------------------------------------------------
	virtual bool RegObject(const key_type& key, const value_type& value)
	{
		auto ret = m_mapObject.insert(std::make_pair(key, value));
		return ret.second;
	}

	//------------------------------------------------------------------------
	// 移除注册
	//------------------------------------------------------------------------
	virtual bool UnregObject(const key_type& key)
	{
		auto it = m_mapObject.find(key);
		if (it != m_mapObject.end())
		{
			delete it->second;
			m_mapObject.erase(it);
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	// 获取对象
	//------------------------------------------------------------------------
	virtual value_type GetObject(const key_type& key)
	{
		auto it = m_mapObject.find(key);
		if (it != m_mapObject.end())
		{
			return it->second;
		}
		static_assert(value_type() == nullptr, "value_type() != nullptr");
		return value_type();
	}

	//------------------------------------------------------------------------
	// 打印所有对象
	//------------------------------------------------------------------------
	virtual void PrintAll(output_type& out) override
	{
#if (0)
		out << "//------------------------------------------------------------------------" << "\n";
		out << "// Manager:" << "\n";
		for (auto it : m_mapObject)
		{
			out << "// [" << it->first << "]: " << it->second << "\n";
		}
		out << "//------------------------------------------------------------------------" << "\n";
#endif
	}

protected:
	map_type m_mapObject;
};
