
#include "ProtobufReader.h"

#include <google/protobuf/util/json_util.h>
#include <glog/logging.h>

#define CHECK_JSON (1)
#if CHECK_JSON
#include <third_party/rapidjson/document.h>
#endif


ProtobufReader::~ProtobufReader()
{
	delete m_pReceiver;
}

bool ProtobufReader::Read(const char* pBuffer, uint32_t nLength)
{
	if (m_pReceiver != nullptr)
	{
		delete m_pReceiver;
	}

	m_pReceiver = m_pFactory->New();
	google::protobuf::util::Status s = google::protobuf::util::JsonStringToMessage(std::string(pBuffer, nLength), m_pReceiver);
	if (s.ok() == false)
	{
		LOG(ERROR) << "ProtobufReader read fail MessageTypeName=" << m_pReceiver->GetTypeName()
			<< " error_code=" << s.error_code()
			<< " error=" << s.ToString();
		return false;
	}
	return true;
}

MultiProtobufReader::~MultiProtobufReader()
{
	for (auto v : m_listResult)
	{
		delete v;
	}
	m_listResult.clear();
}

bool MultiProtobufReader::Read(const char* pBuffer, uint32_t nLength)
{
	const char* nextJson = pBuffer;
	auto nextObject = [&]()
	{
		// 找到Object对象的起点
		if ((nextJson = strstr(nextJson, "{")))
		{
			const char* start = nextJson;
			const char* end = nullptr;
			++nextJson;
			// 跳过嵌套的{}
			{
				const char* nested_start = nextJson;
				const char* nested_end = nested_start;
				do
				{
					nested_start = strstr(nested_end, "{");
					nested_end = strstr(nested_end, "}");
					end = nested_end;
				} while (nested_start && nested_start < nested_end && ++nested_end);
			}

			if (end)
			{
				nextJson = end + 1;

				ProtobufReader r;
				r.SetFactory(m_pFactory);
				if (r.Read(start, nextJson - start))
				{
					m_listResult.push_back(r.GetResult(true));
					return true;
				}
			}
		}

		return false;
	};

#if CHECK_JSON
	// 这里验证buffer是不是json数组
	rapidjson::Document doc;
	if (doc.Parse(pBuffer, nLength).HasParseError())
	{
		LOG(ERROR) << "ProtobufReader json parse error code=" << doc.GetParseError()
			<< " offset=" << doc.GetErrorOffset();
		return false;
	}

	for (auto& v : doc.GetArray())
	{
		if (v.IsObject() == false)
		{
			LOG(ERROR) << "ProtobufReader json content not array objectType=" << v.GetType();
			return false;
		}
		else
		{
			if (nextObject() == false)
			{
				return false;
			}
		}
	}
#else
	while (nextObject());
#endif

	return true;
}
