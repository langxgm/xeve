//------------------------------------------------------------------------
// * @filename: PlayUnit.h
// *
// * @brief: 玩法单元
// *
// * @author: XGM
// * @date: 2018/06/07
//------------------------------------------------------------------------
#pragma once

#include "xgame/state/StateMachine.h"
#include "xgame/gameplayer/GamePlayerCollection.h"

#include <set>
#include <vector>
#include <memory>
#include <stdint.h>

//typedef std::unique_ptr<> GamePlayStateMachinePtr;

class GamePlay;

class PlayUnit
{
public:
	PlayUnit();
	virtual ~PlayUnit();
public:
	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(int64_t nSN, GamePlay* pGamePlay, uint32_t nMinFighterNum, uint32_t nMaxFighterNum, uint32_t nMaxObserverNum);

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------
	// 添加战斗者
	//------------------------------------------------------------------------
	virtual bool AddFighter(int64_t nPlayerGUID, const GamePlayerPtr& pPlayer);

	//------------------------------------------------------------------------
	// 添加观战者
	//------------------------------------------------------------------------
	virtual bool AddObserver(int64_t nPlayerGUID, const GamePlayerPtr& pPlayer);

	//------------------------------------------------------------------------
	// 删除战斗者
	//------------------------------------------------------------------------
	virtual bool DelFighter(int64_t nPlayerGUID);

	//------------------------------------------------------------------------
	// 删除观战者
	//------------------------------------------------------------------------
	virtual bool DelObserver(int64_t nPlayerGUID);

	//------------------------------------------------------------------------
	// 删除玩家(战斗者+观战者)
	//------------------------------------------------------------------------
	virtual bool DelPlayer(int64_t nPlayerGUID);

	//------------------------------------------------------------------------
	// 是不是战斗者
	//------------------------------------------------------------------------
	virtual bool IsFighter(int64_t nPlayerGUID);

	//------------------------------------------------------------------------
	// 是不是观战者
	//------------------------------------------------------------------------
	virtual bool IsObserver(int64_t nPlayerGUID);

	//------------------------------------------------------------------------
	// 获得战斗者
	//------------------------------------------------------------------------
	virtual GamePlayerPtr GetFighter(int64_t nPlayerGUID);

	template<typename T>
	std::shared_ptr<T> GetFighter_cast(int64_t nPlayerGUID)
	{
		return std::static_pointer_cast<T>(GetFighter(nPlayerGUID));
	}

	//------------------------------------------------------------------------
	// 获得观战者
	//------------------------------------------------------------------------
	virtual GamePlayerPtr GetObserver(int64_t nPlayerGUID);

	template<typename T>
	std::shared_ptr<T> GetObserver_cast(int64_t nPlayerGUID)
	{
		return std::static_pointer_cast<T>(GetObserver(nPlayerGUID));
	}

	//------------------------------------------------------------------------
	// 获得玩家(战斗者+观战者)
	//------------------------------------------------------------------------
	virtual GamePlayerPtr GetPlayer(int64_t nPlayerGUID);

	template<typename T>
	std::shared_ptr<T> GetPlayer_cast(int64_t nPlayerGUID)
	{
		return std::static_pointer_cast<T>(GetPlayer(nPlayerGUID));
	}

	//------------------------------------------------------------------------
	// 获得战斗人数
	//------------------------------------------------------------------------
	virtual uint32_t GetFighterNum();

	//------------------------------------------------------------------------
	// 获得观战人数
	//------------------------------------------------------------------------
	virtual uint32_t GetObserverNum();

	//------------------------------------------------------------------------
	// 获得所有人数(战斗+观战)
	//------------------------------------------------------------------------
	virtual uint32_t GetPlayerNum();

	//------------------------------------------------------------------------
	// 获得所有战斗者
	//------------------------------------------------------------------------
	virtual std::vector<GamePlayerPtr>& GetAllFighter();

	//------------------------------------------------------------------------
	// 获得所有观战者
	//------------------------------------------------------------------------
	virtual std::vector<GamePlayerPtr>& GetAllObserver();

	//------------------------------------------------------------------------
	// 获得所有人(战斗+观战)
	//------------------------------------------------------------------------
	virtual std::vector<GamePlayerPtr> GetAllPlayer();

	//------------------------------------------------------------------------
	// 获得另一个战斗者
	//------------------------------------------------------------------------
	virtual GamePlayerPtr GetAnotherFighter(int64_t nMyGUID);

	template<typename T>
	std::shared_ptr<T> GetAnotherFighter_cast(int64_t nMyGUID)
	{
		return std::static_pointer_cast<T>(GetAnotherFighter(nMyGUID));
	}

	//------------------------------------------------------------------------
	// 获得另一个战斗者PlayerGUID
	//------------------------------------------------------------------------
	virtual int64_t GetAnotherFighterGUID(int64_t nMyGUID);

	//------------------------------------------------------------------------
	// 获得另一个战斗者SessionID
	//------------------------------------------------------------------------
	virtual int64_t GetAnotherFighterSessionID(int64_t nMyGUID);

	//------------------------------------------------------------------------
	// 获得所有战斗者的PlayerGUID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllFighterGUID(int64_t nExcludeGUID = 0);

	//------------------------------------------------------------------------
	// 获得所有战斗者的SessionID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllFighterSessionID(int64_t nExcludeGUID = 0);

	//------------------------------------------------------------------------
	// 获得所有观战者的PlayerGUID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllObserverGUID(int64_t nExcludeGUID = 0);

	//------------------------------------------------------------------------
	// 获得所有观战者的SessionID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllObserverSessionID(int64_t nExcludeGUID = 0);

	//------------------------------------------------------------------------
	// 获得所有人PlayerGUID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllPlayerGUID(int64_t nExcludeGUID = 0);

	//------------------------------------------------------------------------
	// 获得所有人SessionID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllSessionID(int64_t nExcludeGUID = 0);

	//------------------------------------------------------------------------
	// 获得不重复的SessionID
	//------------------------------------------------------------------------
	virtual std::set<int64_t> GetAllSessionID_set(int64_t nExcludeGUID = 0);

public:
	int64_t GetSN() const { return m_nSN; }
	void SetSN(int64_t nSN) { m_nSN = nSN; }

	GamePlay* GetGamePlay() const { return m_pGamePlay; }

	uint32_t GetMinFighterNum() const { return m_nMinFighterNum; }
	void SetMinFighterNum(uint32_t nMinFighterNum) { m_nMinFighterNum = nMinFighterNum; }

	uint32_t GetMaxFighterNum() const { return m_Fighters.GetPlayerLimit(); }
	void SetMaxFighterNum(uint32_t nMaxFighterNum) { m_Fighters.SetPlayerLimit(nMaxFighterNum); }

	uint32_t GetMaxObserverNum() const { return m_Observers.GetPlayerLimit(); }
	void SetMaxObserverNum(uint32_t nMaxObserverNum) { m_Observers.SetPlayerLimit(nMaxObserverNum); }

	int64_t GetCreateTime() const { return m_nCreateTime; }

	int64_t GetCreatorGUID() const { return m_nCreatorGUID; }
	void SetCreatorGUID(int64_t nPlayerGUID) { m_nCreatorGUID = nPlayerGUID; }

	int64_t GetOwnerGUID() const { return m_nOwnerGUID; }
	void SetOwnerGUID(int64_t nPlayerGUID) { m_nOwnerGUID = nPlayerGUID; }

protected:
	// 序列号
	int64_t m_nSN = 0;

	// 状态机
	//GamePlayStateMachine m_pStateMachine;

	// 游戏玩法
	GamePlay* m_pGamePlay = nullptr;

	// 战斗人员列表
	GamePlayerVector<int64_t, GamePlayerPtr> m_Fighters;

	// 观战人员列表
	GamePlayerVector<int64_t, GamePlayerPtr> m_Observers;

	// 启动游戏的最少战斗人数
	uint32_t m_nMinFighterNum = 0;

	// 创建时间
	int64_t m_nCreateTime = 0;

	// 创建者的PlayerGUID
	int64_t m_nCreatorGUID = 0;
	// 所有者的PlayerGUID
	int64_t m_nOwnerGUID = 0;
};
