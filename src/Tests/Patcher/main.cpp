#include "Loader.h"

#include <windows.h>
#include <minhook.h>
#include <iostream>
#include <fstream>

namespace
{
	static HANDLE g_thread = 0;
	static DWORD g_threadId = 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls(hModule);

		g_thread = CreateThread(NULL, 0, Loader::InjectHooks, NULL, 0, &g_threadId);
	}
	else if ( ul_reason_for_call == DLL_PROCESS_DETACH )
	{
		Loader::Cleanup();
	}

	return TRUE;
}
