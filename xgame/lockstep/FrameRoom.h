//------------------------------------------------------------------------
// * @filename: FrameRoom.h
// *
// * @brief: 帧同步房间
// *
// * @author: XGM
// * @date: 2019/01/08
//------------------------------------------------------------------------
#pragma once

#include <stdint.h>

class FrameGame;
class FrameSequence;
class FrameExecutor;
class Frame;

class FrameRoom
{
public:
	FrameRoom();
	virtual ~FrameRoom();

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(FrameGame* pFrameGame);

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------
	// 创建帧
	//------------------------------------------------------------------------
	virtual Frame* MakeFrame(int32_t nFrameID);

	//------------------------------------------------------------------------
	// 获得当前帧
	//------------------------------------------------------------------------
	virtual Frame* GetCurrentFrame();

public:
	FrameGame* GetFrameGame() { return m_pFrameGame; }
	FrameSequence* GetFrameSequence() { return m_pFrameSequence; }
	FrameExecutor* GetFrameExecutor() { return m_pFrameExecutor; }

	void* GetOwner() { return m_pOwner; }
	void SetOwner(void* pOwner) { m_pOwner = pOwner; }

protected:
	// 帧同步游戏
	FrameGame* m_pFrameGame = nullptr;

	// 帧序列
	FrameSequence* m_pFrameSequence = nullptr;

	// 帧执行器
	FrameExecutor* m_pFrameExecutor = nullptr;

	// 房间持有者
	void* m_pOwner = nullptr;
};
