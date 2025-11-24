#pragma once

#include "D3DAppTypes.h"
#include "Common.h"

#include <windows.h>

namespace D3DApp
{
	extern int32_t g_allow32BitColors;
	extern int32_t g_nShowCmd;

	extern HINSTANCE g_hInstance;
	extern HINSTANCE g_hPrev;

	extern char* g_lpCmdLine;

	extern D3DAppInfo g_d3dAppI;
	extern PC g_pcStruct;
	extern WindowData g_windowData;
	extern RenderMode g_renderMode;

	int32_t BuildProfileMachine();
	int32_t BuildWindow();

	int32_t WINAPI EnumerateDevices(LPGUID p_guid, LPSTR p_driverDesc, LPSTR p_driverName, LPVOID p_lpContext);
	HRESULT WINAPI EnumDisplayModes(LPDDSURFACEDESC p_surfaceDesc, LPVOID p_context);
	HRESULT WINAPI EnumDevices(
	    LPGUID p_guid,
	    LPSTR p_deviceDesc,
	    LPSTR p_deviceName,
	    LPD3DDEVICEDESC p_d3DHWDeviceDesc,
	    LPD3DDEVICEDESC p_d3DHELDeviceDesc,
	    LPVOID p_context
	);

	int32_t SortDisplayModes(const void* p_modeA, const void* p_modeB);

	LRESULT WINAPI ProfileWndProc(HWND p_hWnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam);
	LRESULT WINAPI NormalWndProc(HWND p_hWnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam);

	void SysParmsOnExit();
}
