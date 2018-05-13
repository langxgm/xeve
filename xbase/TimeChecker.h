//------------------------------------------------------------------------
// * @filename: TimeChecker.h
// *
// * @brief: 时间检测器
// *
// * @author: XGM
// * @date: 2017/10/30
//------------------------------------------------------------------------
#pragma once

#include <stdint.h>

class TimeChecker
{
public:
	//------------------------------------------------------------------------
	// 开始
	//------------------------------------------------------------------------
	void Start(int64_t nNow, int64_t nInterval)
	{
		m_bStart = true;
		m_nStartTime = nNow;
		m_nInterval = nInterval;
	}

	//------------------------------------------------------------------------
	// 停止
	//------------------------------------------------------------------------
	void Stop()
	{
		m_bStart = false;
		m_nStartTime = 0;
		m_nInterval = 0;
	}

	//------------------------------------------------------------------------
	// 是否超时
	//------------------------------------------------------------------------
	bool IsTimeout(int64_t nNow)
	{
		if (m_bStart == false)
		{
			return false;
		}

		int64_t nEnd = m_nStartTime + m_nInterval;
		if (nNow < nEnd)
		{
			return false;
		}

		m_nStartTime = nNow;
		return true;
	}

	//------------------------------------------------------------------------
	// 获得剩余间隔
	//------------------------------------------------------------------------
	int64_t GetRemainTime(int64_t nNow)
	{
		if (IsTimeout(nNow))
		{
			return 0;
		}
		return m_nStartTime + m_nInterval - nNow;
	}

public:
	bool IsStart() { return m_bStart; }
	int64_t GetStartTime() const { return m_nStartTime; }
	int64_t GetInterval() const { return m_nInterval; }
	void SetInterval(int64_t nInterval) { m_nInterval = nInterval; }

private:
	// 是否开始
	bool m_bStart = false;
	// 开始时间
	int64_t m_nStartTime = 0;
	// 间隔时间
	int64_t m_nInterval = 0;
};
