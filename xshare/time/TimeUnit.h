//------------------------------------------------------------------------
// * @filename: TimeUnit.h
// *
// * @brief: 时间单元
// *
// * @author: XGM
// * @date: 2017/08/18
//------------------------------------------------------------------------
#pragma once

#include <stdint.h>

class TimeUnit
{
public:
	TimeUnit() {}
	virtual ~TimeUnit() {}
protected:
	// -t [h]:[m]:s.ms
	// -n execute count
	int8_t m_nHour; // -1 = *
	int8_t m_nMinute;
	int8_t m_nSecond;
	int16_t m_nMillisecond;
	int32_t m_nMax;
	int64_t m_nCount;
};
