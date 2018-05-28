
#include "MessageDispatcher.h"
#include "xgame/object/EntityManager.h"
#include "xgame/object/GameObject_Entity.h"

#include "xbase/TimeUtil.h"

//#include <algorithm>

void MessageDispatcher::DispatchMessage(int64_t nDelay, ObjID_t nSender, ObjID_t nReceiver, int32_t nMsgID, const std::shared_ptr<MsgExtraInfo>& spMsgExtraInfo)
{
	Message msg;
	msg.nMsgID = nMsgID;
	msg.nSender = nSender;
	msg.nReceiver = nReceiver;
	msg.spExtraInfo = spMsgExtraInfo;

	// 发送
	if (nDelay <= 0)
	{
		// 立即发送
		auto pReceiver = EntityManager::GetInstance()->GetEntityByID(msg.nReceiver);
		Discharge(pReceiver, &msg);
	}
	else
	{
		// 延迟发送
		int64_t nNow = TimeUtil::GetCurrentTimeMillis();
		msg.nDispatchTime = nNow + nDelay;

		// <set>
		m_Messages.insert(msg);

		// <list> // 遍历效率低
		//bool bExist = (std::find(m_Messages.begin(), m_Messages.end(), msg) != m_Messages.end());
		//if (bExist)
		//{
		//	// 重复的消息被过滤了
		//}
		//else
		//{
		//	auto pos = std::find_if(m_Messages.begin(), m_Messages.end(),
		//		[&msg](auto& v)
		//		{
		//			return msg < v;
		//		});
		//	m_Messages.insert(pos, msg);
		//}
	}
}

void MessageDispatcher::DispatchDelayedMessages(int64_t nNow)
{
	for (auto it = m_Messages.begin(); it != m_Messages.end();)
	{
		auto& rMsg = (*it);
		if (nNow > rMsg.nDispatchTime && rMsg.nDispatchTime > 0)
		{
			auto pReceiver = EntityManager::GetInstance()->GetEntityByID(rMsg.nReceiver);
			Discharge(pReceiver, &rMsg);
			it = m_Messages.erase(it);
		}
		else
		{
			break;
		}
	}
}

void MessageDispatcher::Discharge(MessageHandler* pReceiver, const Message* pMsg)
{
	if (pReceiver)
	{
		if (pReceiver->HandleMessage(pMsg) == false)
		{
			//std::cout << "Message not handled" << std::endl;
		}
	}
}
