#include "DrawingDevice.h"

namespace DrawingDevice
{
	// $GLOBAL 00884008
	CD3DFramework* g_drawingDevice;

	// $GLOBAL 00884028
	DDAppDevice::App* g_ddAppListHead;

	// $GLOBAL 00884030
	DDAppDevice::App* g_primaryDDApp;

	// $FUNC 004ABA40 [IMPLEMENTED]
	LPDIRECTDRAW4 GetDDraw4() { return g_drawingDevice->pDD; }

	// $FUNC 004ABA70 [IMPLEMENTED]
	LPDIRECT3D3 GetD3D() { return g_drawingDevice->pD3D; }

	// $FUNC 004ABA80 [IMPLEMENTED]
	LPDIRECT3DDEVICE3 GetD3DDevice() { return g_drawingDevice->pd3dDevice; }

	// $FUNC 004B55D0 [UNFINISHED]
	void InitViewport() {}

	// $FUNC 004ABD80 [IMPLEMENTED]
	RECT* GetDestRect() { return &g_drawingDevice->rcViewportRect; }

	// $FUNC 00412B10 [UNFINISHED]
	void Quit() {}

	// $FUNC 004ABEB0 [UNFINISHED]
	HRESULT Build(HWND hWnd, GUID* guid, DDAppDevice* device, DDAppDevice::DisplayMode* displayMode, uint8_t flags) { return 0; }

	// $FUNC 004ACFC0 [UNFINISHED]
	HRESULT GetChosenDevice(DDAppDevice::App** outApp, DDAppDevice** outDevice) { return 0; }

	// $FUNC 004AC420 [IMPLEMENTED]
	HRESULT GetChosenDevice_T(DDAppDevice::App** outApp, DDAppDevice** outDevice) { return GetChosenDevice(outApp, outDevice); }

	// $FUNC 004AD000 [IMPLEMENTED]
	DDAppDevice::App* GetListHead() { return g_ddAppListHead; }
}