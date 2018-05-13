#pragma once
#include <stdint.h>

#ifndef H_OS_WINDOWS // 添加这个宏,防止与其他库冲突(重复定义)

typedef int64_t ssize_t;

void usleep(uint64_t microseconds);

unsigned int sleep(unsigned int seconds);

#endif
