#include "Loader.h"

#include <windows.h>
#include <minhook.h>
#include <iostream>
#include <fstream>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls(hModule);

		Loader::InjectHooks();
	}
	else if ( ul_reason_for_call == DLL_PROCESS_DETACH )
	{
		Loader::Cleanup();
	}

	return TRUE;
}
