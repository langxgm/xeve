
#include "GamePlay.h"
#include "PlayUnit.h"
#include "xgame/gameplayer/GamePlayerFactory.h"
#include "xgame/gameplayer/GamePlayerActorManager.h"

GamePlay::GamePlay()
{

}

GamePlay::~GamePlay()
{
	m_pActorManager.reset();
	m_pPlayerFactory.reset();
	m_listAlivePlayUnit.clear();
	m_mapPlayUnit.clear();
}

GamePlayerActorManager* GamePlay::CreateActorManager()
{
	return new GamePlayerActorManager();
}

uint64_t GamePlay::GenSerialNumber()
{
	return ++m_nGenSerialNumber;
}

void GamePlay::Init(const std::string& strName, uint32_t nLimit)
{
	m_strName = strName;
	m_nLimit = nLimit;
	m_nGenSerialNumber = 0;

	OnRegState();

	m_pPlayerFactory.reset(CreatePlayerFactory());

	m_pActorManager.reset(CreateActorManager());
	OnRegPlayerActor();
}

void GamePlay::Update()
{
	for (auto it = m_listAlivePlayUnit.begin(); it != m_listAlivePlayUnit.end();)
	{
		auto& pPlayUnit = *it;
		if (pPlayUnit->IsAlive())
		{
			pPlayUnit->Update();
			++it;
		}
		else
		{
			it = m_listAlivePlayUnit.erase(it);
		}
	}
}

PlayUnitPtr GamePlay::AddUnit(int64_t nSN /*= 0*/)
{
	if (m_mapPlayUnit.size() >= m_nLimit)
	{
		return nullptr;
	}

	if (nSN == 0)
	{
		nSN = GenSerialNumber();
	}

	PlayUnitPtr pPlayUnit(CreateUnit());

	pPlayUnit->SetSN(nSN);

	// 子类去做Init
	//pPlayUnit->Init();

	auto ret = m_mapPlayUnit.insert(std::make_pair(nSN, pPlayUnit));
	if (ret.second)
	{
		m_listAlivePlayUnit.push_back(pPlayUnit);
		return pPlayUnit;
	}
	return nullptr;
}

bool GamePlay::DelUnit(int64_t nSN)
{
	auto it = m_mapPlayUnit.find(nSN);
	if (it != m_mapPlayUnit.end())
	{
		m_mapPlayUnit.erase(it);
		return true;
	}
	return false;
}

PlayUnitPtr GamePlay::GetUnit(int64_t nSN)
{
	auto it = m_mapPlayUnit.find(nSN);
	if (it != m_mapPlayUnit.end())
	{
		return it->second;
	}
	return nullptr;
}
