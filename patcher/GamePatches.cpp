#include "GamePatches.h"

#include <cstdarg>
#include <cstdio>

namespace NGNLoader
{
	extern void ParseAreaPortalPos(FILE* stream, NGNImage* ngnImage);
}

namespace GamePatches
{
	void InjectHooks()
	{
		printf("[GAME_PATCHES]: Injecting hooks...\n");

		if (MH_CreateHook(TestAddress, (LPVOID)&TestMethodHook, NULL) != MH_OK)
			printf("[CONSTRUCTOR HOOK]: Could not hook test method.\n");
	}

	void __cdecl TestMethodHook(FILE* stream, NGNLoader::NGNImage* ngnImage)
	{
		printf("using our ParseAreaPortalRot!\n");
		NGNLoader::ParseAreaPortalPos(stream, ngnImage);
	}
}
