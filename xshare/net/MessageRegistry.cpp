
#include "MessageRegistry.h"
#include "CRC32.h"

#include <evpp/logging.h>

#include <google/protobuf/message.h>

void MessageRegistry::RegisterMessage(uint32_t nMsgID, const MessageHandleFunc& handle,
	const ::google::protobuf::Message* pFactory)
{
	bool bRet = m_aMessageFactory.Register(nMsgID, pFactory);
	if (!bRet)
	{
		assert(bRet); // 消息ID重复
		LOG_ERROR << "Message repeat register MsgID=" << nMsgID;
		return;
	}
	m_aHandleManager.Register(nMsgID, handle);
}

void MessageRegistry::RegisterMessage(const MessageHandleFunc& handle,
	const ::google::protobuf::Message* pFactory)
{
	uint32_t nMsgID = Crc32(pFactory->GetTypeName().c_str());
	RegisterMessage(nMsgID, handle, pFactory);
}
