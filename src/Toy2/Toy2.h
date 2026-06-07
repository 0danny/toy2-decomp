#pragma once

#include "Common.h"

namespace Toy2
{
	struct ToyCfg
	{
		uint32_t flags;
		uint32_t detail;
		float gammaCorrection;
		int32_t driverIndex;
		int32_t deviceIndex;
		int32_t displayModeIndex;
	};

	extern ToyCfg g_toyCfgData;
	extern char g_pathRegValue[512];
	extern char g_cdPathRegValue[512];
	extern int32_t g_registryKeysRead;
	extern int32_t g_levelFileIndex;

	void OneInit();
	void CheckForQuit();

	int32_t ReadCfg();
	void InitCfg();

	int32_t ShowModeSelect();
	int32_t Run(int32_t argCount, char** argList);
}

STATIC_ASSERT(sizeof(Toy2::ToyCfg) == 0x18);