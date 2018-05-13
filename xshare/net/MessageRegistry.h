//------------------------------------------------------------------------
// * @filename: MessageRegistry.h
// *
// * @brief: 消息注册表
// *
// * @author: XGM
// * @date: 2017/08/09
//------------------------------------------------------------------------
#pragma once

#include "MessageFactory.h"
#include "MessageHandleManager.h"

class MessageRegistry
{
public:
	//------------------------------------------------------------------------
	// 注册监听的消息
	//------------------------------------------------------------------------
	virtual void RegisterListen() = 0;

	//------------------------------------------------------------------------
	// 注册消息
	//------------------------------------------------------------------------
	void RegisterMessage(uint32_t nMsgID, const MessageHandleFunc& handle,
		const ::google::protobuf::Message* pFactory);

	//------------------------------------------------------------------------
	// 注册消息
	//------------------------------------------------------------------------
	void RegisterMessage(const MessageHandleFunc& handle,
		const ::google::protobuf::Message* pFactory);

protected:
	// 消息工厂
	MessageFactory m_aMessageFactory;
	// 消息管理
	MessageHandleManager m_aHandleManager;
};

//------------------------------------------------------------------------
// 注册消息宏
//------------------------------------------------------------------------
#ifndef REG_MSG
#define REG_MSG(func, owner, factory, tips) \
	RegisterMessage(std::bind(func, owner, std::placeholders::_1, std::placeholders::_2),\
		factory::internal_default_instance())
#endif

//------------------------------------------------------------------------
// 在目标处注册消息
//------------------------------------------------------------------------
#ifndef REG_MSG_TO
#define REG_MSG_TO(registry, func, owner, factory, tips) \
	registry->RegisterMessage(std::bind(func, owner, std::placeholders::_1, std::placeholders::_2),\
		factory::internal_default_instance())
#endif
