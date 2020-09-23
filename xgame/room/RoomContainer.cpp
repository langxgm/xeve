
#include "RoomContainer.h"
#include "RoomBase.h"
#include "xgame/player/GamePlayerFactory.h"
#include "xgame/player/GamePlayerActorManager.h"

#include <assert.h>

RoomContainer::RoomContainer()
{

}

RoomContainer::~RoomContainer()
{
	CleanUp();
}

void RoomContainer::CleanUp()
{
	m_strName.clear();
	m_nCapacity = 0;
	m_nRoomNumber = 0;

	m_mapRoom.clear();
	for (auto it : m_listAliveRoom)
	{
		RoomBase* pRoom = it;
		assert(pRoom);
		RecycleRoom(pRoom);
	}
	m_listAliveRoom.clear();

	delete m_pPlayerFactory;
	m_pPlayerFactory = nullptr;

	delete m_pActorManager;
	m_pActorManager = nullptr;
}

void RoomContainer::RecycleRoom(RoomBase* pRoom)
{
	assert(pRoom);
	delete pRoom;
}

GamePlayerActorManager* RoomContainer::CreateActorManager()
{
	return new GamePlayerActorManager();
}

int64_t RoomContainer::NextRoomNumber()
{
	return ++m_nRoomNumber;
}

void RoomContainer::Init(const std::string& strName, int32_t nCapacity)
{
	m_strName = strName;
	m_nCapacity = nCapacity;
	m_nRoomNumber = 0;

	// factory
	m_pPlayerFactory = CreatePlayerFactory();
	m_pActorManager = CreateActorManager();

	// register
	OnRegisterState();
	OnRegisterPlayerActor();
}

void RoomContainer::Update()
{
	auto it = m_listAliveRoom.begin();
	for (; it != m_listAliveRoom.end();)
	{
		RoomBase* pRoom = *it;
		if (pRoom->IsAlive())
		{
			pRoom->Update();
			++it;
		}
		else
		{
			assert(pRoom);
			RecycleRoom(pRoom);
			it = m_listAliveRoom.erase(it);
		}
	}
}

RoomBase* RoomContainer::AddRoom(int64_t nRoomID)
{
	int32_t nSize = GetRoomSize();
	if (nSize >= m_nCapacity)
	{
		return nullptr;
	}

	if (nRoomID == 0)
	{
		nRoomID = NextRoomNumber();
	}

	RoomBase* pRoom = CreateRoom();
	if (pRoom == nullptr)
	{
		assert(false && "new room is nullptr");
		return nullptr;
	}

	pRoom->SetRoomID(nRoomID);

	auto ret = m_mapRoom.insert(std::make_pair(nRoomID, pRoom));
	if (ret.second == false)
	{
		RecycleRoom(pRoom);
		return nullptr;
	}
	m_listAliveRoom.push_back(pRoom);
	return pRoom;
}

bool RoomContainer::RemoveRoom(int64_t nRoomID)
{
	auto it = m_mapRoom.find(nRoomID);
	if (it != m_mapRoom.end())
	{
		m_mapRoom.erase(it);
		// 下一帧,在Update中真正删除(delete p)
		return true;
	}
	return false;
}

RoomBase* RoomContainer::GetRoom(int64_t nRoomID)
{
	return const_cast<RoomBase*>(PeekRoom(nRoomID));
}

const RoomBase* RoomContainer::PeekRoom(int64_t nRoomID) const
{
	auto it = m_mapRoom.find(nRoomID);
	if (it != m_mapRoom.end())
	{
		return it->second;
	}
	return nullptr;
}
