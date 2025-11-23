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

	int32_t BuildProfileMachine();
	int32_t BuildWindow();
}

struct D3DAppInfo
{
	HWND hwnd;
	uint8_t pad0[768];
	LPDIRECTDRAWSURFACE lpTextureSurf[15];
	LPDIRECT3DTEXTURE2 lpTexture[15];
	uint8_t pad0_0[136];
	uint8_t pad1[52276];
	int32_t unkInt1;
	int32_t unkInt2;
	int32_t unkInt3;
	int32_t unkInt4;
	int32_t unkInt5;
	int32_t unkInt6;
	int32_t unkInt7;
	LPDIRECT3D2 lpD3D;
	LPDIRECT3DDEVICE2 lpD3DDevice;
	LPDIRECT3DVIEWPORT2 lpViewport;
	LPDIRECTDRAW2 lpDD;
	int32_t bIsPrimary;
	LPDIRECTDRAWSURFACE2 lpFrontBuffer;
	LPDIRECTDRAWSURFACE2 lpBackBuffer;
	LPDIRECTDRAWSURFACE2 lpZBuffer;
	int32_t bBackBufferInVideo;
	D3DAppMode windowsDisplay;
	SIZE szClient;
	POINT pClientOnPrimary;
	uint8_t bPaused;
	uint8_t bAppActive;
	uint8_t bTexturesDisabled;
	uint8_t bOnlySystemMemory;
	uint8_t bOnlyEmulation;
	uint8_t bMinimized;
	uint8_t bRenderingIsOK;
};
