//------------------------------------------------------------------------
// * @filename: RoomBase.h
// *
// * @brief: 房间
// *
// * @author: XGM
// * @date: 2020/09/10
//------------------------------------------------------------------------
#pragma once

#include <map>
#include <vector>
#include <stdint.h>

class GamePlayer;
class RoomContainer;

class RoomBase
{
public:
	RoomBase();
	virtual ~RoomBase();
	virtual void CleanUp();

public:
	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(int64_t nRoomID, RoomContainer* pContainer);

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------
	// 增加玩家
	//------------------------------------------------------------------------
	virtual bool AddPlayer(int64_t nGUID, GamePlayer* pPlayer);

	//------------------------------------------------------------------------
	// 删除玩家
	//------------------------------------------------------------------------
	virtual bool RemovePlayer(int64_t nGUID);
	virtual void RemoveAll();

	//------------------------------------------------------------------------
	// 获得玩家
	//------------------------------------------------------------------------
	virtual GamePlayer* GetPlayer(int64_t nGUID);
	virtual const GamePlayer* PeekPlayer(int64_t nGUID) const;

	virtual GamePlayer* PlayerAt(int32_t nIndex);

	//------------------------------------------------------------------------
	// 是否存在某个玩家
	//------------------------------------------------------------------------
	virtual bool IsExistPlayer(int64_t nGUID) const;

	//------------------------------------------------------------------------
	// 获得玩家数量
	//------------------------------------------------------------------------
	virtual int32_t GetPlayerSize() const;

protected:
	//------------------------------------------------------------------------
	// 释放玩家内存
	//------------------------------------------------------------------------
	virtual void OnReleasePlayer(GamePlayer* pPlayer);

public:
	//------------------------------------------------------------------------
	// 创建
	//------------------------------------------------------------------------
	virtual void OnCreate(int64_t nPlayerGUID);

	//------------------------------------------------------------------------
	// 销毁
	//------------------------------------------------------------------------
	virtual void OnDestroy();

	//------------------------------------------------------------------------
	// 能不能关闭
	//------------------------------------------------------------------------
	virtual bool IsCanClose(int64_t nPlayerGUID) const;

	//------------------------------------------------------------------------
	// 关闭
	//------------------------------------------------------------------------
	virtual void Close(int64_t nPlayerGUID);

	//------------------------------------------------------------------------
	// 有人进来
	//------------------------------------------------------------------------
	virtual void OnPlayerEnter(int64_t nPlayerGUID) {}

	//------------------------------------------------------------------------
	// 有人离开
	//------------------------------------------------------------------------
	virtual void OnPlayerLeave(int64_t nPlayerGUID) {}

	//------------------------------------------------------------------------
	// 有人上线
	//------------------------------------------------------------------------
	virtual void OnPlayerOnline(int64_t nPlayerGUID) {}

	//------------------------------------------------------------------------
	// 有人离线
	//------------------------------------------------------------------------
	virtual void OnPlayerOffline(int64_t nPlayerGUID) {}

public:
	RoomContainer* GetContainer() const { return m_pContainer; }
	void SetContainer(RoomContainer* pContainer) { m_pContainer = pContainer; }

	int64_t GetRoomID() const { return m_nRoomID; }
	void SetRoomID(int64_t nRoomID) { m_nRoomID = nRoomID; }

	bool IsAlive() const { return m_bAlive; }
	void SetAlive(bool bAlive) { m_bAlive = bAlive; }

	int64_t GetCreateTime() const { return m_nCreateTime; }
	void SetCreateTime(int64_t nTime) { m_nCreateTime = nTime; }

	int64_t GetCreatorGUID() const { return m_nCreatorGUID; }
	void SetCreatorGUID(int64_t nPlayerGUID) { m_nCreatorGUID = nPlayerGUID; }

	int64_t GetOwnerGUID() const { return m_nOwnerGUID; }
	void SetOwnerGUID(int64_t nPlayerGUID) { m_nOwnerGUID = nPlayerGUID; }

protected:
	// 所属的容器
	RoomContainer* m_pContainer = nullptr;

	// 房间ID
	int64_t m_nRoomID = 0;

	// 存活状态
	bool m_bAlive = false;

	// 创建时间
	int64_t m_nCreateTime = 0;

	// 创建者的GUID
	int64_t m_nCreatorGUID = 0;

	// 所有者的GUID
	int64_t m_nOwnerGUID = 0;

	// 玩家映射表 <GUID, 玩家>
	std::map<int64_t, GamePlayer*> m_mapPlayer;

	// 玩家列表
	std::vector<GamePlayer*> m_vecPlayer;
};
