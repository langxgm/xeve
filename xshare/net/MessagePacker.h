//------------------------------------------------------------------------
// * @filename: MessagePacker.h
// *
// * @brief: 消息包装器
// *
// * @author: XGM
// * @date: 2018/07/02
//------------------------------------------------------------------------
#pragma once

#include <memory>

namespace google {
	namespace protobuf {
		class Message;
	}
}

namespace evpp {
	class Buffer;
	class TCPConn;
	typedef std::shared_ptr<TCPConn> TCPConnPtr;
}

struct MessageHeader;
class MessageMeta;
typedef std::shared_ptr<MessageMeta> MessageMetaPtr;

class MessageReceiver;

class MessagePacker
{
public:
	virtual ~MessagePacker() {}

	//------------------------------------------------------------------------
	// 创建元数据
	//------------------------------------------------------------------------
	virtual MessageMeta* NewMeta();

	//------------------------------------------------------------------------
	// 打包
	//------------------------------------------------------------------------
	virtual void Pack(MessageReceiver* pOwner, evpp::Buffer* pBuf, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta = nullptr);
	virtual void PackBytes(MessageReceiver* pOwner, evpp::Buffer* pBuf, const void* pMsg, size_t nMsgLen, const MessageMeta* pMeta = nullptr);

	//------------------------------------------------------------------------
	// 解包
	//------------------------------------------------------------------------
	virtual void Unpack(MessageReceiver* pOwner, const evpp::TCPConnPtr& conn, evpp::Buffer* pBuf);

protected:
	//------------------------------------------------------------------------
	// 打包前
	//------------------------------------------------------------------------
	virtual void PackBefore(uint32_t nMsgID, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta) {}

	//------------------------------------------------------------------------
	// 打包后
	//------------------------------------------------------------------------
	virtual void PackAfter(uint32_t nMsgID, const ::google::protobuf::Message* pMsg, const MessageMeta* pMeta) {}

	//------------------------------------------------------------------------
	// 打包消息头
	//------------------------------------------------------------------------
	virtual void PackHeader(MessageReceiver* pOwner, evpp::Buffer* pBuf, MessageHeader* pHeader);

	//------------------------------------------------------------------------
	// 解包消息头
	//------------------------------------------------------------------------
	virtual bool UnpackHeader(MessageReceiver* pOwner, evpp::Buffer* pBuf, MessageHeader* pHeader);

	//------------------------------------------------------------------------
	// 打包元数据
	//------------------------------------------------------------------------
	virtual void PackMeta(MessageReceiver* pOwner, evpp::Buffer* pBuf, uint32_t nMsgID, const MessageMeta* pMeta);

	//------------------------------------------------------------------------
	// 解包元数据
	//------------------------------------------------------------------------
	virtual bool UnpackMeta(MessageReceiver* pOwner, evpp::Buffer* pBuf, const MessageHeader* pHeader, MessageMetaPtr& pMeta);

	//------------------------------------------------------------------------
	// 打包消息体
	//------------------------------------------------------------------------
	virtual void PackBody(MessageReceiver* pOwner, evpp::Buffer* pBuf, const ::google::protobuf::Message* pMsg, const MessageHeader* pHeader);
	virtual void PackBodyBytes(MessageReceiver* pOwner, evpp::Buffer* pBuf, const void* pMsg, size_t nMsgLen, const MessageHeader* pHeader);

	//------------------------------------------------------------------------
	// 解包消息体
	//------------------------------------------------------------------------
	virtual bool UnpackBody(MessageReceiver* pOwner, int64_t nSessionID, evpp::Buffer* pBuf, const MessageHeader* pHeader, MessageMetaPtr& pMeta);
};
