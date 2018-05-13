//------------------------------------------------------------------------
// * @filename: MessageCounter.h
// *
// * @brief: 消息统计接口
// *
// * @author: XGM
// * @date: 2017/11/17
//------------------------------------------------------------------------
#pragma once

namespace google {
	namespace protobuf {
		class Message;
	}
}

class MessageCounter
{
public:
	virtual ~MessageCounter() {}

	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	virtual void OnSend(int64_t nSessionID, const ::google::protobuf::Message* pMsg) {}

	//------------------------------------------------------------------------
	// 接收消息
	//------------------------------------------------------------------------
	virtual void OnRecv(int64_t nSessionID, const::google::protobuf::Message* pMsg) {}
};
