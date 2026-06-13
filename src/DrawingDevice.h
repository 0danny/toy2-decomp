#pragma once

#include "Common.h"
#include <directx7/d3d.h>

namespace DrawingDevice
{
	struct DrawingDeviceSlot
	{
		int32_t valid;
		int32_t unkInt2;
		int32_t width;
		int32_t height;
		LPDIRECTDRAWSURFACE4 surface1;
		LPDIRECTDRAWSURFACE4 surface2;
	};

	struct DDAppDevice
	{
		struct DisplayMode
		{
			DDSURFACEDESC2 surfaceDesc;
			char modeText[40];
			DisplayMode* nextDisplayMode;
		};

		struct App
		{
			GUID guid;
			App* ref;
			char driverName[40];
			char driverDesc[40];
			DDCAPS ddCaps1;
			DDCAPS ddCaps2;
			HMONITOR hMonitor;
			DWORD vidMemFree;
			DWORD vidMemTotal;
			DWORD freeTextureMem;
			DWORD totalTextureMem;
			DWORD freeVideoMem;
			DWORD totalVideoMem;
			DDAppDevice* primaryDevice;
			DDAppDevice* deviceListHead;
			App* chainDDApp;
		};

		GUID guid;
		DDAppDevice* ref;
		char deviceName[40];
		D3DDEVICEDESC deviceDesc;
		int32_t isHardwareAccelerated;
		int32_t supportsCurrentMode;
		int32_t canRenderWindowedOnPrimary;
		DisplayMode* primaryDisplayMode;
		DisplayMode* displayModeListHead;
		DDAppDevice* nextDevice;
		App* ddAppParent;
	};

	struct CD3DFramework
	{
		HWND hWnd;
		int32_t bIsFullscreen;
		int32_t dwRenderWidth;
		int32_t dwRenderHeight;
		RECT rcScreenRect;
		RECT rcViewportRect;
		LPDIRECTDRAWSURFACE4 pddsFrontBuffer;
		LPDIRECTDRAWSURFACE4 pddsBackBuffer;
		LPDIRECTDRAWSURFACE4 pddsRenderTarget;
		LPDIRECTDRAWSURFACE4 pddsZBuffer;
		LPDIRECT3DDEVICE3 pd3dDevice;
		LPDIRECT3DVIEWPORT3 pvViewport;
		LPDIRECTDRAW4 pDD;
		LPDIRECT3D3 pD3D;
		D3DDEVICEDESC ddDeviceDesc;
		int32_t dwDeviceMemType;
		DDPIXELFORMAT ddpfZBuffer;
		int32_t initialized;
		DrawingDeviceSlot slots[8];

		CD3DFramework();

		void Release();
		int32_t Cleanup();
		
		HRESULT InitalizeForWindow(HWND hWnd, GUID* ddAppGuid, DDAppDevice* device, DDAppDevice::DisplayMode* displayMode, uint8_t flags);
		HRESULT InitalizeDeviceAndSurfaces(GUID* ddAppGuid, GUID* deviceGuid, DDAppDevice::DisplayMode* displayMode, uint8_t flags);
		HRESULT CreateDirectDraw(LPGUID lpGUID, uint8_t flags);
		HRESULT SelectD3DDeviceAndZFormat(GUID* deviceGuid, uint8_t flags);
		HRESULT CreatePrimaryChainAndRects(DDAppDevice::DisplayMode* displayMode, uint8_t flags);
		HRESULT CreateZBuffer();
		HRESULT CreateD3DDevice(const CLSID* guid);
		HRESULT CreateAndSetViewport();

		static HRESULT Build(HWND hWnd, GUID* guid, DDAppDevice* device, DDAppDevice::DisplayMode* displayMode, uint8_t flags);
	};

	extern CD3DFramework* g_drawingDevice;
	extern DDAppDevice::App* g_ddAppListHead;
	extern DDAppDevice::App* g_primaryDDApp;

	LPDIRECTDRAW4 GetDDraw4();
	LPDIRECT3D3 GetD3D();
	LPDIRECT3DDEVICE3 GetD3DDevice();
	void InitViewport();
	RECT* GetDestRect();
	void Quit();
	HRESULT GetChosenDevice_T(DDAppDevice::App** outApp, DDAppDevice** outDevice);
	DDAppDevice::App* GetListHead();

	STATIC_ASSERT(sizeof(DrawingDeviceSlot) == 0x18);
	STATIC_ASSERT(sizeof(CD3DFramework) == 0x234);
	STATIC_ASSERT(sizeof(DDAppDevice::DisplayMode) == 0xA8);
	STATIC_ASSERT(sizeof(DDAppDevice::App) == 0x384);
	STATIC_ASSERT(sizeof(DDAppDevice) == 0x154);
}