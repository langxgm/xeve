
#include "IOUtil.h"

#include <stdio.h>

std::unique_ptr<char[]> IOUtil::ReadFile(const char* pszFileName, int& rLength)
{
	FILE* f = fopen(pszFileName, "rb");
	if (f == nullptr)
	{
		return nullptr;
	}

	fseek(f, 0, SEEK_END);
	int nLength = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* pBuffer = new char[nLength];
	fread(pBuffer, nLength, 1, f);
	fclose(f);

	rLength = nLength;
	return std::unique_ptr<char[]>(pBuffer);
}
