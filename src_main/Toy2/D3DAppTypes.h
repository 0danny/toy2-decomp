#pragma once

#include "Common.h"
#include <directx7/d3d.h>

struct D3DAppMode
{
	int32_t w;
	int32_t h;
	int32_t bpp;
	BOOL bThisDriverCanDo;
};

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

struct RenderStateCache
{
	int32_t zWriteEnable;
	int32_t texturePerspective;
	int32_t shadeMode;
	int32_t textureFilter;
	int32_t cullMode;
	int32_t fillMode;
	int32_t ditherEnable;
	int32_t specularEnable;
	int32_t antiAlias;
	int32_t fogEnable;
	int32_t fogColor;
	int32_t fogTableMode;
	float fogStart;
	float fogEnd;
};

struct WindowData
{
	MSG wndEventMsg;
	HACCEL hAccTable;
	HINSTANCE hInstance;
	HINSTANCE hPrev;
	char* lpCmdLine;
	HWND mainHwnd;
	WNDCLASSEXA wndClass;
	int32_t nShowCmd;
	RenderStateCache stateCache;
	int32_t unkInt1;
	int32_t unkInt2;
	int32_t unkInt3;
	int32_t unkInt4;
	int32_t unkInt5;
	int32_t unkInt6;
	int32_t unkInt7;
	int32_t wndIsExiting;
	int32_t unkInt8;
	LPDIRECTDRAWSURFACE3 unusedDDSurface;
	LPDIRECTDRAWSURFACE3 unusedDDSurface2;
};

struct DisplayMode
{
	DWORD width;
	DWORD height;
	DWORD rgbBitCount;
};

#pragma pack(push, 1)
struct InterfaceDevice
{
	int32_t valid;
	int32_t hasTexturing;
	int32_t isSquareTexturesOnly;
	int32_t isHardwareAccelerated;
	int32_t hasZBuffer;
	int32_t unkInt1;
	int32_t hasAlphaBlending;
	GUID guid;
	char baseName[64];
	char description[64];
	uint8_t pad1[1800];
	D3DDEVICEDESC hwDeviceDesc;
};

struct ExamineDevice
{
	int32_t valid;
	int32_t hasHardwareAccel;
	int32_t has3DSupport;
	int32_t hasVideoDMA;
	int32_t svbCaps;
	int32_t svbcKeyCaps;
	int32_t svbFxCaps;
	int32_t unkInt8;
	int32_t texVidMemTot;
	int32_t texVidMemFree;
	int32_t hasLowVideoMem;
	int32_t isPrimaryDisplay;
	int32_t isPreferredDevice;
	GUID deviceGUID;
	DDCAPS_DX7 ddCaps;
	char driverName[64];
	char driverDesc[64];
	int32_t displayModeCount;
	int32_t selectedDisplayModeIndex;
	DisplayMode displayModes[128];
	IDirectDraw2* dd2;
	IDirect3D2* dd3D;
	int32_t deviceCount;
	InterfaceDevice interfaceDevices[6];
};
#pragma pack(pop)

struct PC
{
	int32_t fullscreenMode;
	int32_t softwareRenderMode;
	int32_t ddDeviceCount;
	ExamineDevice examineDevices[4];
	ExamineDevice* defaultDriver;
	InterfaceDevice* selectedInterfaceDevice;
	DisplayMode* mode;
	DisplayMode* oldMode;
};

enum RenderMode
{
	SoftwareRenderer = 0x1,
	D3DRenderer = 0x2,
};
