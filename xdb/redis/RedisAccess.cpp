
#include "RedisAccess.h"

#ifdef OPEN_REDIS_CLIENT

#include <cpp_redis/cpp_redis>

RedisAccess::RedisAccess()
{
}

RedisAccess::~RedisAccess()
{
	m_pClient.reset();
}

void RedisAccess::Configure(std::string strHost, std::string strUserName, std::string strPassword)
{
	m_pClient.reset(new cpp_redis::client());
	m_strHost = strHost;
	//m_pClient->add_sentinel(strHost, 6379);
}

bool RedisAccess::Connect()
{
	for (int i = 0; i < 3; ++i)
	{
		try
		{
			if (m_pClient->get_sentinel().get_sentinels().size() > 0)
			{
				cpp_redis::network::set_default_nb_workers(2);

				m_pClient->connect("mymaster", [=](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
					if (status == cpp_redis::client::connect_state::dropped)
					{
						cpp_redis::warn(std::string("redis client disconnected from ") + host + ":" + std::to_string(port), __FILE__, __LINE__);
					}
				}, 0, -1, 5000);
			}
			else
			{
				std::string strIP = m_strHost;
				size_t nPort = 6379;

				size_t nPos = m_strHost.find(":");
				if (nPos != std::string::npos)
				{
					strIP = m_strHost.substr(0, nPos);
					nPort = atoi(m_strHost.substr(nPos + 1).c_str());
				}

				m_pClient->connect(strIP/*"127.0.0.1"*/, nPort/*6379*/, [=](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
					if (status == cpp_redis::client::connect_state::dropped)
					{
						cpp_redis::warn(std::string("redis client disconnected from ") + host + ":" + std::to_string(port), __FILE__, __LINE__);
					}
				});
				if (m_pClient->is_connected())
				{
					std::cout << "redis PING" << std::endl;
					m_pClient->ping([](cpp_redis::reply& reply) {
						std::cout << "redis " << reply.as_string() << std::endl;
						cpp_redis::info(reply.as_string(), __FILE__, __LINE__);
					});
					m_pClient->sync_commit();
					break;
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
	}
	return m_pClient->is_connected();
}

cpp_redis::client* RedisAccess::GetAccessor()
{
	return m_pClient.get();
}

#endif
