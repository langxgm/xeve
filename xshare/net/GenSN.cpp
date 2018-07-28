
#include "GenSN.h"

#include "xbase/TimeUtil.h"

#include <atomic>

std::string GenSN::T20()
{
	// 生成20个字符的流水号
	static std::atomic_uint sn_num = { 0 };
	uint64_t nNow = TimeUtil::GetCurrentTimeMillis();

	char szSN[32];
	sprintf(szSN, "%08X%08X%04d", uint32_t(nNow >> 32) & 0xFFFFFFFF, uint32_t(nNow), (++sn_num) % 10000);
	return szSN;
}
