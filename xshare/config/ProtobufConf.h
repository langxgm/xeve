//------------------------------------------------------------------------
// * @filename: ProtobufConf.h
// *
// * @brief: protobuf配置
// *
// * @author: XGM
// * @date: 2017/12/12
//------------------------------------------------------------------------
#pragma once

#include "ConfigReader.h"

#include "xbase/IOUtil.h"

#include <google/protobuf/message.h>
#include <glog/logging.h>

#include <string>
#include <memory>

template<class _Value_t>
class ProtobufConf
{
public:
	typedef _Value_t value_type;
	typedef typename std::remove_pointer<value_type>::type value_nonptr_type;
	typedef std::unique_ptr<value_nonptr_type> value_ptr_type;
	typedef ConfigReader<google::protobuf::Message*, const google::protobuf::Message*> Reader;
public:
	ProtobufConf()
	{
	}

	~ProtobufConf()
	{
	}

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	bool Init(const std::string& strFileName, Reader& r)
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
			m_pValue.reset(static_cast<value_type>(r.GetResult(true)));
		}
		return bRet;
	}

	//------------------------------------------------------------------------
	// 获取值
	//------------------------------------------------------------------------
	value_ptr_type& Value() { return m_pValue; }

	//------------------------------------------------------------------------
	// 获得值的类型名称
	//------------------------------------------------------------------------
	std::string GetValueTypeName()
	{
		return value_nonptr_type::internal_default_instance()->GetTypeName();
	}

private:
	value_ptr_type m_pValue = nullptr;
};
