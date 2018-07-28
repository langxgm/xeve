//------------------------------------------------------------------------
// * @filename: MessageHandleManager.h
// *
// * @brief: 消息处理管理器
// *
// * @author: XGM
// * @date: 2017/08/07
//------------------------------------------------------------------------
#pragma once

#include "MessagePacket.h"

#include <functional>
#include <memory>
#include <list>
#include <map>

//------------------------------------------------------------------------
// 消息处理的回调函数
// pMsg:消息
// nSessionID:会话ID
//------------------------------------------------------------------------
typedef std::function<void(const MessagePtr& pMsg, int64_t nSessionID, const MessageMetaPtr& pMeta)> MessageHandleFunc;

class MessageHandleManager
{
public:
	MessageHandleManager();
	~MessageHandleManager();

	//------------------------------------------------------------------------
	// 注册监听的消息
	//------------------------------------------------------------------------
	void Register(uint32_t nMsgID, const MessageHandleFunc& handle);

	//------------------------------------------------------------------------
	// 取消注册
	//------------------------------------------------------------------------
	void UnRegister(uint32_t nMsgID);

	//------------------------------------------------------------------------
	// 处理消息
	//------------------------------------------------------------------------
	int HandleMessage(uint32_t nMsgID, const MessagePtr& pMsg, int64_t nSessionID, const MessageMetaPtr& pMeta);

private:
	// 消息监听列表
	std::map<uint32_t, std::list<MessageHandleFunc>*> m_mapHandle;
};
