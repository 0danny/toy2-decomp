#pragma once

#include "Common.h"

struct ToyCfg
{
	uint32_t flags;
	uint32_t detail;
	float gammaCorrection;
	int32_t driverIndex;
	int32_t deviceIndex;
	int32_t displayModeIndex;
};

namespace Toy2
{
	extern ToyCfg g_toyCfgData;

	void OneInit();
	void CheckForQuit();

	int32_t ReadCfg();
	void InitCfg();

	int32_t ShowModeSelect();

	int32_t Run(int32_t p_argCount, char** p_argList);
}
