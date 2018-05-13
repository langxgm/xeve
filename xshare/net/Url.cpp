
#include "Url.h"

#include <assert.h>

namespace url
{
	unsigned char ToHex(unsigned char x)
	{
		return x > 9 ? x + 55 : x + 48;
	}

	unsigned char FromHex(unsigned char x)
	{
		unsigned char y = 0;
		if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
		else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
		else if (x >= '0' && x <= '9') y = x - '0';
		else assert(0);
		return y;
	}

	std::string UrlEncode(const std::string& str)
	{
		std::string result;
		size_t len = str.length();
		result.reserve(len * 1.4);
		for (size_t i = 0; i < len; ++i)
		{
			char c = str[i];
			if (isalnum((unsigned char)c)
				|| c == '-' || c == '_' || c == '.' || c == '~')
			{
				result += c;
			}
			else if (c == ' ')
			{
				result += '+';
			}
			else
			{
				result += '%';
				result += ToHex((unsigned char)c >> 4);
				result += ToHex((unsigned char)c & 0xF);
			}
		}
		return result;
	}

	std::string UrlDecode(const std::string& str)
	{
		std::string result;
		size_t len = str.length();
		result.reserve(len);
		for (size_t i = 0; i < len; ++i)
		{
			char c = str[i];
			if (c == '+')
			{
				result += ' ';
			}
			else if (c == '%')
			{
				if (i + 2 < len)
				{
					unsigned char high = FromHex((unsigned char)str[++i]);
					unsigned char low = FromHex((unsigned char)str[++i]);
					result += (high << 4) + low;
				}
				else
				{
					assert(false);
					return "";
				}
			}
			else
			{
				result += c;
			}
		}
		return result;
	}

}
