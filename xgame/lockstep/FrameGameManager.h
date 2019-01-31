//------------------------------------------------------------------------
// * @filename: FrameGameManager.h
// *
// * @brief: 帧同步的游戏管理
// *
// * @author: XGM
// * @date: 2019/01/08
//------------------------------------------------------------------------
#pragma once

#include "xbase/Singleton.h"

#include <map>
#include <string>
#include <memory>

class FrameGame;
typedef std::shared_ptr<FrameGame> FrameGamePtr;

class FrameGameManager : public Singleton<FrameGameManager>
{
public:
	typedef std::map<std::string, FrameGamePtr> map_type;
protected:
	friend class Singleton<FrameGameManager>;
	FrameGameManager() {}
	virtual ~FrameGameManager() {}
public:
	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init() {}

	//------------------------------------------------------------------------
	// 更新
	//------------------------------------------------------------------------
	virtual void Update() = 0;

	//------------------------------------------------------------------------
	// 注册游戏
	//------------------------------------------------------------------------
	virtual bool RegGame(const std::string& strName, const FrameGamePtr& pFrameGame)
	{
		auto ret = m_mapGame.insert(std::make_pair(strName, pFrameGame));
		return ret.second;
	}

	//------------------------------------------------------------------------
	// 移除注册
	//------------------------------------------------------------------------
	virtual bool UnregGame(const std::string& strName)
	{
		auto it = m_mapGame.find(strName);
		if (it != m_mapGame.end())
		{
			m_mapGame.erase(it);
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	// 获得游戏
	//------------------------------------------------------------------------
	virtual FrameGamePtr GetGame(const std::string& strName)
	{
		auto it = m_mapGame.find(strName);
		if (it != m_mapGame.end())
		{
			return it->second;
		}
		return nullptr;
	}

protected:
	// 所有游戏
	map_type m_mapGame;
};
