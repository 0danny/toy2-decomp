#pragma once

#include "Common.h"

namespace Toy2
{
	extern char    g_pathRegValue[512];
	extern char    g_cdPathRegValue[512];
	extern int32_t g_registryKeysRead;

	void ReadRegistry();
	void OneInit();
	void CheckForQuit();

	int32_t ReadCfg();
	int32_t ShowModeSelect();

	int32_t Run(int32_t p_argCount, char** p_argList);
}
