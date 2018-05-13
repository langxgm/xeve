
#include "TimeUtil.h"

#include <chrono>
#include <iostream>
#include <stdio.h>

#ifdef _WIN32
#define localtime_safe(tm, t) localtime_s((tm), (t))
#define time_zone(tz) _get_timezone(&tz)
#else
#define localtime_safe(tm, t) localtime_r((t), (tm))
#define time_zone(tz) ((tz) = __timezone)
#endif

typedef std::chrono::duration<int, std::ratio<86400> > days;
typedef std::chrono::duration<int, std::ratio<86400 * 7> > weeks;

#ifndef USE_SYSTEM_CLOCK
#define USE_SYSTEM_CLOCK (1)
#endif

template<typename T>
int64_t NowTimeConvert()
{
#if (USE_SYSTEM_CLOCK)
	auto tp = std::chrono::system_clock::now();
	auto du = tp.time_since_epoch();
	return std::chrono::duration_cast<T>(du).count();
#else
	auto tp = std::chrono::steady_clock::now();
	auto du = tp.time_since_epoch();
	return std::chrono::duration_cast<T>(du).count();
#endif
}

int64_t TimeUtil::GetCurrentTimeSeconds()
{
#if (USE_SYSTEM_CLOCK)
	auto tp = std::chrono::system_clock::now();
	return std::chrono::system_clock::to_time_t(tp);
#else
	auto tp = std::chrono::steady_clock::now();
	auto du = tp.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::seconds>(du).count();
#endif
}

int64_t TimeUtil::GetCurrentTimeMillis()
{
	return NowTimeConvert<std::chrono::milliseconds>();
}

int64_t TimeUtil::GetCurrentTimeMicros()
{
	return NowTimeConvert<std::chrono::microseconds>();
}

int64_t TimeUtil::GetCurrentTimeNanos()
{
	return NowTimeConvert<std::chrono::nanoseconds>();
}

int64_t TimeUtil::GetCurrentTimeMinute()
{
	return NowTimeConvert<std::chrono::minutes>();
}

int64_t TimeUtil::GetCurrentTimeHour()
{
	return NowTimeConvert<std::chrono::hours>();
}

int64_t TimeUtil::GetCurrentTimeDay()
{
	return NowTimeConvert<days>();
}

int64_t TimeUtil::GetCurrentTimeWeek()
{
	return NowTimeConvert<weeks>();
}

int TimeUtil::GetTimeZone()
{
	long tz = 0;
	time_zone(tz);
	return tz;
}

bool TimeUtil::IsSameDayOfSeconds(int64_t t1, int64_t t2)
{
	time_t s1 = t1, s2 = t2; // 自动进行32-64位转换,因为localtime_safe函数要传地址
	tm tm1, tm2;
	localtime_safe(&tm1, &s1); // 函数进行了时区转换
	localtime_safe(&tm2, &s2);
	return tm1.tm_year == tm2.tm_year
		&& tm1.tm_yday == tm2.tm_yday;
}

bool TimeUtil::IsSameDayOfMillis(int64_t t1, int64_t t2)
{
	return IsSameDayOfSeconds(t1 / 1000, t2 / 1000);
}

bool TimeUtil::IsSameWeekOfSeconds(int64_t t1, int64_t t2)
{
	int64_t diff = abs(t1 - t2);
	if (diff < 86400 * 7)
	{
		time_t s1 = t1;
		tm tm1;
		localtime_safe(&tm1, &s1); // 函数进行了时区转换

		int wday = (tm1.tm_wday == 0 ? 7 : tm1.tm_wday);
		int hs = tm1.tm_hour * 3600;
		int ms = tm1.tm_min * 60;
		int s = tm1.tm_sec;

		// 本周的第几秒
		int ws = (wday - 1) * 86400 + hs + ms + s;
		// 本周日的23:59:59
		int ws_end = 7 * 86400 - 1;

		// 转换成time_t
		// 本周一00:00:00
		int64_t t_w1_begin = t1 - ws;
		// 本周日23:59:59
		int64_t t_w7_end = t_w1_begin + ws_end;
		// [t_w1_begin, t_w7_end]
		return t2 >= t_w1_begin && t2 <= t_w7_end;
	}
	return false;
}

bool TimeUtil::IsSameWeekOfMillis(int64_t t1, int64_t t2)
{
	return IsSameWeekOfSeconds(t1 / 1000, t2 / 1000);
}

int TimeUtil::ToString(char* out, int64_t ts)
{
	tm tm0;
	localtime_safe(&tm0, &ts);
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif
	int n = sprintf(out,
		"%04d-%02d-%02d %02d:%02d:%02d",
		tm0.tm_year + 1900,
		tm0.tm_mon + 1,
		tm0.tm_mday,
		tm0.tm_hour,
		tm0.tm_min,
		tm0.tm_sec);
#ifdef _WIN32
#pragma warning(pop)
#endif
	return n;
}

int TimeUtil::ToString2(char* out, int64_t tms)
{
	int64_t ts = tms / 1000;
	tm tm0;
	localtime_safe(&tm0, &ts);
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif
	int n = sprintf(out,
		"%04d-%02d-%02d %02d:%02d:%02d:%03d",
		tm0.tm_year + 1900,
		tm0.tm_mon + 1,
		tm0.tm_mday,
		tm0.tm_hour,
		tm0.tm_min,
		tm0.tm_sec,
		static_cast<int>(tms % 1000));
#ifdef _WIN32
#pragma warning(pop)
#endif
	return n;
}

int TimeUtil::ToStringYMD(char* out, int64_t ts)
{
	tm tm0;
	localtime_safe(&tm0, &ts);
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif
	int n = sprintf(out,
		"%04d-%02d-%02d",
		tm0.tm_year + 1900,
		tm0.tm_mon + 1,
		tm0.tm_mday);
#ifdef _WIN32
#pragma warning(pop)
#endif
	return n;
}

int TimeUtil::ToStringHMS(char* out, int64_t ts)
{
	tm tm0;
	localtime_safe(&tm0, &ts);
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif
	int n = sprintf(out,
		"%02d:%02d:%02d",
		tm0.tm_hour,
		tm0.tm_min,
		tm0.tm_sec);
#ifdef _WIN32
#pragma warning(pop)
#endif
	return n;
}

int TimeUtil::ToStringHMSMS(char* out, int64_t tms)
{
	int64_t ts = tms / 1000;
	tm tm0;
	localtime_safe(&tm0, &ts);
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif
	int n = sprintf(out,
		"%02d:%02d:%02d:%03d",
		tm0.tm_hour,
		tm0.tm_min,
		tm0.tm_sec,
		static_cast<int>(tms % 1000));
#ifdef _WIN32
#pragma warning(pop)
#endif
	return n;
}

void TimeUtil::Print()
{
	int64_t ms = GetCurrentTimeMillis();
	char text[36];
	ToString2(text, ms);

	std::cout << "TIME INFO:" << "\n";
	std::cout << "  " << "time zone    : " << GetTimeZone() / 3600 << "\n";
	std::cout << "  " << "current time : " << ms / 1000 << "." << ms % 1000 << "\n";
	std::cout << "  " << "format time  : " << text << "\n";
	std::cout << std::endl;
}
