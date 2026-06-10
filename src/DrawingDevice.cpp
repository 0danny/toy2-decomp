#include "DrawingDevice.h"

namespace DrawingDevice
{
	// $GLOBAL 00884008
	CD3DFramework* g_drawingDevice;

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
}