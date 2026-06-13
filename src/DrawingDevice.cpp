#include "DrawingDevice.h"
#include "Nu3D/BmpDataNode.h"

namespace DrawingDevice
{
	// $GLOBAL 00884008
	CD3DFramework* g_drawingDevice;

	// $GLOBAL 00884028
	DDAppDevice::App* g_ddAppListHead;

	// $GLOBAL 00884030
	DDAppDevice::App* g_primaryDDApp;

	// $GLOBAL 005281D4
	int32_t g_validDrawDevice;

	// $GLOBAL 005281D0
	LPDIRECT3DDEVICE3 g_unusedD3D;

	/* ------ CD3DFramework ------- */

	// $FUNC 004AEC80 [IMPLEMENTED]
	CD3DFramework::CD3DFramework()
	{
		hWnd = 0;
		bIsFullscreen = 0;
		dwRenderWidth = 0;
		dwRenderHeight = 0;
		pddsFrontBuffer = 0;
		pddsBackBuffer = 0;
		pddsRenderTarget = 0;
		pddsZBuffer = 0;
		pd3dDevice = 0;
		pvViewport = 0;
		pDD = 0;
		pD3D = 0;
		dwDeviceMemType = 0;
		initialized = 0;

		memset(slots, 0, sizeof(slots));
	}

	// $FUNC 004AECD0 [IMPLEMENTED]
	void CD3DFramework::Release() { Cleanup(); }

	// $FUNC 004AECE0 [IMPLEMENTED]
	int32_t CD3DFramework::Cleanup()
	{
		ULONG drawReleaseResult = 0;
		ULONG deviceReleaseResult = 0;

		if (pvViewport)
		{
			pvViewport->Release();
			pvViewport = 0;
		}

		if (pd3dDevice)
			deviceReleaseResult = pd3dDevice->Release();

		pd3dDevice = 0;

		if (! bIsFullscreen)
		{
			if (pddsBackBuffer)
			{
				pddsBackBuffer->Release();
				pddsBackBuffer = 0;
			}
		}

		if (pddsRenderTarget)
		{
			pddsRenderTarget->Release();
			pddsRenderTarget = 0;
		}

		if (pddsZBuffer)
		{
			pddsZBuffer->Release();
			pddsZBuffer = 0;
		}

		if (pddsFrontBuffer)
		{
			pddsFrontBuffer->Release();
			pddsFrontBuffer = 0;
		}

		if (pD3D)
		{
			pD3D->Release();
			pD3D = 0;
		}

		if (pDD)
		{
			pDD->SetCooperativeLevel(hWnd, 8);
			drawReleaseResult = pDD->Release();
		}

		pDD = 0;

		if (drawReleaseResult || deviceReleaseResult)
			return 0x8200000B;
		else
			return 0;
	}

	// $FUNC 004AEDA0 [IMPLEMENTED]
	HRESULT CD3DFramework::InitalizeForWindow(HWND hWnd, GUID* ddAppGuid, DDAppDevice* device, DDAppDevice::DisplayMode* displayMode, uint8_t flags)
	{
		if (! hWnd || ! displayMode && (flags & 1) != 0)
			return DDERR_INVALIDPARAMS;

		bIsFullscreen = flags & 1;

		HRESULT result = InitalizeDeviceAndSurfaces(ddAppGuid, &device->guid, displayMode, flags);

		if ((result & 0x80000000) != 0)
		{
			Cleanup();

			if (result == DDERR_GENERIC)
				return 0x82000000;
		}

		return result;
	}

	// $FUNC 004AEE10 [IMPLEMENTED]
	HRESULT CD3DFramework::InitalizeDeviceAndSurfaces(GUID* ddAppGuid, GUID* deviceGuid, DDAppDevice::DisplayMode* displayMode, uint8_t flags)
	{
		HRESULT result = CreateDirectDraw(ddAppGuid, flags);

		if (FAILED(result))
			return result;

		if (! deviceGuid || (result = SelectD3DDeviceAndZFormat(deviceGuid, flags), SUCCEEDED(result)))
		{
			result = CreatePrimaryChainAndRects(displayMode, flags);

			if (SUCCEEDED(result))
			{
				if (! deviceGuid)
					return 0;

				if ((flags & 4) == 0 || (result = CreateZBuffer(), SUCCEEDED(result)))
				{
					result = CreateD3DDevice(deviceGuid);

					if (SUCCEEDED(result))
					{
						result = CreateAndSetViewport();

						if (SUCCEEDED(result))
						{
							if (initialized)
								return 0x82000000;

							slots[0].width = dwRenderWidth;
							slots[0].height = dwRenderHeight;
							slots[0].surface1 = pddsBackBuffer;
							slots[0].valid = 1;
							slots[0].surface2 = pddsZBuffer;

							initialized = 1;

							return 0;
						}
					}
				}
			}
		}

		return result;
	}

	// $FUNC 004AEEE0 [UNFINISHED]
	HRESULT CD3DFramework::CreateDirectDraw(LPGUID lpGUID, uint8_t flags) { return 0; }

	// $FUNC 004AEF80 [UNFINISHED]
	HRESULT CD3DFramework::SelectD3DDeviceAndZFormat(GUID* deviceGuid, uint8_t flags) { return 0; }

	// $FUNC 004AF110 [UNFINISHED]
	HRESULT CD3DFramework::CreatePrimaryChainAndRects(DDAppDevice::DisplayMode* displayMode, uint8_t flags) { return 0; }

	// $FUNC 004AF420 [UNFINISHED]
	HRESULT CD3DFramework::CreateZBuffer() { return 0; }

	// $FUNC 004AF4E0 [UNFINISHED]
	HRESULT CD3DFramework::CreateD3DDevice(const CLSID* guid) { return 0; }

	// $FUNC 004AF4E0 [UNFINISHED]
	HRESULT CD3DFramework::CreateAndSetViewport() { return 0; }

	// $FUNC 004ABEB0 [IMPLEMENTED]
	HRESULT CD3DFramework::Build(HWND hWnd, GUID* guid, DDAppDevice* device, DDAppDevice::DisplayMode* displayMode, uint8_t flags)
	{
		g_drawingDevice = new CD3DFramework();
		Nu3D::g_currentBmpDataNode = 0;
		return g_drawingDevice->InitalizeForWindow(hWnd, guid, device, displayMode, flags);
	}

	/* ------ DrawingDevice ------- */

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

	// $FUNC 004ABF00 [IMPLEMENTED]
	void Destroy()
	{
		if (g_drawingDevice)
		{
			g_drawingDevice->Release();
			delete g_drawingDevice;
		}

		g_drawingDevice = 0;
	}

	// $FUNC 00412B10 [IMPLEMENTED]
	void Quit()
	{
		if (g_validDrawDevice)
		{
			if (g_unusedD3D)
			{
				g_unusedD3D->Release();
				g_unusedD3D = 0;
			}

			Destroy();

			g_validDrawDevice = 0;
		}
	}

	// $FUNC 004ACFC0 [IMPLEMENTED]
	HRESULT GetChosenDevice(DDAppDevice::App** outApp, DDAppDevice** outDevice)
	{
		DDAppDevice::App* primaryApp = g_primaryDDApp;

		if (! g_primaryDDApp)
			return 0x81000001;

		if (outApp)
		{
			*outApp = g_primaryDDApp;
			primaryApp = g_primaryDDApp;
		}

		if (outDevice)
			*outDevice = primaryApp->primaryDevice;

		return 0;
	}

	// $FUNC 004AC420 [IMPLEMENTED]
	HRESULT GetChosenDevice_T(DDAppDevice::App** outApp, DDAppDevice** outDevice) { return GetChosenDevice(outApp, outDevice); }

	// $FUNC 004AD000 [IMPLEMENTED]
	DDAppDevice::App* GetListHead() { return g_ddAppListHead; }
}