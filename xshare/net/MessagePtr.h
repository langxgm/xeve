//------------------------------------------------------------------------
// * @filename: MessagePtr.h
// *
// * @brief: 消息指针
// *
// * @author: XGM
// * @date: 2017/08/13
//------------------------------------------------------------------------
#pragma once

#include <memory>

#ifdef _WIN32
#pragma warning(disable: 4251) // protobuf
#endif

namespace google {
	namespace protobuf {
		class Message;
	}
}

// 消息指针
typedef std::shared_ptr<::google::protobuf::Message> MessagePtr;
