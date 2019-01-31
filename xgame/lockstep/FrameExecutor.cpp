
#include "FrameExecutor.h"
#include "FrameSequence.h"
#include "FrameRoom.h"
#include "Frame.h"

#include "xbase/TimeUtil.h"

void FrameExecutor::Init(FrameSequence* pFrameSequence, FrameRoom* pFrameRoom, int32_t nInterval, int32_t nSpeed)
{
	m_nFrameID = 0;
	m_nNextFrameID = 0;
	m_nSpeed = nSpeed;

	m_nNextFrameTime = TimeUtil::GetCurrentTimeMillis();
	m_nFrameInterval = nInterval;

	m_pFrameSequence = pFrameSequence;
	m_pFrameRoom = pFrameRoom;

	m_pFrameRoom->MakeFrame(m_nFrameID);
}

void FrameExecutor::Update()
{
	int64_t nNow = TimeUtil::GetCurrentTimeMillis();
	while (nNow >= m_nNextFrameTime)
	{
		m_nNextFrameTime += m_nFrameInterval;

		m_nFrameID = m_nNextFrameID;
		m_nNextFrameID += m_nSpeed;

		OnTimer();

		m_pFrameRoom->MakeFrame(m_nNextFrameID);
	}
}

void FrameExecutor::OnTimer()
{
	Frame* pCurrent = m_pFrameSequence->GetFrame(m_nFrameID);
	if (pCurrent == nullptr)
	{
		return;
	}

	pCurrent->OnFinish(m_nNextFrameID);
	pCurrent->Broadcast();
}
