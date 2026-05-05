#include "Loader.h"

namespace Loader
{

	void OpenConsole()
	{
		AllocConsole();

		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);

		printf("[LOADER]: Dll loaded.\n");

		SetConsoleTitleA("Testing DLL");
	}

	void Cleanup()
	{
		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();

		printf("[LOADER]: Hooks uninstalled successfully.\n");
	}

	DWORD WINAPI InjectHooks(LPVOID lpParam)
	{
		OpenConsole();

		printf("[LOADER]: Injecting hooks...\n");

		if ( MH_Initialize() != MH_OK )
		{
			printf("[LOADER]: Failed to initialize MinHook.\n");
			return FALSE;
		}

		GamePatches::InjectHooks();

		MH_EnableHook(MH_ALL_HOOKS);

		return 0;
	}
}
