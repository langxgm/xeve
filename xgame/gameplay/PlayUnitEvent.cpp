
#include "PlayUnit.h"
#include "GamePlay.h"

#include "xbase/TimeUtil.h"

void PlayUnit::OnCreate(int64_t nPlayerGUID)
{
	m_bAlive = true;

	m_nCreateTime = TimeUtil::GetCurrentTimeMillis();

	m_nCreatorGUID = nPlayerGUID;
	m_nOwnerGUID = nPlayerGUID;
}

void PlayUnit::OnDestroy()
{
	m_bAlive = false;

	GetGamePlay()->DelUnit(GetSN());
}

bool PlayUnit::IsCanClose(int64_t nPlayerGUID)
{
	return m_nOwnerGUID == nPlayerGUID;
}

void PlayUnit::Close(int64_t nPlayerGUID)
{
	OnDestroy();
}
