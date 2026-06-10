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

	struct CD3DFramework
	{
		int32_t hWnd;
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
	};

	extern CD3DFramework* g_drawingDevice;

	LPDIRECTDRAW4 GetDDraw4();
	LPDIRECT3D3 GetD3D();
	LPDIRECT3DDEVICE3 GetD3DDevice();
	void InitViewport();
	RECT *GetDestRect();

	STATIC_ASSERT(sizeof(DrawingDeviceSlot) == 0x18);
	STATIC_ASSERT(sizeof(CD3DFramework) == 0x234);
}