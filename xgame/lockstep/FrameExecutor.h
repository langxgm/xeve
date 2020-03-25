//------------------------------------------------------------------------
// * @filename: FrameExecutor.h
// *
// * @brief: 帧执行器
// *
// * @author: XGM
// * @date: 2019/01/04
//------------------------------------------------------------------------
#pragma once

#include <stdint.h>

class FrameSequence;
class FrameRoom;

class FrameExecutor
{
public:
	virtual ~FrameExecutor() {}

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(FrameSequence* pFrameSequence, FrameRoom* pFrameRoom, int32_t nInterval, int32_t nSpeed);

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------
	// 播放一帧
	//------------------------------------------------------------------------
	virtual void OnTimer();

public:
	int32_t GetFrameID() const { return m_nFrameID; }

	int32_t GetNextFrameID() const { return m_nNextFrameID; }
	void SetNextFrameID(int32_t nFrameID) { m_nNextFrameID = nFrameID; }

	int32_t GetSpeed() const { return m_nSpeed; }
	void SetSpeed(int32_t nSpeed) { m_nSpeed = nSpeed; }

private:
	// 当前帧编号
	int32_t m_nFrameID = 0;
	// 下一帧编号
	int32_t m_nNextFrameID = 0;
	// 播放速度
	int32_t m_nSpeed = 1;

	// 下一帧的时间(单位:毫秒)
	int64_t m_nNextFrameTime = 0;
	// 帧间隔(单位:毫秒)
	int64_t m_nFrameInterval = 100;

	// 帧序列
	FrameSequence* m_pFrameSequence = nullptr;
	// 帧同步房间
	FrameRoom* m_pFrameRoom = nullptr;
};
