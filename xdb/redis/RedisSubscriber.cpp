
#include "RedisSubscriber.h"

#ifdef OPEN_REDIS_CLIENT

#include <cpp_redis/cpp_redis>

RedisSubscriber::RedisSubscriber()
{

}

RedisSubscriber::~RedisSubscriber()
{
	m_pSubscriber.reset();
}

void RedisSubscriber::Configure(std::string strHost, std::string strUserName, std::string strPassword)
{
	m_pSubscriber.reset(new cpp_redis::subscriber());
	m_strHost = strHost;
}

bool RedisSubscriber::Connect()
{
	try
	{
		std::string strIP = m_strHost;
		size_t nPort = 6379;

		size_t nPos = m_strHost.find(":");
		if (nPos != std::string::npos)
		{
			strIP = m_strHost.substr(0, nPos);
			nPort = atoi(m_strHost.substr(nPos + 1).c_str());
		}

		m_pSubscriber->connect(strIP/*"127.0.0.1"*/, nPort/*6379*/, [=](const std::string& host, std::size_t port, cpp_redis::subscriber::connect_state status) {
			if (status == cpp_redis::subscriber::connect_state::dropped)
			{
				cpp_redis::warn(std::string("redis subscriber disconnected from ") + host + ":" + std::to_string(port), __FILE__, __LINE__);
			}
			else if (status == cpp_redis::subscriber::connect_state::ok)
			{
				cpp_redis::warn(std::string("redis subscriber connect ok to ") + host + ":" + std::to_string(port), __FILE__, __LINE__);
			}
		}, 0, -1, 5000);
		if (m_pSubscriber->is_connected())
		{
			std::cout << "redis subscriber connected" << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
	return m_pSubscriber->is_connected();
}

void RedisSubscriber::Disconnect()
{
	m_pSubscriber->cancel_reconnect();

	if (m_pSubscriber->is_connected())
	{
		m_pSubscriber->disconnect(true);
	}
}

cpp_redis::subscriber* RedisSubscriber::GetSubscriber()
{
	return m_pSubscriber.get();
}

#endif
