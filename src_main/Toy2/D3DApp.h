#pragma once

#include "common.h"

#include <windows.h>

namespace D3DApp
{
    extern int32_t g_allow32BitColors;
    extern int32_t g_nShowCmd;

    extern HINSTANCE g_hInstance;
    extern HINSTANCE g_hPrev;

    extern char* g_lpCmdLine;
    extern char g_pathRegValue[512];
    extern char g_cdPathRegValue[512];

    int32_t BuildProfileMachine();
    int32_t BuildWindow();
}