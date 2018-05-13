//------------------------------------------------------------------------
// * @filename: ProtobufReader.h
// *
// * @brief: protobuf配置读取
// *
// * @author: XGM
// * @date: 2017/12/05
//------------------------------------------------------------------------
#pragma once

#include "ConfigReader.h"

namespace google {
	namespace protobuf {
		class Message;
	}
}

class ProtobufReader : public ConfigReader<google::protobuf::Message*, const google::protobuf::Message*>
{
public:
	ProtobufReader() {}
	virtual ~ProtobufReader();

	//------------------------------------------------------------------------
	// 读取
	//------------------------------------------------------------------------
	virtual bool Read(const char* pBuffer, uint32_t nLength) override;

	//------------------------------------------------------------------------
	// 返回结果
	//------------------------------------------------------------------------
	virtual result_type GetResult(bool isRelease) override
	{
		if (isRelease)
		{
			auto pResult = m_pReceiver;
			m_pReceiver = nullptr;
			return pResult;
		}
		return m_pReceiver;
	}

	//------------------------------------------------------------------------
	// 设置用于创建结果的工厂
	//------------------------------------------------------------------------
	virtual void SetFactory(factory_type f) override { m_pFactory = f; }

protected:
	factory_type m_pFactory = nullptr;
	result_type m_pReceiver = nullptr;
};

class MultiProtobufReader : public ConfigReader<google::protobuf::Message*, const google::protobuf::Message*>
{
public:
	MultiProtobufReader() {}
	virtual ~MultiProtobufReader();

	//------------------------------------------------------------------------
	// 读取
	//------------------------------------------------------------------------
	virtual bool Read(const char* pBuffer, uint32_t nLength) override;

	//------------------------------------------------------------------------
	// 返回结果
	//------------------------------------------------------------------------
	virtual result_type GetResult(bool isRelease) override { return nullptr; }

	//------------------------------------------------------------------------
	// 返回结果集
	//------------------------------------------------------------------------
	virtual std::list<result_type>* GetResults() override { return &m_listResult; }

	//------------------------------------------------------------------------
	// 设置用于创建结果的工厂
	//------------------------------------------------------------------------
	virtual void SetFactory(factory_type f) override { m_pFactory = f; }

protected:
	factory_type m_pFactory = nullptr;
	std::list<result_type> m_listResult;
};
