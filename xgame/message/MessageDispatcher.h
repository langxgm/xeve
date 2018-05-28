//------------------------------------------------------------------------
// * @filename: MessageDispatcher.h
// *
// * @brief: 消息管理器
// *
// * @author: XGM
// * @date: 2017/01/23
//------------------------------------------------------------------------
#pragma once

#include "xbase/Singleton.h"
#include "xgame/object/GameType.h"
#include "xgame/message/Message.h"

#include <set>

// 立即发送消息
#define SEND_MSG_IMMEDIATELY (0)
// 没有附加信息
#define NO_ADDITIONAL_INFO (nullptr)

class MessageHandler;

class MessageDispatcher : public Singleton<MessageDispatcher>
{
protected:
	friend class Singleton<MessageDispatcher>;
	MessageDispatcher()
	{
	}
	~MessageDispatcher()
	{
	}

public:
	//------------------------------------------------------------------------
	// 发送消息
	//------------------------------------------------------------------------
	void DispatchMessage(int64_t nDelay, ObjID_t nSender, ObjID_t nReceiver, int32_t nMsgID, const std::shared_ptr<MsgExtraInfo>& spMsgExtraInfo);

	//------------------------------------------------------------------------
	// 定时发送延迟消息
	//------------------------------------------------------------------------
	void DispatchDelayedMessages(int64_t nNow);

private:
	//------------------------------------------------------------------------
	// 目标对象接收消息
	//------------------------------------------------------------------------
	void Discharge(MessageHandler* pReceiver, const Message* pMsg);

private:
	// 存放延迟消息的容器,按照消息的发送时间排序
	std::set<Message> m_Messages;
};
