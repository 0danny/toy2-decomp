#pragma once

#include "Common.h"
#include <directx7/ddraw.h>

namespace ModeSelect
{
	typedef int32_t(* DeviceFilterCallback_t)(LPDDCAPS caps, void* context);
	
	extern int32_t g_unusedFlag1;
	extern int32_t g_unusedFlag2;

	void SetForceFullscreen_T(int32_t forceFullscreen);
	int32_t EnumerateDrivers_T(DeviceFilterCallback_t callback);
	void Show();
	int32_t DeviceFilterCallback(LPDDCAPS caps, void* context);
}