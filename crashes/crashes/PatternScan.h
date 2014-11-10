#include "main.h"

#ifdef WIN32
#include <Psapi.h>
#else

#endif


DWORD FindPattern(char *pattern, char *mask)
{
#ifdef WIN32
	MODULEINFO mInfo = { 0 };

	GetModuleInformation(GetCurrentProcess(), GetModuleHandle("samp.dll"), &mInfo, sizeof(MODULEINFO));

	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;
#else

#endif

	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}