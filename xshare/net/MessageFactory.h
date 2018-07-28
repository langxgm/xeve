//------------------------------------------------------------------------
// * @filename: MessageFactory.h
// *
// * @brief: 消息工厂
// *
// * @author: XGM
// * @date: 2017/08/07
//------------------------------------------------------------------------
#pragma once

#include <map>

namespace google {
	namespace protobuf {
		class Message;
	}
}

class MessageFactory
{
public:
	//------------------------------------------------------------------------
	// 注册工厂
	//------------------------------------------------------------------------
	bool Register(uint32_t nMsgID, const ::google::protobuf::Message* pFactory);

	//------------------------------------------------------------------------
	// 取消注册
	//------------------------------------------------------------------------
	void UnRegister(uint32_t nMsgID);

	//------------------------------------------------------------------------
	// 创建消息
	//------------------------------------------------------------------------
	::google::protobuf::Message* NewMessage(uint32_t nMsgID) const;

private:
	std::map<uint32_t, const ::google::protobuf::Message*> m_mapFactory;
};
