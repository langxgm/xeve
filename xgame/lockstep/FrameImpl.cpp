
#include "FrameImpl.h"

void FrameBase::Init(int32_t nFrameID, FrameRoom* pFrameRoom)
{
	m_nFrameID = nFrameID;
	m_pFrameRoom = pFrameRoom;
}

int32_t FrameBase::GetFrameID()
{
	return m_nFrameID;
}

FrameRoom* FrameBase::GetFrameRoom()
{
	return m_pFrameRoom;
}
