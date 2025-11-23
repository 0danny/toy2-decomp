#pragma once

#include "common.h"

#include <windows.h>
#include <directx7/d3d.h>
#include <directx7/d3dtypes.h>

namespace D3DApp
{
	extern int32_t g_allow32BitColors;
	extern int32_t g_nShowCmd;

	extern HINSTANCE g_hInstance;
	extern HINSTANCE g_hPrev;

	extern char* g_lpCmdLine;

	int32_t BuildProfileMachine();
	int32_t BuildWindow();
}
