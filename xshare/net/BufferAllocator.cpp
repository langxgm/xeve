
#include "BufferAllocator.h"

#include "xbase/TimeUtil.h"

#include <evpp/buffer.h>

#include <algorithm>

BufferAllocator::BufferAllocator(uint32_t nInitCapacity)
{
	for (uint32_t i = 0; i < nInitCapacity; ++i)
	{
		m_listBuffer.push_back(new evpp::Buffer());
	}
}

BufferAllocator::~BufferAllocator()
{
	std::for_each(m_listBuffer.begin(), m_listBuffer.end(),
		[](evpp::Buffer* p) { delete p; });
	m_listBuffer.clear();
}

std::string BufferAllocator::GetAllocInfo()
{
	std::string info;
	info.reserve(128);

	MemState temp = { 0 };
	uint64_t cacheSize = 0;
	{
		std::lock_guard<decltype(m_aMutex)> guard(m_aMutex);
		temp = m_aMemState5m;
		cacheSize = m_listBuffer.size();
	}

	info += "BufferAllocator: {";
	info.append(" interval: ").append(std::to_string(temp.GetIntervalTime()));
	info.append(", newCount: ").append(std::to_string(temp.GetNewCount()));
	info.append(", delCount: ").append(std::to_string(temp.GetDelCount()));
	info.append(", useCount: ").append(std::to_string(temp.Count()));
	info.append(", cacheSize: ").append(std::to_string(cacheSize));
	info += " }";
	return std::move(info);
}

uint64_t BufferAllocator::GetCacheSize()
{
	std::lock_guard<decltype(m_aMutex)> guard(m_aMutex);
	return m_listBuffer.size();
}

BufferAllocator::BufferPtr BufferAllocator::Alloc()
{
	{
		std::lock_guard<decltype(m_aMutex)> guard(m_aMutex);

		m_aMemState5m.Inc();

		// 从缓存取
		if (m_listBuffer.size() > 0)
		{
			evpp::Buffer* pBuffer = m_listBuffer.front();
			m_listBuffer.pop_front();
			return BufferPtr(pBuffer,
				[this](evpp::Buffer* pValue) { Recover(pValue); });
		}
	}
	return BufferPtr(new evpp::Buffer,
		[this](evpp::Buffer* pValue) { Recover(pValue); });
}

void BufferAllocator::Recover(evpp::Buffer* pBuffer)
{
	{
		std::lock_guard<decltype(m_aMutex)> guard(m_aMutex);

		m_aMemState5m.Dec();

		// 5分钟内新分配少于100个,则不缓存
		bool bInCache = true;
		if (m_aMemState5m.IsTimeout())
		{
			if (m_aMemState5m.Count() < 100
				&& m_listBuffer.size() > 1)
			{
				bInCache = false;
			}

			m_aMemState5m.Reset();
		}

		// 缓存,循环利用
		if (bInCache)
		{
			m_listBuffer.push_back(pBuffer);
			pBuffer = nullptr;
		}
	}

	// 删除
	if (pBuffer)
	{
		delete pBuffer;
	}
}

void MemState::Inc()
{
	++m_nNewCount;

	if (m_nStartTime == 0)
	{
		m_nStartTime = TimeUtil::GetCurrentTimeSeconds();
	}
}

void MemState::Dec()
{
	++m_nDelCount;

	if (m_nStartTime == 0)
	{
		m_nStartTime = TimeUtil::GetCurrentTimeSeconds();
	}
}

uint64_t MemState::Count()
{
	if (m_nNewCount > m_nDelCount)
	{
		return m_nNewCount - m_nDelCount;
	}
	return 0;
}

bool MemState::IsTimeout()
{
	auto nNowSeconds = TimeUtil::GetCurrentTimeSeconds();
	return m_nStartTime - nNowSeconds >= m_nIntervalTime;
}

void MemState::Reset()
{
	m_nNewCount = 0;
	m_nDelCount = 0;
	m_nStartTime = 0;
	m_nIntervalTime = 0;
}
