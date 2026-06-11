#include "ModeSelect.h"
#include "DrawingDevice.h"
#include <directx7/d3d.h>

namespace ModeSelect
{
	// $GLOBAL 00505590
	int32_t g_forceFullscreen = 1;

	// $GLOBAL 004ECC10
	int32_t g_unusedFlag1 = 1;

	// $GLOBAL 004ECC0C
	int32_t g_unusedFlag2 = 1;

	// $GLOBAL 00884034;
	LPDDENUMCALLBACKA g_ddDeviceFilterCallback;

	// $GLOBAL 00884020
	int32_t g_foundRefDevice;

	// $GLOBAL 0088401C
	LPDIRECTDRAW4 g_ddraw4;

	// $FUNC 004ACBE0 [UNFINISHED]
	HRESULT SelectPrimaryDevice(uint8_t selectionFlags) { return 0; }

	// $FUNC 004ACFB0 [IMPLEMENTED]
	void SetForceFullscreen(int32_t p_forceFullscreen) { g_forceFullscreen = 1; }

	// $FUNC 004AC390 [IMPLEMENTED]
	void SetForceFullscreen_T(int32_t forceFullscreen) { SetForceFullscreen(forceFullscreen); }

	// $FUNC 004AC760 [UNFINISHED]
	HRESULT CALLBACK D3DDeviceEnumCallback(
		GUID* lpGuid, LPSTR lpDeviceDescription, LPSTR lpDeviceName, LPD3DDEVICEDESC lpD3DHWDeviceDesc, LPD3DDEVICEDESC lpD3DHELDeviceDesc, LPVOID lpContext)
	{
		using namespace DrawingDevice;

		DDAppDevice::App* ddApp = reinterpret_cast<DDAppDevice::App*>(lpContext);

		return D3DENUMRET_OK;
	}

	// $FUNC 004AC540 [IMPLEMENTED]
	BOOL WINAPI DDrawEnumCallbackExA(GUID* lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR hm)
	{
		using namespace DrawingDevice;

		DDSCAPS2 ddsCaps;
		LPDIRECTDRAW directDraw;

		if (DirectDrawCreate(lpGUID, &directDraw, 0) < 0)
			return TRUE;

		if (directDraw->QueryInterface(IID_IDirectDraw4, (LPVOID*)&g_ddraw4) < 0)
		{
			directDraw->Release();
			return TRUE;
		}

		directDraw->Release();

		LPDIRECT3D3 d3d3;
		if (g_ddraw4->QueryInterface(IID_IDirect3D3, (LPVOID*)&d3d3) < 0)
		{
			g_ddraw4->Release();
			return TRUE;
		}

		DDAppDevice::App* ddApp = new DDAppDevice::App;

		if (ddApp)
		{
			memset(ddApp, 0, sizeof(DDAppDevice::App));

			if (lpGUID)
			{
				ddApp->guid = *lpGUID;
				ddApp->ref = ddApp;
			}

			strncpy(ddApp->driverName, lpDriverName, sizeof(ddApp->driverName) - 1);
			strncpy(ddApp->driverDesc, lpDriverDescription, sizeof(ddApp->driverDesc) - 1);

			ddApp->hMonitor = hm;
			ddApp->ddCaps1.dwSize = sizeof(ddApp->ddCaps1);
			ddApp->ddCaps2.dwSize = sizeof(ddApp->ddCaps2);

			g_ddraw4->GetCaps(&ddApp->ddCaps1, &ddApp->ddCaps2);

			ddApp->vidMemTotal = ddApp->ddCaps1.dwVidMemTotal;
			ddApp->vidMemFree = ddApp->ddCaps1.dwVidMemFree;

			memset(&ddsCaps.dwCaps2, 0, 12);
			ddsCaps.dwCaps = 16896;

			g_ddraw4->GetAvailableVidMem(&ddsCaps, &ddApp->totalTextureMem, &ddApp->freeTextureMem);

			memset(&ddsCaps.dwCaps2, 0, 12);
			ddsCaps.dwCaps = 4096;

			g_ddraw4->GetAvailableVidMem(&ddsCaps, &ddApp->totalVideoMem, &ddApp->freeVideoMem);
			d3d3->EnumDevices(D3DDeviceEnumCallback, ddApp);

			if (ddApp->deviceListHead)
			{
				DDAppDevice::App* chain = g_ddAppListHead;
				DDAppDevice::App** curChain;

				for (curChain = &g_ddAppListHead; chain; chain = chain->chainDDApp)
					curChain = &chain->chainDDApp;

				*curChain = ddApp;

				if (! lpGUID)
					g_primaryDDApp = ddApp;
			}
			else
			{
				delete ddApp;
			}

			d3d3->Release();
			g_ddraw4->Release();

			return TRUE;
		}

		return reinterpret_cast<BOOL>(ddApp);
	}

	// $FUNC 004ACBC0 [IMPLEMENTED]
	BOOL WINAPI DDrawEnumCallback(GUID* lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext)
	{
		return DDrawEnumCallbackExA(lpGUID, lpDriverDescription, lpDriverName, 0, 0);
	}

	// $FUNC 004AC4D0 [IMPLEMENTED]
	HRESULT EnumerateDrivers(LPDDENUMCALLBACKA callback)
	{
		typedef HRESULT(WINAPI * DirectDrawEnumerateExA_t)(LPDDENUMCALLBACKEXA lpCallback, LPVOID lpContext, DWORD dwFlags);

		g_ddDeviceFilterCallback = callback;
		g_foundRefDevice = 0;

		HMODULE moduleHandle = GetModuleHandleA("DDRAW.DLL");

		if (! moduleHandle)
			return 0x81000004;

		DirectDrawEnumerateExA_t procAddress = (DirectDrawEnumerateExA_t)GetProcAddress(moduleHandle, "DirectDrawEnumerateExA");

		if (procAddress)
			procAddress(DDrawEnumCallbackExA, 0, 7);
		else
			DirectDrawEnumerateA(DDrawEnumCallback, 0);

		return SelectPrimaryDevice(0);
	}

	// $FUNC 004AC3A0 [IMPLEMENTED]
	HRESULT EnumerateDrivers_T(LPDDENUMCALLBACKA callback) { return EnumerateDrivers(callback); }

	// $FUNC 004334B0 [UNFINISHED]
	void Show() {};

	// $FUNC 00412B00 [IMPLEMENTED]
	BOOL WINAPI DeviceFilterCallback(GUID* lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext) { return 0; }
}