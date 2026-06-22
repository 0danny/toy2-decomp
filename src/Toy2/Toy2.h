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

	struct SectorBackdropTexIdTable
	{
		int32_t primary[10];
		int32_t secondary[8];
	};

	void SetBackdropByIndex(int32_t index);
	void ProcessMiscEventsEx();

	extern ToyCfg g_toyCfgData;
	extern int32_t g_levelFileIndex;
	extern int32_t g_isElevatorHopLevel;
	extern int32_t g_hasBackdrop;
	extern int32_t g_mainMenuState;
	extern int32_t g_attractModeTimer;
	extern int32_t g_returnedToTitle;
	extern int32_t g_saveLoaded;
	extern int32_t g_showBlackFrames;
	extern int32_t g_demoMode;
	extern int32_t g_hasStaticBackdrop;
	extern int32_t g_nextBackdropId;

	STATIC_ASSERT(sizeof(ToyCfg) == 0x18);
}
