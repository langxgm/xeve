
#include "test_common.h"

#include "xshare/work/WorkerPool.h"
#include "xshare/work/WorkDispatcher.h"

//------------------------------------------------------------------------
// 测试调用参数
//------------------------------------------------------------------------
TEST_UNIT(TestWorkDispatcher) {

	evpp::EventLoop loop;

	WorkerPool::InitInstance(&loop, 1);
	WorkerPool::Me()->Start();

	auto afunc = [=]() {
	};

	evpp::EventLoop::Functor func = [=]() {
	};

	// default
	WorkDispatcherSync().RunAfter(1.0, afunc);

	WorkDispatcherAsync().RunAfter(1.0, afunc);
	WorkDispatcherAsync().RunAfter(1.0, func);
	WorkDispatcherAsync().RunAfter(1.0, [=]() {
	});


	// with hash
	WorkDispatcherSync(1).RunAfter(1.0, afunc);

	WorkDispatcherAsync(1).RunAfter(1.0, afunc);
	WorkDispatcherAsync(2).RunAfter(1.0, func);
	WorkDispatcherAsync(3).RunAfter(1.0, [=]() {
	});

	WorkerPool::Me()->Stop();
	WorkerPool::DestroyInstance();
}
