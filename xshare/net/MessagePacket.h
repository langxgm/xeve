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

// 消息包头
struct MessageHeader
{
	uint32_t nTotalLen = 0; // 总长度
	uint32_t nMetaLen = 0; // 元数据长度
	uint32_t nMsgLen = 0; // 消息内容长度

	static constexpr uint32_t GetHeaderByteSize()
	{
		return sizeof(nTotalLen) + sizeof(nMetaLen); // 消息包头占用的网络字节数
	}
};

// 消息元数据
class MessageMeta
{
public:
	virtual ~MessageMeta() {}

	//------------------------------------------------------------------------
	// 元数据基类长度
	//------------------------------------------------------------------------
	virtual uint32_t GetBaseByteSize() const final { return sizeof(m_nMsgID); }

	//------------------------------------------------------------------------
	// 元数据总长度
	//------------------------------------------------------------------------
	virtual uint32_t GetByteSize() const { return sizeof(m_nMsgID); }

public:
	//------------------------------------------------------------------------
	// 获得自定义内容
	//------------------------------------------------------------------------
	virtual ::google::protobuf::Message* GetUserdata() { return nullptr; }

	//------------------------------------------------------------------------
	// 一般为PlayerGUID
	//------------------------------------------------------------------------
	virtual int64_t GetGUID() { return 0; }
	virtual void SetGUID(int64_t nGUID) {}

public:
	uint32_t GetMsgID() const { return m_nMsgID; }
	void SetMsgID(uint32_t nMsgID) { m_nMsgID = nMsgID; }
protected:
	uint32_t m_nMsgID = 0; // 消息ID
};
typedef std::shared_ptr<MessageMeta> MessageMetaPtr;

// 消息包
struct MessagePacket
{
	uint64_t nSessionID = 0; // 会话ID
	MessageMetaPtr pMeta; // 元数据
	uint32_t nMsgID = 0; // 消息ID
	MessagePtr pMsg; // 消息内容
};
