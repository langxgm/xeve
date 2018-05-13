
#include "SessionStatistics.h"

#include "xbase/TimeUtil.h"

void SessionStatistics::AddMessageCount(uint32_t nMsgID, int32_t nMsgCount)
{
	m_nCount += nMsgCount;
	m_nLastTime = TimeUtil::GetCurrentTimeMillis();
}

int32_t SessionStatistics::GetMessageCount(uint32_t nMsgID)
{
	return m_nCount;
}

int64_t SessionStatistics::GetLastTime()
{
	return m_nLastTime;
}

std::string SessionStatistics::GetStatusInfo()
{
	std::string info;
	info.reserve(96);

	char szTime[32];
	TimeUtil::ToString(szTime, m_nLastTime);

	info += "SessionStatistics: {";
	info.append(" msgCount: ").append(std::to_string(m_nCount));
	info.append(", lastTime: ").append("\"").append(szTime).append("\"");
	info += " }";
	return std::move(info);
}
