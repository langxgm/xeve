//------------------------------------------------------------------------
// * @filename: FrameImpl.h
// *
// * @brief: 帧实现
// *
// * @author: XGM
// * @date: 2019/01/04
//------------------------------------------------------------------------
#pragma once

#include "Frame.h"

class FrameBase : public Frame
{
public:
	virtual void Init(int32_t nFrameID, FrameRoom* pFrameRoom) override;
	virtual int32_t GetFrameID() override;
	virtual FrameRoom* GetFrameRoom() override;
protected:
	// 当前帧编号
	int32_t m_nFrameID = 0;
	// 帧同步房间
	FrameRoom* m_pFrameRoom = nullptr;
};

// 普通帧
class NormalFrame : public FrameBase
{
public:
	virtual bool IsKeyFrame() override { return false; }
};

// 关键帧
class KeyFrame : public FrameBase
{
public:
	virtual bool IsKeyFrame() override { return true; }
};
