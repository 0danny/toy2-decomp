#pragma once

#include "Common.h"

namespace Toy2
{
	struct ToyCfg
	{
		uint32_t flags;
		int32_t detail;
		float gammaCorrection;
		int32_t driverIndex;
		int32_t deviceIndex;
		int32_t displayModeIndex;
	};

	void SetBackdropByIndex(int32_t index);

	extern ToyCfg g_toyCfgData;
	extern int32_t g_levelFileIndex;
	extern int32_t g_isElevatorHopLevel;
	extern int32_t g_hasBackdrop;
	extern int32_t g_mainMenuState;
	extern uint32_t g_frameDelta;
	extern int32_t g_attractModeTimer;
	extern int32_t g_returnedToTitle;

	STATIC_ASSERT(sizeof(ToyCfg) == 0x18);
}
