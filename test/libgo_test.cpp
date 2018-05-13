
#include "test_common.h"

#include <libgo/coroutine.h>

#include <chrono>
#include <iostream>
#include <string>

using namespace std;

namespace
{
	static const int N = 10000;//1000000

	template <typename T>
	void dump(string name, int n, T start, T end)
	{
		cout << name << "    " << n << "      " <<
			chrono::duration_cast<chrono::nanoseconds>(end - start).count() / n << " ns/op" << endl;
		//    cout << "ok. cost " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
	}

	void test_switch(int coro)
	{
		auto start = chrono::steady_clock::now();
		co_chan<void> c(coro);
		for (int i = 0; i < coro; ++i)
			go[=]{
			for (int i = 0; i < N / coro; ++i)
			co_yield;
		c << nullptr;
		};
		for (int i = 0; i < coro; ++i)
			c >> nullptr;
		auto end = chrono::steady_clock::now();
		dump("BenchmarkSwitch_" + std::to_string(coro), N, start, end);
	}

	void test_channel(int capa, int n)
	{
		co_chan<bool> c(capa);
		auto start = chrono::steady_clock::now();
		go[=]{
			for (int i = 0; i < n; ++i) {
				c << true;
			}
		};
		for (int i = 0; i < n; ++i)
			c >> nullptr;
		auto end = chrono::steady_clock::now();
		dump("BenchmarkChannel_" + std::to_string(capa), n, start, end);
	}
}

//------------------------------------------------------------------------
// 测试libgo
//------------------------------------------------------------------------
TEST_UNIT(TestLibgo)
{
	go[]{ test_switch(1); };
	co_sched.RunUntilNoTask();

	go[]{ test_switch(1000); };
	co_sched.RunUntilNoTask();

	//go[]{ test_switch(10000); };
	//co_sched.RunUntilNoTask();

	go[]{ test_channel(0, N); };
	co_sched.RunUntilNoTask();

	go[]{ test_channel(1, N); };
	co_sched.RunUntilNoTask();

	go[]{ test_channel(10000, 10000); };
	co_sched.RunUntilNoTask();

	//go[]{ test_channel(5000000, 5000000); };
	//co_sched.RunUntilNoTask();
}
