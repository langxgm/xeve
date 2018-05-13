
#include "test_common.h"

#include "rapidjson/document.h"

//------------------------------------------------------------------------
// 测试json
//------------------------------------------------------------------------
TEST_UNIT(TestJson) {

	const char* json = "{\
		\"hello\":\"world\",\
		\"t\":true,\
		\"f\":false,\
		\"n\":null,\
		\"i\":123,\
		\"pi\":3.1415926,\
		\"a\":[1, 2, 3, 4],\
		\"o\":{\"i\":456, \"s\":\"string\"}\
		}";

	rapidjson::Document doc;
	if (doc.Parse(json).HasParseError())
	{
		std::cout << "error code=" << doc.GetParseError()
			<< " offset=" << doc.GetErrorOffset() << std::endl;
		return;
	}

	if (doc.IsObject())
	{
		for (auto& m : doc.GetObject())
		{
			std::cout << "member=" << m.name.GetString()
				<< " is " << m.value.GetType()
				<< std::endl;
		}
	}
	if (doc.HasMember("hello"))
	{
		rapidjson::Value& p = doc["hello"];
		if (p.IsString())
		{
			std::cout << p.GetString() << std::endl;
		}
	}
	if (doc.HasMember("t"))
	{
		rapidjson::Value& p = doc["t"];
		if (p.IsBool())
		{
			std::cout << p.GetBool() << std::endl;
		}
	}
	if (doc.HasMember("f"))
	{
		rapidjson::Value& p = doc["f"];
		if (p.IsBool())
		{
			std::cout << p.GetBool() << std::endl;
		}
	}
	if (doc.HasMember("n"))
	{
		rapidjson::Value& p = doc["n"];
		if (p.IsNull())
		{
			std::cout << p.IsNull() << std::endl;
		}
	}
	if (doc.HasMember("i"))
	{
		rapidjson::Value& p = doc["i"];
		if (p.IsNumber())
		{
			std::cout << p.GetInt() << std::endl;
			std::cout << p.GetUint() << std::endl;
			std::cout << p.GetInt64() << std::endl;
			std::cout << p.GetUint64() << std::endl;
		}
	}
	if (doc.HasMember("pi"))
	{
		rapidjson::Value& p = doc["pi"];
		if (p.IsDouble())
		{
			std::cout << p.GetDouble() << std::endl;
		}
	}
	if (doc.HasMember("a"))
	{
		rapidjson::Value& p = doc["a"];
		if (p.IsArray())
		{
			// 1
			for (rapidjson::SizeType i = 0; i < p.Size(); ++i)
			{
				if (p[i].IsInt())
				{
					std::cout << p[i].GetInt() << std::endl;
				}
			}

			// 2
			for (auto& v : p.GetArray())
			{
				if (v.IsInt())
				{
					std::cout << v.GetInt() << std::endl;
				}
			}
		}
	}

	auto itObject = doc.FindMember("o");
	if (itObject != doc.MemberEnd() && itObject->value.IsObject())
	{
		auto itInt = itObject->value.FindMember("i");
		if (itInt != itObject->value.MemberEnd() && itInt->value.IsInt())
		{
			std::cout << itInt->value.GetInt() << std::endl;
		}

		auto itString = itObject->value.FindMember("s");
		if (itString != itObject->value.MemberEnd() && itString->value.IsString())
		{
			std::cout << itString->value.GetString() << std::endl;
		}
	}

}
