
#include "WorkerPool.h"

#include <evpp/event_loop_thread_pool.h>

WorkerPool::WorkerPool(evpp::EventLoop* pBaseLoop, uint32_t nThreadNum)
	:m_pBaseLoop(pBaseLoop)
	, m_pPool(new evpp::EventLoopThreadPool(pBaseLoop, nThreadNum))
{

}

WorkerPool::~WorkerPool()
{
	assert(m_pPool->IsStopped());
	delete m_pPool;
}

bool WorkerPool::Start()
{
	bool bRet = m_pPool->Start(true);
	if (bRet)
	{
		assert(m_pPool->IsRunning());
	}
	return bRet;
}

void WorkerPool::Stop()
{
	m_pPool->Stop(true);
	assert(m_pPool->IsStopped());
	m_pPool->Join();
}

evpp::EventLoop* WorkerPool::Sync()
{
	return m_pBaseLoop;
}

evpp::EventLoop* WorkerPool::Async()
{
	return m_pPool->GetNextLoop();
}

evpp::EventLoop* WorkerPool::AsyncWithHash(uint64_t nHash)
{
	return m_pPool->GetNextLoopWithHash(nHash);
}

uint32_t WorkerPool::GetAsyncNum()
{
	return m_pPool->thread_num();
}
