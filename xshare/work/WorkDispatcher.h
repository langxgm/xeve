//------------------------------------------------------------------------
// * @filename: WorkDispatcher.h
// *
// * @brief: 工作分配器
// *
// * @author: XGM
// * @date: 2017/08/17
//------------------------------------------------------------------------
#pragma once

#include "WorkerPool.h"

#include <evpp/event_loop.h>

template<bool _sync>
class WorkDispatcher
{
public:
	//------------------------------------------------------------------------
	// 获得同步工作者
	//------------------------------------------------------------------------
	static evpp::EventLoop* SyncWorker()
	{
		return WorkerPool::Me()->Sync();
	}

	//------------------------------------------------------------------------
	// 获得异步工作者
	//------------------------------------------------------------------------
	static evpp::EventLoop* AsyncWorker()
	{
		return WorkerPool::Me()->Async();
	}

	//------------------------------------------------------------------------
	// 用HASH值获得异步工作者
	//------------------------------------------------------------------------
	static evpp::EventLoop* AsyncWorkerWithHash(uint64_t nHash)
	{
		return WorkerPool::Me()->AsyncWithHash(nHash);
	}

	//------------------------------------------------------------------------
	// 获得工作者
	//------------------------------------------------------------------------
	static evpp::EventLoop* Worker(bool sync)
	{
		return sync ? SyncWorker() : AsyncWorker();
	}

public:
	//------------------------------------------------------------------------
	// 默认构造
	//------------------------------------------------------------------------
	WorkDispatcher()
		: m_pWorker(Worker(_sync))
	{
	}

	//------------------------------------------------------------------------
	// 用HASH值构造工作者
	//------------------------------------------------------------------------
	explicit WorkDispatcher(uint64_t nHash)
		: m_pWorker(_sync ? SyncWorker() : AsyncWorkerWithHash(nHash))
	{
	}

private:
	// 工作者指针
	evpp::EventLoop* m_pWorker = nullptr;

public:
	template<class _Func_t>
	evpp::InvokeTimerPtr RunAfter(double delay_ms, _Func_t&& f)
	{
		return m_pWorker->RunAfter(delay_ms, std::forward<_Func_t>(f));
	}

	template<class _Func_t>
	evpp::InvokeTimerPtr RunAfter(evpp::Duration delay, _Func_t&& f)
	{
		return m_pWorker->RunAfter(delay, std::forward<_Func_t>(f));
	}

	// RunEvery executes Functor f every period interval time.
	template<class _Func_t>
	evpp::InvokeTimerPtr RunEvery(evpp::Duration interval, _Func_t&& f)
	{
		return m_pWorker->RunEvery(interval, std::forward<_Func_t>(f));
	}

	template<class _Func_t>
	void RunInLoop(_Func_t&& handler)
	{
		m_pWorker->RunInLoop(std::forward<_Func_t>(handler));
	}

	template<class _Func_t>
	void QueueInLoop(_Func_t&& handler)
	{
		m_pWorker->QueueInLoop(std::forward<_Func_t>(handler));
	}
};

typedef WorkDispatcher<true>  WorkDispatcherSync;  // 线程同步的工作分配器
typedef WorkDispatcher<false> WorkDispatcherAsync; // 线程异步的工作分配器
