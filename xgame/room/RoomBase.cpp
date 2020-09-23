
#include "RoomBase.h"
#include "xgame/player/GamePlayer.h"

#include <algorithm>
#include <assert.h>

RoomBase::RoomBase()
{

}

RoomBase::~RoomBase()
{
	CleanUp();
}

void RoomBase::CleanUp()
{
	m_pContainer = nullptr;
	m_nRoomID = 0;
	m_bAlive = false;
	m_nCreateTime = 0;
	m_nCreatorGUID = 0;
	m_nOwnerGUID = 0;

	m_mapPlayer.clear();
	for (auto it : m_vecPlayer)
	{
		GamePlayer* pPlayer = it;
		assert(pPlayer);
		OnReleasePlayer(pPlayer);
	}
	m_vecPlayer.clear();
}

void RoomBase::Init(int64_t nRoomID, RoomContainer* pContainer)
{
	m_nRoomID = nRoomID;
	m_pContainer = pContainer;
}

void RoomBase::Update()
{

}

bool RoomBase::AddPlayer(int64_t nGUID, GamePlayer* pPlayer)
{
	auto it = m_mapPlayer.find(nGUID);
	if (it == m_mapPlayer.end())
	{
		m_mapPlayer.insert(std::make_pair(nGUID, pPlayer));
		m_vecPlayer.push_back(pPlayer);
		return true;
	}
	return false;
}

bool RoomBase::RemovePlayer(int64_t nGUID)
{
	auto it = m_mapPlayer.find(nGUID);
	if (it != m_mapPlayer.end())
	{
		GamePlayer* pPlayer = it->second;
		assert(pPlayer);
		OnReleasePlayer(pPlayer);
		m_mapPlayer.erase(it);

		auto f = std::find_if(m_vecPlayer.begin(), m_vecPlayer.end(),
			[nGUID](GamePlayer* pPlayer) {
				return pPlayer->GetPlayerGUID() == nGUID;
			});
		if (f != m_vecPlayer.end())
		{
			m_vecPlayer.erase(f);
		}
		return true;
	}
	return false;
}

void RoomBase::RemoveAll()
{
	for (auto it : m_vecPlayer)
	{
		GamePlayer* pPlayer = it;
		assert(pPlayer);
		OnReleasePlayer(pPlayer);
	}
	m_vecPlayer.clear();
	m_mapPlayer.clear();
}

GamePlayer* RoomBase::GetPlayer(int64_t nGUID)
{
	return const_cast<GamePlayer*>(PeekPlayer(nGUID));
}

const GamePlayer* RoomBase::PeekPlayer(int64_t nGUID) const
{
	auto it = m_mapPlayer.find(nGUID);
	if (it != m_mapPlayer.end())
	{
		return it->second;
	}
	return nullptr;
}

GamePlayer* RoomBase::PlayerAt(int32_t nIndex)
{
	int32_t nSize = static_cast<int32_t>(m_vecPlayer.size());
	assert(nIndex >= 0 && nIndex < nSize);
	if (nIndex >= 0 && nIndex < nSize)
	{
		return m_vecPlayer[nIndex];
	}
	return nullptr;
}

bool RoomBase::IsExistPlayer(int64_t nGUID) const
{
	return PeekPlayer(nGUID) != nullptr;
}

int32_t RoomBase::GetPlayerSize() const
{
	return static_cast<int32_t>(m_vecPlayer.size());
}
