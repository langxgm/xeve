
#include "Env.h"
#include "Application.h"

#include "xbase/LoadAverage.h"
#include "xbase/TimeUtil.h"

#include <evpp/inner_pre.h>
#include <evpp/event_loop.h>
#include <evpp/libevent.h>

#include <signal.h>
#include <numeric>

Env::Env(Application* pApp)
	:m_pApp(pApp)
{
#ifdef WIN32
	// Initialize Winsock 2.2
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err)
	{
		std::cout << "WSAStartup() failed with error: %d" << err;
	}
#endif
}

Env::~Env()
{
#ifdef WIN32
	system("pause");
	WSACleanup();
#endif
}

bool Env::InitEnv(int argc, char** argv)
{
#if WIN32
	// 设置窗口名称
	{
		std::string cmd = "title " + m_pApp->GetName();
		system(cmd.c_str());
	}
#endif

#if WIN32
	// Detected memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#if WIN32
	signal_number_t signo = SIGINT;
#else
	signal_number_t signo = SIGTERM;
#endif
	m_pSignalEvent.reset(new evpp::SignalEventWatcher(signo,
		m_pApp->GetMainEventLoop(), std::bind(&Env::HandleSignal, this)));
	m_pApp->GetMainEventLoop()->RunInLoop([this]() {
		m_pSignalEvent->Init();
		m_pSignalEvent->AsyncWait();
	});
	return true;
}

void Env::ExitEnv()
{
	m_pSignalEvent.reset();
}

void Env::HandleSignal()
{
	LOG(ERROR) << "Handle exit signal. . .";
	m_pApp->Exit();
}

#if WIN32
#include <list>
double CalcLoadAverage(std::list<double>& elapseds, uint32_t near_num)
{
	std::list<double>::iterator end = elapseds.begin();
	size_t i = 0;
	for (; i < elapseds.size() && i < near_num; ++i)
	{
		end++;
	}
	if (elapseds.size() == 0)
	{
		return 0.0;
	}
	return std::accumulate(elapseds.begin(), end, 0.0) / i;
}
#endif

void Env::ShowLoadAverage(double frameElapsed, double frameInterval)
{
#if WIN32
	// 保存15分钟的数据
	static const size_t size1s = 1 / frameInterval; // num of interval
	static const size_t size5s = 5, size15s = 15, size1m = 60, size5m = 5 * 60, size15m = 15 * 60; // num of 1s
	static LoadAverage<double> la1s(size1s), la5s(size5s), la15s(size15s), la1m(size1m), la5m(size5m), la15m(size15m);

	la1s.PushValue(frameElapsed);

	// 1s更新一次
	int64_t now = TimeUtil::GetCurrentTimeMillis();
	static int64_t last = now;
	if (now - last >= 1000)
	{
		last = now;

		// 1s,5s,15s,1m,5m,15m平均负载
		double load1s = 0, load5s = 0, load15s = 0, load1m = 0, load5m = 0, load15m = 0;

		load1s = la1s.GetAverageValue();

		la5s.PushValue(load1s);
		la15s.PushValue(load1s);
		la1m.PushValue(load1s);
		la5m.PushValue(load1s);
		la15m.PushValue(load1s);

		load5s = la5s.GetAverageValue();
		load15s = la15s.GetAverageValue();
		load1m = la1m.GetAverageValue();
		load5m = la5m.GetAverageValue();
		load15m = la15m.GetAverageValue();

		char buf[64];
		sprintf(buf, "-s %.3f, %.3f, %.3f/%.3f -m %.3f, %.3f, %.3f", load1s, load5s, load15s, frameInterval,
			load1m, load5m, load15m);

#if 0
		char cmd[128];
		sprintf(cmd, "title %s: %s", m_pApp->GetName().c_str(), buf);
		system(cmd);
#else
		char title[128];
		sprintf(title, "%s: %s", m_pApp->GetName().c_str(), buf);

		char oldTitle[256];
		GetConsoleTitle(oldTitle, sizeof(oldTitle));
		if (strcmp(title, oldTitle) != 0)
		{
			SetConsoleTitle(title);
		}
#endif
	}
#endif
}
