
#include "Session.h"
#include "SessionStatistics.h"

#include <evpp/tcp_conn.h>

Session::Session(evpp::TCPConnPtr conn)
	: m_pConn(conn)
{

}

Session::~Session()
{
	m_pStat.reset();
}

uint64_t Session::GetID()
{
	return m_pConn->id();
}

void Session::SetStat(SessionStatPtr&& pStat)
{
	m_pStat = std::move(pStat);
}

void Session::AddCacheMessage(MessagePacket& aMsgPacket)
{
	m_listMessage.push_back(std::move(aMsgPacket));

	if (m_pStat)
	{
		m_pStat->AddMessageCount(m_listMessage.back().nMsgID, 1);
	}
}

void Session::PopCacheMessage(MessagePacket& aMsgPacket)
{
	if (m_listMessage.size() > 0)
	{
		aMsgPacket = std::move(m_listMessage.front());
		m_listMessage.pop_front();
	}
}

void Session::PopCacheMessages(std::list<MessagePacket>& rList, uint32_t nMsgNum)
{
	if (m_listMessage.size() == 0)
	{
		return;
	}

	if (m_listMessage.size() <= nMsgNum)
	{
		rList.swap(m_listMessage);
	}
	else
	{
		auto itEnd = m_listMessage.begin();
		while (nMsgNum-- && itEnd != m_listMessage.end())
		{
			++itEnd;
		}
		rList.splice(rList.end(), m_listMessage, m_listMessage.begin(), itEnd);
	}
}

std::string Session::GetStatusInfo()
{
	std::string info;
	info.reserve(160);

	info += "Session: {";
	info.append(" id: ").append(std::to_string(GetID()));
	info.append(", connected: ").append(m_bConnected ? "true" : "false");
	info.append(", messageSize: ").append(std::to_string(m_listMessage.size()));

	if (m_pStat)
	{
		info.append(", ").append(m_pStat->GetStatusInfo());
	}

	info += " }";
	return std::move(info);
}
