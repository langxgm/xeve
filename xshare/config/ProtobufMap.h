//------------------------------------------------------------------------
// * @filename: ProtobufMap.h
// *
// * @brief: protobuf配置表
// *
// * @author: XGM
// * @date: 2017/12/05
//------------------------------------------------------------------------
#pragma once

#include "ConfigReader.h"

#include "xbase/IOUtil.h"

#include <google/protobuf/message.h>
#include <glog/logging.h>

#include <map>
#include <string>
#include <memory>

namespace protobufmap
{
	//------------------------------------------------------------------------
	// 反射字段
	//------------------------------------------------------------------------
	template<class _Type_t>
	struct ReflectionField
	{
		//------------------------------------------------------------------------
		// 获取字段的值
		//------------------------------------------------------------------------
		static _Type_t GetValue(google::protobuf::Message* pMsg, const google::protobuf::FieldDescriptor* pField)
		{
			return 0;
		}
	};

	template<>
	struct ReflectionField<int32_t>
	{
		static int32_t GetValue(google::protobuf::Message* pMsg, const google::protobuf::FieldDescriptor* pField)
		{
			return pMsg->GetReflection()->GetInt32(*pMsg, pField);
		}
	};

	template<>
	struct ReflectionField<uint32_t>
	{
		static uint32_t GetValue(google::protobuf::Message* pMsg, const google::protobuf::FieldDescriptor* pField)
		{
			return pMsg->GetReflection()->GetUInt32(*pMsg, pField);
		}
	};

	template<>
	struct ReflectionField<int64_t>
	{
		static int64_t GetValue(google::protobuf::Message* pMsg, const google::protobuf::FieldDescriptor* pField)
		{
			return pMsg->GetReflection()->GetInt64(*pMsg, pField);
		}
	};

	template<>
	struct ReflectionField<uint64_t>
	{
		static uint64_t GetValue(google::protobuf::Message* pMsg, const google::protobuf::FieldDescriptor* pField)
		{
			return pMsg->GetReflection()->GetUInt64(*pMsg, pField);
		}
	};

	template<>
	struct ReflectionField<std::string>
	{
		static std::string GetValue(google::protobuf::Message* pMsg, const google::protobuf::FieldDescriptor* pField)
		{
			return pMsg->GetReflection()->GetString(*pMsg, pField);
		}
	};
}

class IProtobufMap
{
public:
	virtual ~IProtobufMap() {}
	virtual std::string Name() = 0;
	virtual uint32_t Size() = 0;
};

template<class _Key_t, class _Value_t>
class ProtobufMap : public IProtobufMap
{
public:
	typedef _Key_t key_type;
	typedef _Value_t value_type;
	typedef typename std::remove_pointer<value_type>::type value_nonptr_type;
	typedef std::shared_ptr<value_nonptr_type> value_ptr_type;
	typedef std::map<key_type, value_ptr_type> map_type;
	typedef typename map_type::iterator map_iterator;
	typedef ConfigReader<google::protobuf::Message*, const google::protobuf::Message*> Reader;
public:
	ProtobufMap()
	{
	}

	~ProtobufMap()
	{
	}

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	bool Init(const std::string& strFileName, const std::string& strKeyName, Reader& r)
	{
		int nLength = 0;
		auto pBuffer = IOUtil::ReadFile(strFileName.c_str(), nLength);
		if (!pBuffer)
		{
			LOG(ERROR) << "Config file not exist, fileName=" << strFileName; // 配置文件不存在
			return false;
		}

		auto pFactory = value_nonptr_type::internal_default_instance();
		r.SetFactory(pFactory);

		bool bRet = r.Read(pBuffer.get(), nLength);
		pBuffer.reset();
		if (bRet)
		{
			auto pField = pFactory->GetDescriptor()->FindFieldByName(strKeyName);
			if (pField == nullptr)
			{
				LOG(ERROR) << "Message field undefined, MessageTypeName=" << pFactory->GetTypeName()
					<< " fieldName=" << strKeyName; // 有字段未定义
				return false;
			}
			for (auto& pResult : *r.GetResults())
			{
				auto key = protobufmap::ReflectionField<key_type>::GetValue(pResult, pField);
				auto ret = m_mapValue.insert(std::make_pair(key, value_ptr_type(static_cast<value_type>(pResult))));
				pResult = nullptr;
				if (ret.second == false)
				{
					LOG(ERROR) << "Config key repeat, fileName=" << strFileName
						<< " keyName=" << strKeyName
						<< " keyValue=" << key; // key的值重复
					return false;
				}
			}
			r.GetResults()->clear();

			m_strKeyName = strKeyName;
		}
		return bRet;
	}

	//------------------------------------------------------------------------
	// 查找
	//------------------------------------------------------------------------
	value_ptr_type Find(const key_type& key)
	{
		auto it = m_mapValue.find(key);
		if (it != m_mapValue.end())
		{
			return it->second;
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	// 删除
	//------------------------------------------------------------------------
	value_ptr_type Remove(const key_type& key)
	{
		auto it = m_mapValue.erase(key);
		if (it != m_mapValue.end())
		{
			return it->second;
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	// 删除所有
	//------------------------------------------------------------------------
	void RemoveAll()
	{
		m_mapValue.clear();
	}

	//------------------------------------------------------------------------
	// 名字
	//------------------------------------------------------------------------
	virtual std::string Name() override
	{
		return std::string("ProtobufMap<") + typeid(key_type).name() + " " + GetKeyName()
			+ ", " + typeid(value_type).name() + ">";
	}

	//------------------------------------------------------------------------
	// size
	//------------------------------------------------------------------------
	virtual uint32_t Size() override
	{
		return m_mapValue.size();
	}

	//------------------------------------------------------------------------
	// 获得key名字
	//------------------------------------------------------------------------
	const std::string& GetKeyName()
	{
		return m_strKeyName;
	}

	//------------------------------------------------------------------------
	// 获得值的类型名称
	//------------------------------------------------------------------------
	std::string GetValueTypeName()
	{
		return value_nonptr_type::internal_default_instance()->GetTypeName();
	}

public:
	//------------------------------------------------------------------------
	// begin迭代器
	//------------------------------------------------------------------------
	map_iterator begin()
	{
		return m_mapValue.begin();
	}

	//------------------------------------------------------------------------
	// end迭代器
	//------------------------------------------------------------------------
	map_iterator end()
	{
		return m_mapValue.end();
	}

private:
	std::string m_strKeyName;
	map_type m_mapValue;
};
