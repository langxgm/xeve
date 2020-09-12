
#include "RoomBase.h"
#include "RoomContainer.h"
#include "xgame/player/GamePlayer.h"

#include "xbase/TimeUtil.h"

#include <assert.h>

void RoomBase::OnReleasePlayer(GamePlayer* pPlayer)
{
	assert(pPlayer);
	delete pPlayer;
}

void RoomBase::OnCreate(int64_t nPlayerGUID)
{
	m_bAlive = true;

	m_nCreateTime = TimeUtil::GetCurrentTimeMillis();

	m_nCreatorGUID = nPlayerGUID;
	m_nOwnerGUID = nPlayerGUID;
}

void RoomBase::OnDestroy()
{
	m_bAlive = false;

	m_pContainer->RemoveRoom(m_nRoomID);
}

bool RoomBase::IsCanClose(int64_t nPlayerGUID) const
{
	return m_nOwnerGUID == nPlayerGUID;
}

void RoomBase::Close(int64_t nPlayerGUID)
{
	OnDestroy();
}
