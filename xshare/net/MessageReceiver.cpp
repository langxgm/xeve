
#include "MessageReceiver.h"
#include "MessageCounter.h"
#include "Session.h"
#include "SessionManager.h"
#include "SessionStatistics.h"
#include "SessionFactory.h"
#include "MessagePacker.h"

#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>

MessageReceiver::MessageReceiver()
{
}

MessageReceiver::~MessageReceiver()
{
	m_vecSessions.clear();
	m_pMessagePacker.reset();
	m_pSessionFactory.reset();
}

void MessageReceiver::Init(evpp::EventLoop* pLoop, uint32_t nSectionNum)
{
	m_pLoop = pLoop;

	m_pSessionFactory.reset(CreateSessionFactory());
	m_pMessagePacker.reset(CreateMessagePacker());

	nSectionNum = (std::max)(nSectionNum, (uint32_t)1);
	m_vecSessions.reserve(nSectionNum);
	for (uint32_t i = 0; i < nSectionNum; ++i)
	{
		m_vecSessions.push_back(SessionMgrPtr(m_pSessionFactory->NewSessionManager()/*new SessionManager()*/));
	}
	m_nSectionNum = nSectionNum;

	RegisterListen();
}

MessagePacker* MessageReceiver::CreateMessagePacker()
{
	return new MessagePacker();
}

void MessageReceiver::OnConnection(const evpp::TCPConnPtr& conn)
{
	// 连接/断开
	if (conn->IsConnected())
	{
		conn->SetTCPNoDelay(true);

		auto& pSMgr = GetSessionMgrPtr(conn->id());
		std::lock_guard<std::recursive_mutex> guard(pSMgr->GetLock());

		SessionPtr pSession(m_pSessionFactory->NewSession(conn)/*new Session(conn)*/);
		pSession->SetConnected(true);
		pSession->SetStat(SessionStatPtr(m_pSessionFactory->NewSessionStatistics()/*new SessionStatistics()*/));

		pSMgr->Add(conn->id(), pSession);
	}
	else
	{
		auto& pSMgr = GetSessionMgrPtr(conn->id());
		std::lock_guard<std::recursive_mutex> guard(pSMgr->GetLock());

		// 处理完消息列表,再删除
		// 保证执行消息时连接的有效性
		const SessionPtr& pSession = pSMgr->Get(conn->id());
		if (pSession)
		{
			pSession->SetConnected(false);
		}
	}
}

void MessageReceiver::OnMessage(const evpp::TCPConnPtr& conn, evpp::Buffer* pBuf)
{
	m_pMessagePacker->Unpack(this, conn, pBuf);
}

void MessageReceiver::CacheMessage(MessagePacket& aMsgPacket, int64_t nSessionID)
{
	auto& pSMgr = GetSessionMgrPtr(nSessionID);
	std::lock_guard<std::recursive_mutex> guard(pSMgr->GetLock());

	const SessionPtr& pSession = pSMgr->Get(nSessionID);
	if (pSession)
	{
		pSession->AddCacheMessage(aMsgPacket);
	}
}

void MessageReceiver::Update()
{
	// 搜集所有的消息和断开的连接
	std::list<MessagePacket> aMsgList;
	std::list<uint64_t> aDisconnectList;
	// 扫描
	{
		std::list<MessagePacket> aList;
		for (auto& pSMgr : m_vecSessions)
		{
			std::lock_guard<std::recursive_mutex> guard(pSMgr->GetLock());
			pSMgr->Visit([this, &aList, &aMsgList, &aDisconnectList](const SessionPtrMap& mapSession) {
				for (auto& it : mapSession)
				{
					const SessionPtr& pSession = it.second;
					// 每个连接(每帧)处理N个消息
					pSession->PopCacheMessages(aList, m_nMaxDealPerFrame);
					aMsgList.splice(aMsgList.end(), aList);
					if (!pSession->Connected())
					{
						aDisconnectList.push_back(pSession->GetID());
					}
				}
			});
		}
	}

	// 消息统计
	auto pCounter = Counter();

	// 处理消息
	for (auto& rMsg : aMsgList)
	{
		if (pCounter)
		{
			pCounter->OnRecv(rMsg.nSessionID, rMsg.pMsg.get());
		}

		uint32_t nCount = m_aHandleManager.HandleMessage(rMsg.nMsgID, rMsg.pMsg, rMsg.nSessionID, rMsg.pMeta);
		assert(nCount);
		if (nCount > 0)
		{
			// TODO
		}
	}

	// 处理断开的连接
	for (auto& nSessionID : aDisconnectList)
	{
		OnDisconnect(nSessionID);
	}
	// 删除断开的连接
	for (auto& nSessionID : aDisconnectList)
	{
		auto& pSMgr = GetSessionMgrPtr(nSessionID);
		std::lock_guard<std::recursive_mutex> guard(pSMgr->GetLock());
		pSMgr->Remove(nSessionID);
	}
}

evpp::TCPConnPtr MessageReceiver::GetConnPtr(int64_t nSessionID)
{
	auto& pSMgr = GetSessionMgrPtr(nSessionID);
	std::lock_guard<std::recursive_mutex> guard(pSMgr->GetLock());

	const SessionPtr& pSession = pSMgr->Get(nSessionID);
	if (pSession)
	{
		return pSession->GetConn();
	}
	return nullptr;
}

void MessageReceiver::CloseConn(int64_t nSessionID)
{
	auto conn = GetConnPtr(nSessionID);
	if (conn)
	{
		conn->Close();
	}
}

uint64_t MessageReceiver::GetSessionNum()
{
	uint64_t nSessionNum = 0;
	for (auto& pSMgr : m_vecSessions)
	{
		std::lock_guard<std::recursive_mutex> guard(pSMgr->GetLock());
		nSessionNum += pSMgr->Size();
	}
	return nSessionNum;
}

uint32_t MessageReceiver::GetSectionIndex(int64_t nSessionID)
{
	return nSessionID % m_nSectionNum;
}

const SessionMgrPtr& MessageReceiver::GetSessionMgrPtr(int64_t nSessionID)
{
	uint32_t nIndex = GetSectionIndex(nSessionID);
	assert(nIndex < m_vecSessions.size());
	return m_vecSessions[nIndex];
}

int MessageReceiver::Send(int64_t nSessionID, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta)
{
	auto pWriteBuffer = m_aWriteBufferAllocator.Alloc();
	WriteBuffer(pWriteBuffer.get(), pMsg, pMeta);
	{
		auto conn = GetConnPtr(nSessionID);
		if (conn)
		{
			Send(conn, pWriteBuffer->data(), pWriteBuffer->length());
			ResetBuffer(pWriteBuffer.get());

			if (auto pCounter = Counter())
			{
				pCounter->OnSend(nSessionID, pMsg);
			}
			return 0;
		}
	}
	ResetBuffer(pWriteBuffer.get());
	return -1;
}

int MessageReceiver::Send(const std::vector<int64_t>& vecSessionID, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta)
{
	auto pWriteBuffer = m_aWriteBufferAllocator.Alloc();
	WriteBuffer(pWriteBuffer.get(), pMsg, pMeta);
	{
		for (auto& nSessionID : vecSessionID)
		{
			auto conn = GetConnPtr(nSessionID);
			if (conn)
			{
				Send(conn, pWriteBuffer->data(), pWriteBuffer->length());

				if (auto pCounter = Counter())
				{
					pCounter->OnSend(nSessionID, pMsg);
				}
			}
		}
	}
	ResetBuffer(pWriteBuffer.get());
	return 0;
}

int MessageReceiver::Send(const std::function<int64_t(void)>& funcNext, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta)
{
	auto pWriteBuffer = m_aWriteBufferAllocator.Alloc();
	WriteBuffer(pWriteBuffer.get(), pMsg, pMeta);
	{
		while (auto nSessionID = funcNext())
		{
			auto conn = GetConnPtr(nSessionID);
			if (conn)
			{
				Send(conn, pWriteBuffer->data(), pWriteBuffer->length());

				if (auto pCounter = Counter())
				{
					pCounter->OnSend(nSessionID, pMsg);
				}
			}
		}
	}
	ResetBuffer(pWriteBuffer.get());
	return 0;
}

int MessageReceiver::Send(int64_t nSessionID, const void* pMsg, size_t nLen)
{
	auto conn = GetConnPtr(nSessionID);
	if (conn)
	{
		Send(conn, pMsg, nLen);
		return 0;
	}
	return -1;
}

void MessageReceiver::Send(const evpp::TCPConnPtr& conn, const void* pMsg, size_t nLen)
{
	conn->Send(pMsg, nLen);
}

int MessageReceiver::Send(const std::vector<int64_t>& vecSessionID, const void* pMsg, size_t nLen)
{
	for (auto& nSessionID : vecSessionID)
	{
		auto conn = GetConnPtr(nSessionID);
		if (conn)
		{
			Send(conn, pMsg, nLen);
		}
	}
	return 0;
}

void MessageReceiver::SendToAll(const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta)
{
	auto pWriteBuffer = m_aWriteBufferAllocator.Alloc();
	WriteBuffer(pWriteBuffer.get(), pMsg, pMeta);
	{
		std::list<evpp::TCPConnPtr> vecConn;
		// 获得所有连接
		for (auto& pSMgr : m_vecSessions)
		{
			std::lock_guard<std::recursive_mutex> guard(pSMgr->GetLock());
			pSMgr->Visit([this, &vecConn](const SessionPtrMap& mapSession) {
				for (auto& it : mapSession)
				{
					vecConn.push_back(it.second->GetConn());
				}
			});
		}

		for (auto& conn : vecConn)
		{
			Send(conn, pWriteBuffer->data(), pWriteBuffer->length());
		}
	}
	ResetBuffer(pWriteBuffer.get());

	if (auto pCounter = Counter())
	{
		pCounter->OnSend(-1, pMsg);
	}
}

void MessageReceiver::WriteBuffer(evpp::Buffer* pWriteBuffer, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta)
{
	m_pMessagePacker->Pack(this, pWriteBuffer, pMsg, pMeta);
}

void MessageReceiver::WriteBuffer(evpp::Buffer* pWriteBuffer, const void* pMsg, size_t nMsgLen, const MessageMeta* pMeta)
{
	m_pMessagePacker->PackBytes(this, pWriteBuffer, pMsg, nMsgLen, pMeta);
}

void MessageReceiver::ResetBuffer(evpp::Buffer* pWriteBuffer)
{
	pWriteBuffer->Reset();
}

std::string MessageReceiver::GetStatusInfo()
{
	std::string info;
	info.reserve(512);

	auto nSessionNum = GetSessionNum();

	info += "MessageReceiver: {";
	info.append(" sessionNum: ").append(std::to_string(nSessionNum));
	info.append(", ").append(m_aWriteBufferAllocator.GetAllocInfo());
	info += " }";
	return std::move(info);
}
