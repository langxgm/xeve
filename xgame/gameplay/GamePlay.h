//------------------------------------------------------------------------
// * @filename: GamePlay.h
// *
// * @brief: 游戏玩法
// *
// * @author: XGM
// * @date: 2018/06/07
//------------------------------------------------------------------------
#pragma once

#include <map>
#include <string>
#include <memory>

class PlayUnit;
typedef std::shared_ptr<PlayUnit> PlayUnitPtr;

class GamePlayerFactory;
typedef std::shared_ptr<GamePlayerFactory> GamePlayerFactoryPtr;

class GamePlayerActorManager;
typedef std::unique_ptr<GamePlayerActorManager> GamePlayerActorManagerPtr;

class GamePlay
{
public:
	GamePlay();
	virtual ~GamePlay();
protected:
	//------------------------------------------------------------------------
	// 内部创建单元
	//------------------------------------------------------------------------
	virtual PlayUnit* CreateUnit() = 0;

	//------------------------------------------------------------------------
	// 创建玩家的工厂
	//------------------------------------------------------------------------
	virtual GamePlayerFactory* CreatePlayerFactory() { return nullptr; }

	//------------------------------------------------------------------------
	// 创建玩家扮演的角色管理
	//------------------------------------------------------------------------
	virtual GamePlayerActorManager* CreateActorManager();

	//------------------------------------------------------------------------
	// 生成序列号
	//------------------------------------------------------------------------
	virtual uint64_t GenSerialNumber();

public:
	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(const std::string& strName, uint32_t nLimit);

	//------------------------------------------------------------------------
	// 注册状态
	//------------------------------------------------------------------------
	virtual void OnRegState() {}

	//------------------------------------------------------------------------
	// 注册玩家扮演的角色
	//------------------------------------------------------------------------
	virtual void OnRegPlayerActor() {}

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update();

	//------------------------------------------------------------------------
	// 增加单元
	//------------------------------------------------------------------------
	virtual PlayUnitPtr AddUnit(int64_t nSN = 0);

	template<typename T>
	std::shared_ptr<T> AddUnit_cast(int64_t nSN = 0)
	{
		return std::static_pointer_cast<T>(AddUnit(nSN));
	}

	//------------------------------------------------------------------------
	// 删除单元
	//------------------------------------------------------------------------
	virtual bool DelUnit(int64_t nSN);

	//------------------------------------------------------------------------
	// 获得单元
	//------------------------------------------------------------------------
	virtual PlayUnitPtr GetUnit(int64_t nSN);

	template<typename T>
	std::shared_ptr<T> GetUnit_cast(int64_t nSN)
	{
		return std::static_pointer_cast<T>(GetUnit(nSN));
	}

	//------------------------------------------------------------------------
	// 获得玩家的工厂
	//------------------------------------------------------------------------
	const GamePlayerFactoryPtr& GetPlayerFactory() { return m_pPlayerFactory; }

	//------------------------------------------------------------------------
	// 获得玩家扮演的角色管理
	//------------------------------------------------------------------------
	const GamePlayerActorManagerPtr& GetActorManager() { return m_pActorManager; }

public:
	const std::string& GetName() { return m_strName; }

	uint32_t GetLimit() { return  m_nLimit; }

protected:
	// 名字
	std::string m_strName;

	// 单元映射表 <唯一ID, 单元>
	std::map<int64_t, PlayUnitPtr> m_mapPlayUnit;

	// 数量限制
	uint32_t m_nLimit = 0;

	// 生成唯一ID的自增序列
	uint64_t m_nGenSerialNumber = 0;

	// 创建玩家的工厂
	GamePlayerFactoryPtr m_pPlayerFactory;

	// 玩家扮演的角色管理
	GamePlayerActorManagerPtr m_pActorManager;
};
