
#include "FrameRoom.h"
#include "FrameGame.h"
#include "FrameSequence.h"
#include "FrameExecutor.h"
#include "Frame.h"

#include <assert.h>

FrameRoom::FrameRoom()
{

}

FrameRoom::~FrameRoom()
{
	delete m_pFrameExecutor;
	delete m_pFrameSequence;
}

void FrameRoom::Init(FrameGame* pFrameGame)
{
	m_pFrameGame = pFrameGame;

	m_pFrameSequence = pFrameGame->CreateFrameSequence();
	m_pFrameExecutor = pFrameGame->CreateFrameExecutor();

	m_pFrameExecutor->Init(m_pFrameSequence, this,
		m_pFrameGame->GetFrameInterval(), m_pFrameGame->GetFrameSpeed());
}

void FrameRoom::Update()
{
	m_pFrameExecutor->Update();
}

Frame* FrameRoom::MakeFrame(int32_t nFrameID)
{
	Frame* pNewFrame = m_pFrameGame->CreateFrame();
	pNewFrame->Init(nFrameID, this);

	if (m_pFrameSequence->AddFrame(pNewFrame) == false)
	{
		delete pNewFrame;
		pNewFrame = nullptr;
	}
	return pNewFrame;
}

Frame* FrameRoom::GetCurrentFrame()
{
	int32_t nFrameID = m_pFrameExecutor->GetFrameID();

	Frame* pFrame = m_pFrameSequence->GetFrame(nFrameID);
	if (pFrame == nullptr)
	{
#if 0
		pFrame = MakeFrame(nFrameID);
#else
		assert(false && "current frame is nullptr");
#endif
}
	return pFrame;
}
