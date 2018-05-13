
#include "MessageFactory.h"

#include <google/protobuf/message.h>

bool MessageFactory::Register(uint32_t nMsgID, const ::google::protobuf::Message* pFactory)
{
	auto ret = m_mapFactory.insert(std::make_pair(nMsgID, pFactory));
	return ret.second;
}

void MessageFactory::UnRegister(uint32_t nMsgID)
{
	m_mapFactory.erase(nMsgID);
}

google::protobuf::Message* MessageFactory::NewMessage(uint32_t nMsgID)
{
	auto itFind = m_mapFactory.find(nMsgID);
	if (itFind != m_mapFactory.end())
	{
		return itFind->second->New();
	}
	return nullptr;
}
