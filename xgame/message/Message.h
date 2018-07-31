//------------------------------------------------------------------------
// * @filename: Message.h
// *
// * @brief: 消息结构
// *
// * @author: XGM
// * @date: 2017/01/23
//------------------------------------------------------------------------
#pragma once

#include <memory>
#include <math.h>
#include <stdint.h>

struct MsgExtraInfo
{
};

struct Message
{
	int64_t			nMsgID = 0;			// 消息ID
	int64_t			nSender = 0;		// 发送者
	int64_t			nReceiver = 0;		// 接受者
	int64_t			nDispatchTime = 0;	// 消息发送时间(单位:毫秒)
	std::shared_ptr<MsgExtraInfo> spExtraInfo; // 附加的信息

	Message()
	{
	}

	Message(const Message& rhs)
	{
		nMsgID = rhs.nMsgID;
		nSender = rhs.nSender;
		nReceiver = rhs.nReceiver;
		nDispatchTime = rhs.nDispatchTime;
		spExtraInfo = rhs.spExtraInfo;
	}

	Message& operator=(const Message& rhs)
	{
		return assign(rhs);
	}

	Message& assign(const Message& rhs)
	{
		if (this != &rhs)
		{
			nMsgID = rhs.nMsgID;
			nSender = rhs.nSender;
			nReceiver = rhs.nReceiver;
			nDispatchTime = rhs.nDispatchTime;
			spExtraInfo = rhs.spExtraInfo;
		}
		return *this;
	}
};

// 过滤相同消息的时间差
const int64_t g_IgnoreDelay = 25;

inline bool operator==(const Message& lhs, const Message& rhs)
{
	return abs(lhs.nDispatchTime - rhs.nDispatchTime) < g_IgnoreDelay
		&& (lhs.nMsgID == rhs.nMsgID)
		&& (lhs.nSender == rhs.nSender)
		&& (lhs.nReceiver == rhs.nReceiver)
		&& (lhs.spExtraInfo.get() == rhs.spExtraInfo.get());
}

inline bool operator<(const Message& lhs, const Message& rhs)
{
	if (lhs == rhs)
	{
		return false;
	}
	return (lhs.nDispatchTime < rhs.nDispatchTime);
}
