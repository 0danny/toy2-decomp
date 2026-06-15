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
	extern int32_t g_levelFileIndex;
	extern int32_t g_isElevatorHopLevel;

	void OneInit();
	void CheckForQuit();

	int32_t ReadCfg();
	void InitCfg();

	int32_t ShowModeSelect();
	int32_t Run(int32_t argCount, char** argList);

	STATIC_ASSERT(sizeof(ToyCfg) == 0x18);
}
