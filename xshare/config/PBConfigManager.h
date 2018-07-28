//------------------------------------------------------------------------
// * @filename: PBConfigManager.h
// *
// * @brief: protobuf配置管理
// *
// * @author: XGM
// * @date: 2017/12/11
//------------------------------------------------------------------------
#pragma once

#include "ProtobufMap.h"

#include "xbase/Manager.h"
#include "xbase/Singleton.h"

#include <map>
#include <string>

namespace google {
	namespace protobuf {
		class Message;
	}
}

class PBConfigManager : protected Manager<size_t, IProtobufMap*>, public Singleton<PBConfigManager>
{
public:
	typedef Manager<std::string, const google::protobuf::Message*> mybase_manager;
	typedef typename mybase_manager::key_type key_type;
	typedef typename mybase_manager::value_type value_type;
	template<class T> using rm_const_ptr_t = typename std::remove_pointer<typename std::remove_const<T>::type>::type;
	template<class T> using add_const_ptr_t = typename std::add_pointer<typename std::add_const<rm_const_ptr_t<T>>::type>::type;
protected:
	friend class Singleton<PBConfigManager>;
	PBConfigManager() {}
	virtual ~PBConfigManager() {}
public:

	//------------------------------------------------------------------------
	// 注册配置
	//------------------------------------------------------------------------
	template<class _Object_t, class... _InitArgs_t>
	bool Init(_InitArgs_t&&... args)
	{
		using pbmap_type = typename std::remove_pointer<typename std::decay<_Object_t>::type>::type;

		auto obj = new pbmap_type();
		if (obj->Init(std::forward<_InitArgs_t>(args)...) == false)
		{
			delete obj;
			return false;
		}

		auto name = typeid(pbmap_type).hash_code();
		auto ret = m_mapObject.insert(std::make_pair(name, obj));
		if (ret.second == false)
		{
			delete obj;
			LOG(ERROR) << "Config repeat, typeinfo=" << typeid(pbmap_type).name(); // 配置重复
		}
		return ret.second;
	}

	//------------------------------------------------------------------------
	// 获取配置
	//------------------------------------------------------------------------
	template<class _Key_t, class _Value_t>
	typename ProtobufMap<_Key_t, add_const_ptr_t<_Value_t>>::value_ptr_type At(const _Key_t& key)
	{
		static_assert(std::is_pointer<_Value_t>::value, "value non pointer type");
		using pbmap_type = ProtobufMap<_Key_t, add_const_ptr_t<_Value_t>>;

		auto name = typeid(pbmap_type).hash_code();
		auto it = m_mapObject.find(name);
		if (it != m_mapObject.end())
		{
#ifdef _DEBUG
			auto rs = dynamic_cast<pbmap_type*>(it->second);
#else
			auto rs = static_cast<pbmap_type*>(it->second);
#endif
			if (rs)
			{
				return rs->Find(key);
			}
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	// 获取ProtobufMap<K,V>对象的指针
	//------------------------------------------------------------------------
	template<class _Key_t, class _Value_t>
	ProtobufMap<_Key_t, add_const_ptr_t<_Value_t>>* ObjectPtr()
	{
		static_assert(std::is_pointer<_Value_t>::value, "value non pointer type");
		using pbmap_type = ProtobufMap<_Key_t, add_const_ptr_t<_Value_t>>;

		auto name = typeid(pbmap_type).hash_code();
		auto it = m_mapObject.find(name);
		if (it != m_mapObject.end())
		{
#ifdef _DEBUG
			return dynamic_cast<pbmap_type*>(it->second);
#else
			return static_cast<pbmap_type*>(it->second);
#endif
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	// 打印所有对象
	//------------------------------------------------------------------------
	virtual void PrintAll(std::ostream& out) override
	{
		out << "//------------------------------------------------------------------------" << "\n";
		out << "// PBConfigManager:" << "\n";
		for (auto it : m_mapObject)
		{
			out << "// [" << it.second->Name() << "]: " << it.second->Size() << "\n";
		}
		out << "//------------------------------------------------------------------------" << "\n";
	}
};
