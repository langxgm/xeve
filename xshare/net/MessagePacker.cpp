
#include "MessagePacker.h"
#include "MessageReceiver.h"
#include "CRC32.h"

#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>

#include <google/protobuf/message.h>

MessageMeta* MessagePacker::NewMeta()
{
	return new MessageMeta();
}

void MessagePacker::Pack(MessageReceiver* pOwner, evpp::Buffer* pBuf, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta /*= nullptr*/)
{
	// 消息ID
	uint32_t nMsgID = Crc32(pMsg->GetTypeName().c_str());
	if (pMeta)
	{
		const_cast<MessageMeta*>(pMeta)->SetMsgID(nMsgID);
	}

	// 打包前
	PackBefore(nMsgID, pMsg, pMeta);

	// 打包中
	MessageHeader aHeader;
	aHeader.nMsgLen = pMsg->ByteSizeLong();
	aHeader.nMetaLen = pMeta ? pMeta->GetByteSize() : 4; // 4:消息ID
	aHeader.nTotalLen = MessageHeader::GetHeaderByteSize() + aHeader.nMetaLen + aHeader.nMsgLen;

	if (aHeader.nTotalLen > pBuf->WritableBytes())
	{
		pBuf->EnsureWritableBytes(aHeader.nTotalLen);
	}

	PackHeader(pOwner, pBuf, &aHeader);
	PackMeta(pOwner, pBuf, nMsgID, pMeta);
	PackBody(pOwner, pBuf, pMsg, &aHeader);

	// 打包后
	PackAfter(nMsgID, pMsg, pMeta);
}

void MessagePacker::PackBytes(MessageReceiver* pOwner, evpp::Buffer* pBuf, const void* pMsg, size_t nMsgLen, const MessageMeta* pMeta /*= nullptr*/)
{
	MessageHeader aHeader;
	aHeader.nMsgLen = nMsgLen;
	aHeader.nMetaLen = pMeta ? pMeta->GetByteSize() : 4; // 4:消息ID
	aHeader.nTotalLen = MessageHeader::GetHeaderByteSize() + aHeader.nMetaLen + aHeader.nMsgLen;

	if (aHeader.nTotalLen > pBuf->WritableBytes())
	{
		pBuf->EnsureWritableBytes(aHeader.nTotalLen);
	}

	PackHeader(pOwner, pBuf, &aHeader);
	PackMeta(pOwner, pBuf, pMeta->GetMsgID(), pMeta);
	PackBodyBytes(pOwner, pBuf, pMsg, nMsgLen, &aHeader);
}

void MessagePacker::Unpack(MessageReceiver* pOwner, const evpp::TCPConnPtr& conn, evpp::Buffer* pBuf)
{
	const uint32_t nHeaderLen = MessageHeader::GetHeaderByteSize();

	// 读取消息
	while (pBuf->size() >= nHeaderLen)
	{
		// 消息包长度
		const uint32_t nLen = pBuf->PeekInt32();
		if (nLen > pOwner->MaxMessageLen() || nLen < nHeaderLen
			|| pBuf->size() > pOwner->MaxBufLen())
		{
			LOG_ERROR << "Invalid length=" << nLen
				<< " bufsize=" << pBuf->size()
				<< " addr=" << conn->AddrToString();
			conn->Close();
			break;
		}

		if (pBuf->size() < nLen)
		{
			break;
		}

		MessageHeader aHeader;
		if (UnpackHeader(pOwner, pBuf, &aHeader) == false)
		{
			LOG_ERROR << "UnpackHeader Failed";
			conn->Close();
			break;
		}

		assert(aHeader.nTotalLen == nLen);

		MessageMetaPtr pMeta(NewMeta());
		if (pMeta)
		{
			if (UnpackMeta(pOwner, pBuf, &aHeader, pMeta) == false)
			{
				LOG_ERROR << "UnpackMeta Failed";
				conn->Close();
				break;
			}
		}
		else
		{
			LOG_ERROR << "NewMeta Failed";
			conn->Close();
			break;
		}

		if (UnpackBody(pOwner, conn->id(), pBuf, &aHeader, pMeta) == false)
		{
			LOG_ERROR << "UnpackBody Failed";
			conn->Close();
			break;
		}
	}
}

void MessagePacker::PackHeader(MessageReceiver* pOwner, evpp::Buffer* pBuf, MessageHeader* pHeader)
{
	// 总长度
	pBuf->AppendInt32(pHeader->nTotalLen);
	// 元数据长度
	pBuf->AppendInt32(pHeader->nMetaLen);
}

bool MessagePacker::UnpackHeader(MessageReceiver* pOwner, evpp::Buffer* pBuf, MessageHeader* pHeader)
{
	const uint32_t nHeaderLen = MessageHeader::GetHeaderByteSize();

	// 总长度
	pHeader->nTotalLen = pBuf->ReadInt32();
	// 元数据长度
	pHeader->nMetaLen = pBuf->ReadInt32();
	// 消息长度
	pHeader->nMsgLen = pHeader->nTotalLen - nHeaderLen - pHeader->nMetaLen;

	if (pHeader->nMetaLen > (pHeader->nTotalLen - nHeaderLen)
		|| pHeader->nMsgLen >= (pHeader->nTotalLen - nHeaderLen))
	{
		LOG_ERROR << "Invalid MessageHeader "
			<< " totalLen=" << pHeader->nTotalLen
			<< " metaLen=" << pHeader->nMetaLen
			<< " msgLen=" << pHeader->nMsgLen;
		return false;
	}
	return true;
}

void MessagePacker::PackMeta(MessageReceiver* pOwner, evpp::Buffer* pBuf, uint32_t nMsgID, const MessageMeta* pMeta)
{
	// 消息ID
	pBuf->AppendInt32(nMsgID);

	// 写入元数据自定义内容
	if (pMeta)
	{
		if (auto pUserdata = const_cast<MessageMeta*>(pMeta)->GetUserdata())
		{
			if (auto nByteSize = pUserdata->ByteSizeLong())
			{
				if (pUserdata->SerializeToArray(pBuf->WriteBegin(), pBuf->WritableBytes()))
				{
					pBuf->WriteBytes(nByteSize);
				}
			}
		}
	}
}

bool MessagePacker::UnpackMeta(MessageReceiver* pOwner, evpp::Buffer* pBuf, const MessageHeader* pHeader, MessageMetaPtr& pMeta)
{
	if (pHeader->nMetaLen < pMeta->GetByteSize())
	{
		LOG_ERROR << "Failed metaLen=" << pHeader->nMetaLen
			<< " metaByteSize=" << pMeta->GetByteSize();
		return false;
	}

	// 消息ID
	pMeta->SetMsgID(pBuf->ReadInt32());

	// 读取元数据自定义内容
	evpp::Slice s = pBuf->Next(pHeader->nMetaLen - pMeta->GetBaseByteSize());
	if (s.size() > 0)
	{
		if (auto pUserdata = pMeta->GetUserdata())
		{
			if (pUserdata->ParseFromArray(s.data(), s.size()) == false)
			{
				LOG_ERROR << "Failed metadata.parse() msgID=" << pMeta->GetMsgID()
					<< " userdataName=" << pUserdata->GetTypeName();
				return false;
			}
			// 移除已读
			s.remove_prefix(pUserdata->ByteSizeLong());
		}
	}

	return true;
}

void MessagePacker::PackBody(MessageReceiver* pOwner, evpp::Buffer* pBuf, const ::google::protobuf::Message* pMsg, const MessageHeader* pHeader)
{
	// 消息内容长度
	size_t nSize = pHeader->nMsgLen;
	// 消息内容
	pMsg->SerializeToArray((void*)pBuf->WriteBegin(), pBuf->WritableBytes());
	pBuf->WriteBytes(nSize);
}

void MessagePacker::PackBodyBytes(MessageReceiver* pOwner, evpp::Buffer* pBuf, const void* pMsg, size_t nMsgLen, const MessageHeader* pHeader)
{
	pBuf->Write(pMsg, nMsgLen);
}

bool MessagePacker::UnpackBody(MessageReceiver* pOwner, int64_t nSessionID, evpp::Buffer* pBuf, const MessageHeader* pHeader, MessageMetaPtr& pMeta)
{
	evpp::Slice s = pBuf->Next(pHeader->nMsgLen);

	uint32_t nMsgID = pMeta->GetMsgID();
	MessagePtr pMsg(pOwner->GetMessageFactory().NewMessage(nMsgID));
	if (pMsg)
	{
		if (pMsg->ParseFromArray(s.data(), s.size()))
		{
			MessagePacket aMsgPacket;
			aMsgPacket.nSessionID = nSessionID;
			aMsgPacket.pMeta = std::move(pMeta);
			aMsgPacket.nMsgID = nMsgID;
			aMsgPacket.pMsg = std::move(pMsg);
			pOwner->CacheMessage(aMsgPacket, nSessionID);
		}
		else
		{
			LOG_ERROR << "Failed msg.parse() msgID=" << nMsgID
				<< " msgName=" << pMsg->GetTypeName();
		}
	}
	else
	{
		pOwner->OnMissMessage(nMsgID, s.data(), s.size(), nSessionID, pMeta);
		pOwner->OnTransfer(nMsgID, pBuf->length() ? pBuf->data() - pHeader->nTotalLen : pBuf->data(), pHeader->nTotalLen, nSessionID, pMeta);
	}

	return true;
}
