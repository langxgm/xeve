
#include "Application.h"
#include "Env.h"
#include "time/BuildTime.h"

#include <evpp/inner_pre.h>
#include <evpp/event_loop.h>
#include <evpp/event_loop_thread.h>
#include <evpp/timestamp.h>
#include <gflags/gflags.h>

// 当前程序的ID
DEFINE_int32(id, 1, "the application id");

Application::Application()
{
	m_pEnv = new Env(this);
	m_pLoop = new evpp::EventLoop();
	m_pLoopDaemon = new evpp::EventLoopThread();
}

Application::~Application()
{
	delete m_pLoopDaemon;
	delete m_pLoop;
	delete m_pEnv;
}

void FailureSignalHandler(const char* data, int size)
{
	LOG(ERROR) << std::string(data, size);
}

std::string GetLogFlagsInfo()
{
	std::string info;
	info.reserve(256);
	info += "LogFlagsInfo: {";
	info.append(" minloglevel: ").append(std::to_string(FLAGS_minloglevel)); // 最小日志等级,默认0:INFO
	info.append(", logbuflevel: ").append(std::to_string(FLAGS_logbuflevel)); // 小于等于这个标志的日志被缓冲,更高级别的立即刷新,默认0:INFO
	info.append(", log_dir: ").append('"' + FLAGS_log_dir + '"'); // 日志文件输出目录,默认为空(/tmp/)
	info.append(", max_log_size: ").append(std::to_string(FLAGS_max_log_size)); // 日志文件大小(MB),默认1800
	info.append(", logbufsecs: ").append(std::to_string(FLAGS_logbufsecs)); // 日志缓冲时间,默认30秒
	info.append(", stderrthreshold: ").append(std::to_string(FLAGS_stderrthreshold)); // 除了将日志输出到文件之外,还将此错误级别和更高级别的日志同时输出到stderr,默认2:ERROR
	info.append(", colorlogtostderr: ").append(std::to_string(FLAGS_colorlogtostderr)); // 日志输出到标准输出的颜色,默认0:false
	info.append(", stop_logging_if_full_disk: ").append(std::to_string(FLAGS_stop_logging_if_full_disk)); // 磁盘满了,停止日志,默认0:false
	info += " }";
	return info;
}

std::string GetLogDir(const char* argv0)
{
	std::string path = argv0;
	size_t pos = path.rfind("\\");
	if (pos == std::string::npos)
	{
		pos = path.rfind("/");
	}
	if (pos != std::string::npos)
	{
		path = path.substr(0, pos);
		path += "/log/";
		return path;
	}
	return "";
}

int32_t Application::GetID()
{
	return FLAGS_id;
}

bool Application::Init(int argc, char** argv)
{
	//google::ShowUsageWithFlags(argv[0]);
	google::SetVersionString(GetBuildTime()/*"1.0.0"*/);
	google::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);

	// 日志级别
	{
#ifdef _DEBUG
		google::LogSeverity lv = google::GLOG_INFO;
		FLAGS_minloglevel = lv;
		FLAGS_stderrthreshold = lv;
#endif
	}
	// 日志目录
	{
		std::string path = GetLogDir(argv[0]);
		if (path.length() > 0)
		{
			// 目录
			FLAGS_log_dir = path;
			// 文件
			//google::SetLogDestination(google::GLOG_INFO, "");
		}
	}
	// 日志输出到标准输出的颜色
	FLAGS_colorlogtostderr = true;
	// 日志文件大小(MB)
	FLAGS_max_log_size = 100;
	// 日志缓冲时间,默认30秒
	//FLAGS_logbufsecs = 0;
	// 磁盘满了,停止日志
	FLAGS_stop_logging_if_full_disk = true;

	// 异常信号处理(SIGSEGV)
	//google::InstallFailureSignalHandler();
	//google::InstallFailureWriter(&FailureSignalHandler);

	LOG(INFO) << "INFO";
	LOG(WARNING) << "WARNING";
	LOG(ERROR) << "ERROR";

#ifdef _DEBUG
	std::string type = "DEBUG";
#else
	std::string type = "RELEASE";
#endif
	LOG(WARNING) << "Start " << GetName() << " -" << type << " -id=" << GetID();
	LOG(WARNING) << "Build date: " << GetBuildTime();

	LOG(WARNING) << GetLogFlagsInfo();

	// 事件守护线程
	m_pLoopDaemon->set_name("EventLoopThread-daemon");
	m_pLoopDaemon->Start(true);

	// 初始化运行环境
	return m_pEnv->InitEnv(argc, argv);
}

void Application::Exit()
{
	m_pLoopDaemon->Stop(true);
	m_pEnv->ExitEnv();
	m_pLoop->Stop();

	LOG(ERROR) << "Application exit. . .";
	google::ShutdownGoogleLogging();
	google::ShutDownCommandLineFlags();
}

void Application::Loop(int fps)
{
	// run frame
	m_pLoop->RunEvery(evpp::Duration(1), [this, fps]() {

		if (m_pLoop->IsStopping())
		{
			// Main EventLoop::Stop 退出时
			// 模块已删除,不需要再执行了
			return;
		}

		double frameInterval = 1.0 / fps;

		evpp::Timestamp begin = evpp::Timestamp::Now();

		// frame
		RunOnce();

		evpp::Duration cost = evpp::Timestamp::Now() - begin;
#ifdef WIN32
		if (cost.Seconds() > frameInterval)
#else
		if (cost.Seconds() > (std::max)(frameInterval, 0.04)) // 非windows平台帧间隔限制加大,linux有时空跑一帧会达到0.01s
#endif
		{
			LOG_WARN << "one loop use time over limit=" << frameInterval
				<< " cost=" << cost.Seconds();
		}

		// load average
		m_pEnv->ShowLoadAverage(cost.Seconds(), frameInterval);

		if (cost.Seconds() < frameInterval)
		{
			double s = frameInterval - cost.Seconds();
			int ms = s * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}
	});

	// loop
	m_pLoop->Run();
}
