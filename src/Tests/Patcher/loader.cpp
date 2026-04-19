#include "loader.h"

namespace Loader
{
	void InjectHooks()
	{
		printf("[LOADER]: Injecting hooks...\n");

		GamePatches::InjectHooks();

		MH_EnableHook(MH_ALL_HOOKS);
	}
}
