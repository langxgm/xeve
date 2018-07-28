
#include "PlayUnit.h"
#include "xgame/gameplayer/GamePlayer.h"

PlayUnit::PlayUnit()
{

}

PlayUnit::~PlayUnit()
{
	m_Observers.RemoveAllPlayer();
	m_Fighters.RemoveAllPlayer();
}

void PlayUnit::Init(int64_t nSN, GamePlay* pGamePlay, uint32_t nMinFighterNum, uint32_t nMaxFighterNum, uint32_t nMaxObserverNum)
{
	m_nSN = nSN;
	m_pGamePlay = pGamePlay;
	m_Fighters.Init(nMaxFighterNum);
	m_Observers.Init(nMaxObserverNum);
	m_nMinFighterNum = nMinFighterNum;
}

void PlayUnit::Update()
{
	//m_pStateMachine->Update();
}

bool PlayUnit::AddFighter(int64_t nPlayerGUID, const GamePlayerPtr& pPlayer)
{
	return m_Fighters.AddPlayer(pPlayer);
}

bool PlayUnit::AddObserver(int64_t nPlayerGUID, const GamePlayerPtr& pPlayer)
{
	return m_Observers.AddPlayer(pPlayer);
}

bool PlayUnit::DelFighter(int64_t nPlayerGUID)
{
	return m_Fighters.DelPlayer(nPlayerGUID);
}

void PlayUnit::RemoveAllFighter()
{
	m_Fighters.RemoveAllPlayer();
}

bool PlayUnit::DelObserver(int64_t nPlayerGUID)
{
	return m_Observers.DelPlayer(nPlayerGUID);
}

void PlayUnit::RemoveAllObserver()
{
	m_Observers.RemoveAllPlayer();
}

bool PlayUnit::DelPlayer(int64_t nPlayerGUID)
{
	return DelFighter(nPlayerGUID) || DelObserver(nPlayerGUID);
}

void PlayUnit::RemoveAllPlayer()
{
	RemoveAllFighter();
	RemoveAllObserver();
}

bool PlayUnit::IsFighter(int64_t nPlayerGUID)
{
	return m_Fighters.IsExistPlayer(nPlayerGUID);
}

bool PlayUnit::IsObserver(int64_t nPlayerGUID)
{
	return m_Observers.IsExistPlayer(nPlayerGUID);
}

bool PlayUnit::IsExistPlayer(int64_t nPlayerGUID)
{
	return IsFighter(nPlayerGUID) || IsObserver(nPlayerGUID);
}

GamePlayerPtr PlayUnit::GetFighter(int64_t nPlayerGUID)
{
	return m_Fighters.GetPlayer(nPlayerGUID);
}

GamePlayerPtr PlayUnit::GetObserver(int64_t nPlayerGUID)
{
	return m_Observers.GetPlayer(nPlayerGUID);
}

GamePlayerPtr PlayUnit::GetPlayer(int64_t nPlayerGUID)
{
	auto pPlayer = GetFighter(nPlayerGUID);
	return pPlayer ? std::move(pPlayer) : std::move(GetObserver(nPlayerGUID));
}

uint32_t PlayUnit::GetFighterNum()
{
	return m_Fighters.GetPlayerNum();
}

uint32_t PlayUnit::GetObserverNum()
{
	return m_Observers.GetPlayerNum();
}

uint32_t PlayUnit::GetPlayerNum()
{
	return m_Fighters.GetPlayerNum() + m_Observers.GetPlayerNum();
}

const std::vector<GamePlayerPtr>& PlayUnit::GetAllFighter()
{
	return m_Fighters.GetAllPlayer();
}

const std::vector<GamePlayerPtr>& PlayUnit::GetAllObserver()
{
	return m_Observers.GetAllPlayer();
}

std::vector<GamePlayerPtr> PlayUnit::GetAllPlayer()
{
	std::vector<GamePlayerPtr> vecPlayer;
	vecPlayer.reserve(m_Fighters.GetPlayerNum() + m_Observers.GetPlayerNum());
	vecPlayer.insert(vecPlayer.end(), m_Fighters.GetAllPlayer().begin(), m_Fighters.GetAllPlayer().end());
	vecPlayer.insert(vecPlayer.end(), m_Observers.GetAllPlayer().begin(), m_Observers.GetAllPlayer().end());
	return std::move(vecPlayer);
}

GamePlayerPtr PlayUnit::GetAnotherFighter(int64_t nMyGUID)
{
	return m_Fighters.GetAnother(nMyGUID);
}

int64_t PlayUnit::GetAnotherFighterGUID(int64_t nMyGUID)
{
	return m_Fighters.GetAnotherGUID(nMyGUID);
}

int64_t PlayUnit::GetAnotherFighterSessionID(int64_t nMyGUID)
{
	return m_Fighters.GetAnotherSessionID(nMyGUID);
}

std::vector<int64_t> PlayUnit::GetAllFighterGUID(int64_t nExcludeGUID /*= 0*/)
{
	return m_Fighters.GetAllPlayerGUID(nExcludeGUID);
}

std::vector<int64_t> PlayUnit::GetAllFighterSessionID(int64_t nExcludeGUID /*= 0*/)
{
	return m_Fighters.GetAllSessionID(nExcludeGUID);
}

std::vector<int64_t> PlayUnit::GetAllObserverGUID(int64_t nExcludeGUID /*= 0*/)
{
	return m_Observers.GetAllPlayerGUID(nExcludeGUID);
}

std::vector<int64_t> PlayUnit::GetAllObserverSessionID(int64_t nExcludeGUID /*= 0*/)
{
	return m_Observers.GetAllSessionID(nExcludeGUID);
}

std::vector<int64_t> PlayUnit::GetAllPlayerGUID(int64_t nExcludeGUID /*= 0*/)
{
	return m_Fighters.GetUnionPlayerGUID(m_Observers, nExcludeGUID);
}

std::vector<int64_t> PlayUnit::GetAllSessionID(int64_t nExcludeGUID /*= 0*/)
{
	return m_Fighters.GetUnionSessionID(m_Observers, nExcludeGUID);
}

std::set<int64_t> PlayUnit::GetAllSessionID_set(int64_t nExcludeGUID /*= 0*/)
{
	return m_Fighters.GetUnionSessionID_set(m_Observers, nExcludeGUID);
}
