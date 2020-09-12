//------------------------------------------------------------------------
// * @filename: RoomContainer.h
// *
// * @brief: 房间容器
// *
// * @author: XGM
// * @date: 2020/09/10
//------------------------------------------------------------------------
#pragma once

#include <map>
#include <list>
#include <string>
#include <stdint.h>

class RoomBase;
class GamePlayerFactory;
class GamePlayerActorManager;

class RoomContainer
{
public:
	RoomContainer();
	virtual ~RoomContainer();
	virtual void CleanUp();

protected:
	//------------------------------------------------------------------------
	// 内部创建房间
	//------------------------------------------------------------------------
	virtual RoomBase* CreateRoom() = 0;

	//------------------------------------------------------------------------
	// 回收房间
	//------------------------------------------------------------------------
	virtual void RecycleRoom(RoomBase* pRoom);

	//------------------------------------------------------------------------
	// 创建玩家的工厂
	//------------------------------------------------------------------------
	virtual GamePlayerFactory* CreatePlayerFactory() { return nullptr; }

	//------------------------------------------------------------------------
	// 创建玩家扮演的角色管理
	//------------------------------------------------------------------------
	virtual GamePlayerActorManager* CreateActorManager();

	//------------------------------------------------------------------------
	// 生成房间号
	//------------------------------------------------------------------------
	virtual int64_t NextRoomNumber();

public:
	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(const std::string& strName, int32_t nCapacity);

	//------------------------------------------------------------------------
	// 注册状态
	//------------------------------------------------------------------------
	virtual void OnRegisterState() {}

	//------------------------------------------------------------------------
	// 注册玩家扮演的角色
	//------------------------------------------------------------------------
	virtual void OnRegisterPlayerActor() {}

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------
	// 增加房间
	//------------------------------------------------------------------------
	virtual RoomBase* AddRoom(int64_t nRoomID);

	//------------------------------------------------------------------------
	// 删除房间
	//------------------------------------------------------------------------
	virtual bool RemoveRoom(int64_t nRoomID);

	//------------------------------------------------------------------------
	// 获得房间
	//------------------------------------------------------------------------
	virtual RoomBase* GetRoom(int64_t nRoomID);
	virtual const RoomBase* PeekRoom(int64_t nRoomID) const;

	//------------------------------------------------------------------------
	// 获得玩家的工厂
	//------------------------------------------------------------------------
	GamePlayerFactory* GetPlayerFactory() { return m_pPlayerFactory; }

	//------------------------------------------------------------------------
	// 获得玩家扮演的角色管理
	//------------------------------------------------------------------------
	GamePlayerActorManager* GetActorManager() { return m_pActorManager; }

public:
	const std::string& GetName() const { return m_strName; }

	int32_t GetCapacity() const { return  m_nCapacity; }
	int32_t GetRoomSize() const { return static_cast<int32_t>(m_mapRoom.size()); }
	int32_t GetAliveSize() const { return static_cast<int32_t>(m_listAliveRoom.size()); }

protected:
	// 容器名称
	std::string m_strName;

	// 容量
	int32_t m_nCapacity = 0;

	// 自增的房间号
	int64_t m_nRoomNumber = 0;

	// 房间映射表 <房间ID, 房间>
	std::map<int64_t, RoomBase*> m_mapRoom;

	// 存活的房间列表
	std::list<RoomBase*> m_listAliveRoom;

	// 创建玩家的工厂
	GamePlayerFactory* m_pPlayerFactory = nullptr;

	// 玩家扮演的角色管理
	GamePlayerActorManager* m_pActorManager = nullptr;
};
