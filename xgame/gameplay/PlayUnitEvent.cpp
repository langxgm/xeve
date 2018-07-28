
#include "PlayUnit.h"

void PlayUnit::OnCreate(int64_t nPlayerGUID)
{
	m_bAlive = true;
}

void PlayUnit::OnDestroy()
{
	m_bAlive = false;
}
