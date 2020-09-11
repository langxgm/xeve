//------------------------------------------------------------------------
// * @filename: GamePlayerCollection.h
// *
// * @brief: 玩家集合
// *
// * @author: XGM
// * @date: 2018/06/07
//------------------------------------------------------------------------
#pragma once

#include "GamePlayer.h"

#include <set>
#include <map>
#include <vector>
#include <memory>
#include <stdint.h>
#include <assert.h>

class GamePlayer;
typedef std::shared_ptr<GamePlayer> GamePlayerPtr;

template<typename player_key_type, typename player_value_type>
class GamePlayerCollection
{
public:
	using key_type = player_key_type;
	using value_type = player_value_type;
public:
	GamePlayerCollection() {}
	virtual ~GamePlayerCollection() {}

	//------------------------------------------------------------------------
	// 初始化
	//------------------------------------------------------------------------
	virtual void Init(uint32_t nPlayerLimit = UINT32_MAX)
	{
		m_nPlayerLimit = nPlayerLimit;
	}

	//------------------------------------------------------------------------
	// 增加玩家
	//------------------------------------------------------------------------
	virtual bool InsertPlayer(const key_type& key, const value_type& value) = 0;

	//------------------------------------------------------------------------
	// 增加玩家
	//------------------------------------------------------------------------
	virtual bool AddPlayer(const value_type& value) = 0;

	//------------------------------------------------------------------------
	// 删除玩家
	//------------------------------------------------------------------------
	virtual bool DelPlayer(const key_type& key) = 0;

	//------------------------------------------------------------------------
	// 删除所有玩家
	//------------------------------------------------------------------------
	virtual void RemoveAllPlayer() = 0;

	//------------------------------------------------------------------------
	// 获得玩家
	//------------------------------------------------------------------------
	virtual value_type GetPlayer(const key_type& key) = 0;

	//------------------------------------------------------------------------
	// 是否存在玩家
	//------------------------------------------------------------------------
	virtual bool IsExistPlayer(const key_type& key) = 0;

	//------------------------------------------------------------------------
	// 获得玩家数量
	//------------------------------------------------------------------------
	virtual uint32_t GetPlayerNum() = 0;

	//------------------------------------------------------------------------
	// 玩家数量上限
	//------------------------------------------------------------------------
	virtual uint32_t GetPlayerLimit() const { return m_nPlayerLimit; }
	virtual void SetPlayerLimit(uint32_t nPlayerLimit) { m_nPlayerLimit = nPlayerLimit; }

	//------------------------------------------------------------------------
	// 是否满员
	//------------------------------------------------------------------------
	virtual bool IsFull() { return GetPlayerNum() >= GetPlayerLimit(); }

	//------------------------------------------------------------------------
	// 获得所有人PlayerGUID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllPlayerGUID(int64_t nExcludeGUID = 0) = 0;

	//------------------------------------------------------------------------
	// 获得所有人SessionID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllSessionID(int64_t nExcludeGUID = 0) = 0;

	//------------------------------------------------------------------------
	// 获得不重复的SessionID
	//------------------------------------------------------------------------
	virtual std::set<int64_t> GetAllSessionID_set(int64_t nExcludeGUID = 0) = 0;

protected:
	// 玩家人数限制
	uint32_t m_nPlayerLimit = UINT32_MAX;
};

template<typename player_key_type = int64_t, typename player_value_type = GamePlayerPtr>
class GamePlayerMapping : public GamePlayerCollection<player_key_type, player_value_type>
{
public:
	using mybase = GamePlayerCollection<player_key_type, player_value_type>;
	using key_type = typename mybase::key_type;
	using value_type = typename mybase::value_type;
	typedef std::map<key_type, value_type> map_type;
public:
	GamePlayerMapping() {}
	virtual ~GamePlayerMapping() {}

	using mybase::GetPlayerLimit;

	//------------------------------------------------------------------------
	// 增加玩家
	//------------------------------------------------------------------------
	virtual bool InsertPlayer(const key_type& key, const value_type& value) override
	{
		if (GetPlayerNum() >= GetPlayerLimit())
		{
			return false; // 人满了
		}

		if (m_mapPlayers.find(key) == m_mapPlayers.end())
		{
			m_mapPlayers.insert(std::make_pair(key, value));
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	// 增加玩家
	//------------------------------------------------------------------------
	virtual bool AddPlayer(const value_type& value) override
	{
		assert(false);
		return false;
	}

	//------------------------------------------------------------------------
	// 删除玩家
	//------------------------------------------------------------------------
	virtual bool DelPlayer(const key_type& key) override
	{
		auto it = m_mapPlayers.find(key);
		if (it != m_mapPlayers.end())
		{
			m_mapPlayers.erase(it);
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	// 删除所有玩家
	//------------------------------------------------------------------------
	virtual void RemoveAllPlayer()override
	{
		m_mapPlayers.clear();
	}

	//------------------------------------------------------------------------
	// 获得玩家
	//------------------------------------------------------------------------
	virtual value_type GetPlayer(const key_type& key) override
	{
		auto it = m_mapPlayers.find(key);
		if (it != m_mapPlayers.end())
		{
			return it->second;
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	// 是否存在玩家
	//------------------------------------------------------------------------
	virtual bool IsExistPlayer(const key_type& key) override
	{
		auto it = m_mapPlayers.find(key);
		return it != m_mapPlayers.end();
	}

	//------------------------------------------------------------------------
	// 获得玩家数量
	//------------------------------------------------------------------------
	virtual uint32_t GetPlayerNum() override
	{
		return m_mapPlayers.size();
	}

	//------------------------------------------------------------------------
	// 获得所有人PlayerGUID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllPlayerGUID(int64_t nExcludeGUID = 0) override
	{
		std::vector<int64_t> vecGUID;
		int32_t nCapacity = (nExcludeGUID == 0 ? m_mapPlayers.size() : m_mapPlayers.size() - 1);
		if (nCapacity > 0)
		{
			vecGUID.reserve(nCapacity);
			for (auto& it : m_mapPlayers)
			{
				auto& pPlayer = it.second;
				if (pPlayer->GetPlayerGUID() != nExcludeGUID)
				{
					vecGUID.push_back(pPlayer->GetPlayerGUID());
				}
			}
		}
		return std::move(vecGUID);
	}

	//------------------------------------------------------------------------
	// 获得所有人SessionID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllSessionID(int64_t nExcludeGUID = 0) override
	{
		std::vector<int64_t> vecSessionID;
		int32_t nCapacity = (nExcludeGUID == 0 ? m_mapPlayers.size() : m_mapPlayers.size() - 1);
		if (nCapacity > 0)
		{
			vecSessionID.reserve(nCapacity);
			for (auto& it : m_mapPlayers)
			{
				auto& pPlayer = it.second;
				if (pPlayer->GetPlayerGUID() != nExcludeGUID)
				{
					vecSessionID.push_back(pPlayer->GetSessionID());
				}
			}
		}
		return std::move(vecSessionID);
	}

	//------------------------------------------------------------------------
	// 获得不重复的SessionID
	//------------------------------------------------------------------------
	virtual std::set<int64_t> GetAllSessionID_set(int64_t nExcludeGUID = 0) override
	{
		int64_t nExcludeSessionID = 0;
		int32_t nRefCount = 0;
		std::set<int64_t> setSessionID;
		for (auto& it : m_mapPlayers)
		{
			auto& pPlayer = it.second;
			setSessionID.insert(pPlayer->GetSessionID());
			if (pPlayer->GetPlayerGUID() == nExcludeGUID)
			{
				nExcludeSessionID = pPlayer->GetSessionID();
				++nRefCount;
			}
		}
		if (nExcludeSessionID != 0 && nRefCount == 1)
		{
			setSessionID.erase(nExcludeSessionID);
		}
		return std::move(setSessionID);
	}

public:
	// 玩家列表管理<PlayerGUID, 玩家>
	map_type m_mapPlayers;
};

template<typename player_key_type = int64_t, typename player_value_type = GamePlayerPtr>
class GamePlayerVector : public GamePlayerCollection<player_key_type, player_value_type>
{
public:
	using mybase = GamePlayerCollection<player_key_type, player_value_type>;
	using key_type = typename mybase::key_type;
	using value_type = typename mybase::value_type;
	typedef std::vector<value_type> vector_type;
public:
	GamePlayerVector() {}
	virtual ~GamePlayerVector() {}

	using mybase::GetPlayerLimit;

	//------------------------------------------------------------------------
	// 增加玩家
	//------------------------------------------------------------------------
	virtual bool InsertPlayer(const key_type& key, const value_type& value) override
	{
		assert(false);
		return false;
	}

	//------------------------------------------------------------------------
	// 增加玩家
	//------------------------------------------------------------------------
	virtual bool AddPlayer(const value_type& value) override
	{
		if (GetPlayerNum() >= GetPlayerLimit())
		{
			return false; // 人满了
		}

		m_vecPlayers.push_back(value);
		return true;
	}

	//------------------------------------------------------------------------
	// 删除玩家
	//------------------------------------------------------------------------
	virtual bool DelPlayer(const key_type& key) override
	{
		for (auto it = m_vecPlayers.begin(); it != m_vecPlayers.end(); ++it)
		{
			if ((*it)->GetPlayerGUID() == key)
			{
				m_vecPlayers.erase(it);
				return true;
			}
		}
		return false;
	}

	//------------------------------------------------------------------------
	// 删除所有玩家
	//------------------------------------------------------------------------
	virtual void RemoveAllPlayer()override
	{
		m_vecPlayers.clear();
	}

	//------------------------------------------------------------------------
	// 获得玩家
	//------------------------------------------------------------------------
	virtual value_type GetPlayer(const key_type& key) override
	{
		for (auto& pPlayer : m_vecPlayers)
		{
			if (pPlayer->GetPlayerGUID() == key)
			{
				return pPlayer;
			}
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	// 是否存在玩家
	//------------------------------------------------------------------------
	virtual bool IsExistPlayer(const key_type& key) override
	{
		for (auto& pPlayer : m_vecPlayers)
		{
			if (pPlayer->GetPlayerGUID() == key)
			{
				return true;
			}
		}
		return false;
	}

	//------------------------------------------------------------------------
	// 获得玩家数量
	//------------------------------------------------------------------------
	virtual uint32_t GetPlayerNum() override
	{
		return m_vecPlayers.size();
	}

	//------------------------------------------------------------------------
	// 获得所有人PlayerGUID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllPlayerGUID(int64_t nExcludeGUID = 0) override
	{
		std::vector<int64_t> vecGUID;
		int32_t nCapacity = (nExcludeGUID == 0 ? m_vecPlayers.size() : m_vecPlayers.size() - 1);
		if (nCapacity > 0)
		{
			vecGUID.reserve(nCapacity);
			for (auto& pPlayer : m_vecPlayers)
			{
				if (pPlayer->GetPlayerGUID() != nExcludeGUID)
				{
					vecGUID.push_back(pPlayer->GetPlayerGUID());
				}
			}
		}
		return std::move(vecGUID);
	}

	//------------------------------------------------------------------------
	// 获得并集后所有人PlayerGUID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetUnionPlayerGUID(const GamePlayerVector<key_type, value_type>& rOther, int64_t nExcludeGUID = 0)
	{
		std::vector<int64_t> vecGUID;
		int32_t nCapacity = m_vecPlayers.size() + rOther.m_vecPlayers.size();
		nCapacity += (nExcludeGUID == 0 ? 0 : -1);
		if (nCapacity > 0)
		{
			vecGUID.reserve(nCapacity);
			for (auto& pPlayer : m_vecPlayers)
			{
				if (pPlayer->GetPlayerGUID() != nExcludeGUID)
				{
					vecGUID.push_back(pPlayer->GetPlayerGUID());
				}
			}
			for (auto& pPlayer : rOther.m_vecPlayers)
			{
				if (pPlayer->GetPlayerGUID() != nExcludeGUID)
				{
					vecGUID.push_back(pPlayer->GetPlayerGUID());
				}
			}
		}
		return std::move(vecGUID);
	}

	//------------------------------------------------------------------------
	// 获得所有人SessionID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetAllSessionID(int64_t nExcludeGUID = 0) override
	{
		std::vector<int64_t> vecSessionID;
		int32_t nCapacity = (nExcludeGUID == 0 ? m_vecPlayers.size() : m_vecPlayers.size() - 1);
		if (nCapacity > 0)
		{
			vecSessionID.reserve(nCapacity);
			for (auto& pPlayer : m_vecPlayers)
			{
				if (pPlayer->GetPlayerGUID() != nExcludeGUID)
				{
					vecSessionID.push_back(pPlayer->GetSessionID());
				}
			}
		}
		return std::move(vecSessionID);
	}

	//------------------------------------------------------------------------
	// 获得并集后所有人SessionID
	//------------------------------------------------------------------------
	virtual std::vector<int64_t> GetUnionSessionID(const GamePlayerVector<key_type, value_type>& rOther, int64_t nExcludeGUID = 0)
	{
		std::vector<int64_t> vecSessionID;
		int32_t nCapacity = m_vecPlayers.size() + rOther.m_vecPlayers.size();
		nCapacity += (nExcludeGUID == 0 ? 0 : -1);
		if (nCapacity > 0)
		{
			vecSessionID.reserve(nCapacity);
			for (auto& pPlayer : m_vecPlayers)
			{
				if (pPlayer->GetPlayerGUID() != nExcludeGUID)
				{
					vecSessionID.push_back(pPlayer->GetSessionID());
				}
			}
			for (auto& pPlayer : rOther.m_vecPlayers)
			{
				if (pPlayer->GetPlayerGUID() != nExcludeGUID)
				{
					vecSessionID.push_back(pPlayer->GetSessionID());
				}
			}
		}
		return std::move(vecSessionID);
	}

	//------------------------------------------------------------------------
	// 获得不重复的SessionID
	//------------------------------------------------------------------------
	virtual std::set<int64_t> GetAllSessionID_set(int64_t nExcludeGUID = 0) override
	{
		int64_t nExcludeSessionID = 0;
		int32_t nRefCount = 0;
		std::set<int64_t> setSessionID;
		for (auto& pPlayer : m_vecPlayers)
		{
			setSessionID.insert(pPlayer->GetSessionID());
			if (pPlayer->GetPlayerGUID() == nExcludeGUID)
			{
				nExcludeSessionID = pPlayer->GetSessionID();
				++nRefCount;
			}
		}
		if (nExcludeSessionID != 0 && nRefCount == 1)
		{
			setSessionID.erase(nExcludeSessionID);
		}
		return std::move(setSessionID);
	}

	//------------------------------------------------------------------------
	// 获得并集后不重复的SessionID
	//------------------------------------------------------------------------
	virtual std::set<int64_t> GetUnionSessionID_set(const GamePlayerVector<key_type, value_type>& rOther, int64_t nExcludeGUID = 0)
	{
		int64_t nExcludeSessionID = 0;
		int32_t nRefCount = 0;
		std::set<int64_t> setSessionID;
		for (auto& pPlayer : m_vecPlayers)
		{
			setSessionID.insert(pPlayer->GetSessionID());
			if (pPlayer->GetPlayerGUID() == nExcludeGUID)
			{
				nExcludeSessionID = pPlayer->GetSessionID();
				++nRefCount;
			}
		}
		for (auto& pPlayer : rOther.m_vecPlayers)
		{
			setSessionID.insert(pPlayer->GetSessionID());
			if (pPlayer->GetPlayerGUID() == nExcludeGUID)
			{
				nExcludeSessionID = pPlayer->GetSessionID();
				++nRefCount;
			}
		}
		if (nExcludeSessionID != 0 && nRefCount == 1)
		{
			setSessionID.erase(nExcludeSessionID);
		}
		return std::move(setSessionID);
	}

	//------------------------------------------------------------------------
	// 获得所有玩家
	//------------------------------------------------------------------------
	virtual vector_type& GetAllPlayer()
	{
		return m_vecPlayers;
	}

	//------------------------------------------------------------------------
	// 获得另一个玩家
	//------------------------------------------------------------------------
	virtual value_type GetAnother(int64_t nMyGUID)
	{
		for (auto& pPlayer : m_vecPlayers)
		{
			if (pPlayer->GetPlayerGUID() != nMyGUID)
			{
				return pPlayer;
			}
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	// 获得另一个玩家PlayerGUID
	//------------------------------------------------------------------------
	virtual int64_t GetAnotherGUID(int64_t nMyGUID)
	{
		for (auto& pPlayer : m_vecPlayers)
		{
			if (pPlayer->GetPlayerGUID() != nMyGUID)
			{
				return pPlayer->GetPlayerGUID();
			}
		}
		return 0;
	}

	//------------------------------------------------------------------------
	// 获得另一个玩家SessionID
	//------------------------------------------------------------------------
	virtual int64_t GetAnotherSessionID(int64_t nMyGUID)
	{
		for (auto& pPlayer : m_vecPlayers)
		{
			if (pPlayer->GetPlayerGUID() != nMyGUID)
			{
				return pPlayer->GetSessionID();
			}
		}
		return 0;
	}

public:
	// 玩家列表
	vector_type m_vecPlayers;
};
