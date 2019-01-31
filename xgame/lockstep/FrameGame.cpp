
#include "FrameGame.h"
#include "FrameRoom.h"

FrameGame::FrameGame()
{

}

FrameGame::~FrameGame()
{

}

FrameRoom* FrameGame::CreateFrameRoom()
{
	FrameRoom* pFrameRoom = new FrameRoom();
	if (pFrameRoom)
	{
		pFrameRoom->Init(this);
	}
	return pFrameRoom;
}

void FrameGame::Init(const std::string& strName)
{
	m_strName = strName;
}
