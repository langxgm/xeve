//------------------------------------------------------------------------
// * @filename: NonCopyable.h
// *
// * @brief: 禁止拷贝
// *
// * @author: XGM
// * @date: 2016/12/12
//------------------------------------------------------------------------
#pragma once

class NonCopyable
{
public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
};
