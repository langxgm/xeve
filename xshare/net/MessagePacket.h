//------------------------------------------------------------------------
// * @filename: MessagePacket.h
// *
// * @brief: 消息包
// *
// * @author: XGM
// * @date: 2017/08/13
//------------------------------------------------------------------------
#pragma once

#include "MessagePtr.h"

// 消息包
struct MessagePacket
{
	uint32_t nMsgID = 0; // 消息ID
	uint64_t nSessionID = 0; // 会话ID
	MessagePtr pMsg; // 消息
};
