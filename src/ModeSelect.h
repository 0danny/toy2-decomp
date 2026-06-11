#pragma once

#include "Common.h"
#include <directx7/ddraw.h>

namespace ModeSelect
{
	extern int32_t g_unusedFlag1;
	extern int32_t g_unusedFlag2;

	void SetForceFullscreen_T(int32_t forceFullscreen);
	HRESULT EnumerateDrivers_T(LPDDENUMCALLBACKA callback);
	void Show();
	BOOL WINAPI DeviceFilterCallback(GUID* lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext);
}