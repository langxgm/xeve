
#include "MessageHandleManager.h"

MessageHandleManager::MessageHandleManager()
{

}

MessageHandleManager::~MessageHandleManager()
{
	for (auto& it : m_mapHandle)
	{
		delete (it.second);
	}
}

void MessageHandleManager::Register(uint32_t nMsgID, const MessageHandleFunc& handle)
{
	auto itFind = m_mapHandle.find(nMsgID);
	if (itFind != m_mapHandle.end())
	{
		itFind->second->push_back(handle);
	}
	else
	{
		std::list<MessageHandleFunc>* pList = new std::list<MessageHandleFunc>();
		pList->push_back(handle);
		m_mapHandle.insert(std::make_pair(nMsgID, pList));
	}
}

void MessageHandleManager::UnRegister(uint32_t nMsgID)
{
	auto it = m_mapHandle.find(nMsgID);
	if (it != m_mapHandle.end())
	{
		delete (it->second);
		m_mapHandle.erase(it);
	}
}

int MessageHandleManager::HandleMessage(uint32_t nMsgID, const MessagePtr& pMsg, int64_t nSessionID, const MessageMetaPtr& pMeta)
{
	uint32_t nCount = 0;
	auto itFind = m_mapHandle.find(nMsgID);
	if (itFind != m_mapHandle.end())
	{
		std::list<MessageHandleFunc>* pList = itFind->second;
		for (auto& it : *pList)
		{
			(it)(pMsg, nSessionID, pMeta);
			++nCount;
		}
	}
	return nCount;
}
