#include "D3DApp.h"
#include "Logger.h"

namespace
{
	int32_t g_changingCoopLevel = 0;
	int32_t g_checkAvailableMem = 1;
	int32_t g_sysParamsInfo;
}

namespace D3DApp
{
	int32_t g_allow32BitColors = 0;
	int32_t g_nShowCmd = 0;

	HINSTANCE g_hInstance = NULL;
	HINSTANCE g_hPrev = NULL;

	char* g_lpCmdLine = NULL;

	D3DAppInfo g_d3dAppI;
	PC g_pcStruct;
	WindowData g_windowData;
	RenderMode g_renderMode;

	// $FUNC 004093A0 [IMPLEMENTED]
	int32_t BuildProfileMachine()
	{
		memset(&g_pcStruct, 0, sizeof(g_pcStruct));

		Logger::Log("---------------------\n");
		Logger::Log("BEGIN EXAMINE MACHINE\n\n");

		int32_t count = 0;

		WNDCLASSA wndClass;
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = ProfileWndProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = g_windowData.hInstance;
		wndClass.hIcon = LoadIconA(0, IDI_WINLOGO);
		wndClass.hCursor = LoadCursorA(0, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = "";
		wndClass.lpszClassName = "Examining Machine";

		if ( ! RegisterClassA(&wndClass) )
			return 0;

		int32_t curDeviceCount = 1;

		g_windowData.mainHwnd = CreateWindowExA(
		    WS_EX_APPWINDOW, "Examining Machine", "Examining Machine",
		    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		    CW_USEDEFAULT, 0, 0, g_windowData.hInstance, 0
		);

		ShowWindow(g_windowData.mainHwnd, 1);
		UpdateWindow(g_windowData.mainHwnd);
		SetFocus(g_windowData.mainHwnd);

		g_pcStruct.ddDeviceCount = 0;

		HRESULT ddEnumResult = DirectDrawEnumerateA(EnumerateDevices, 0);
		if ( ddEnumResult < 0 )
			Logger::LogDDError("DirectDrawEnumerateA(ExamineDDEnumCallback, 0)", ddEnumResult);

		Logger::Log("END EXAMINE MACHINE\n");
		Logger::Log("-------------------\n\n");
		Logger::Log("--------------------\n");
		Logger::Log("BEGIN SELECT DRIVERS\n\n");

		g_pcStruct.defaultDriver = g_pcStruct.examineDevices;
		g_pcStruct.examineDevices[0].isPreferredDevice = 1;

		Logger::Log(
		    "INFO DirectDraw - Default driver >>>%s,%s<<< selected.\n", g_pcStruct.examineDevices[0].driverName,
		    g_pcStruct.examineDevices[0].driverDesc
		);

		if ( g_pcStruct.ddDeviceCount > 1 )
		{
			int32_t deviceIter = 1;

			do
			{
				ExamineDevice* curDevice = &g_pcStruct.examineDevices[deviceIter];

				if ( curDevice->valid && g_pcStruct.examineDevices[deviceIter].hasHardwareAccel &&
				     g_pcStruct.examineDevices[deviceIter].has3DSupport )
				{
					g_pcStruct.defaultDriver = curDevice;

					Logger::Log(
					    "INFO DirectDraw - Override default driver, new driver >>>%s,%s<<< selected.\n", curDevice->driverName, curDevice->driverDesc
					);
				}

				deviceIter = ++curDeviceCount;
			}
			while ( curDeviceCount < g_pcStruct.ddDeviceCount );
		}

		if ( g_pcStruct.defaultDriver )
		{
			const char* hasHardwareAccel = "has";

			if ( ! g_pcStruct.defaultDriver->hasHardwareAccel )
				hasHardwareAccel = "has no";

			Logger::Log("INFO DirectDraw - Device %s hardware acceleration.\n", hasHardwareAccel);
		}
		else
		{
			Logger::Log("INFO No available DirectDraw devices.\n");
		}

		Logger::Log("\n");

		g_pcStruct.softwareRenderMode = 0;

		if ( g_pcStruct.defaultDriver->isPrimaryDisplay )
		{
			Logger::Log("INFO Direct3D - Searching for hardware RGB driver.\n");
			ExamineDevice* defaultDriver = g_pcStruct.defaultDriver;

			int32_t hardwareRGBCount = 0;
			int32_t softwareRGBCount = 0;

			InterfaceDevice* hardwareRGBDevice;
			InterfaceDevice* softwareRGBDevice;

			if ( g_pcStruct.defaultDriver->deviceCount > 0 )
			{
				int32_t curInterfaceDev = 0;

				InterfaceDevice** softwareDevices = &softwareRGBDevice;
				InterfaceDevice** hardwareDevices = &hardwareRGBDevice;

				do
				{
					InterfaceDevice* interfaceDevice = &g_pcStruct.defaultDriver->interfaceDevices[curInterfaceDev];

					if ( interfaceDevice->isHardwareAccelerated )
					{
						if ( (interfaceDevice->hwDeviceDesc.dcmColorModel & 2) != 0 )
						{
							*hardwareDevices++ = interfaceDevice;
							++hardwareRGBCount;
						}
					}
					else if ( (interfaceDevice->hwDeviceDesc.dcmColorModel & 2) != 0 )
					{
						*softwareDevices = interfaceDevice;
						++softwareRGBCount;
						++softwareDevices;
					}

					curInterfaceDev = ++count;
				}
				while ( count < defaultDriver->deviceCount );
			}

			if ( hardwareRGBCount && hardwareRGBDevice->hasTexturing )
			{
				g_pcStruct.selectedInterfaceDevice = hardwareRGBDevice;
				g_pcStruct.fullscreenMode = 1;
				Logger::Log(
				    "INFO Direct3D - Hardware RGB driver >>>%s,%s<<< selected\n", hardwareRGBDevice->baseName, hardwareRGBDevice->description
				);
			}
			else
			{
				if ( g_checkAvailableMem )
				{
					g_pcStruct.fullscreenMode = 0;

					if ( softwareRGBCount )
					{
						g_pcStruct.selectedInterfaceDevice = softwareRGBDevice;
						Logger::Log("INFO Direct3D - Driver >>>%s,%s<<< selected\n", softwareRGBDevice->baseName, softwareRGBDevice->description);
						defaultDriver = g_pcStruct.defaultDriver;
					}

					defaultDriver->hasHardwareAccel = 0;
				}
				else
				{
					Logger::Log("INFO Direct3D - Found no hardware RGB drivers.\n");
					g_pcStruct.defaultDriver = g_pcStruct.examineDevices;

					Logger::Log(
					    "INFO DirectDraw - Resetting to default driver >>>%s,%s<<<.\n", g_pcStruct.examineDevices[0].driverName,
					    g_pcStruct.examineDevices[0].driverDesc
					);

					g_pcStruct.fullscreenMode = 1;
					g_pcStruct.softwareRenderMode = 1;
				}
			}
		}
		else
		{
			int32_t deviceCount = g_pcStruct.defaultDriver->deviceCount;
			int16_t interfaceDeviceIndex = 0;

			if ( deviceCount > 0 )
			{
				int32_t loopCounter1 = 0;

				while ( ! g_pcStruct.defaultDriver->interfaceDevices[loopCounter1].isHardwareAccelerated )
				{
					loopCounter1 = ++interfaceDeviceIndex;

					if ( interfaceDeviceIndex >= deviceCount )
					{
						deviceCount = -1;
						break;
					}
				}

				if ( deviceCount > 0 )
				{
					g_pcStruct.fullscreenMode = 1;
					g_pcStruct.selectedInterfaceDevice = &g_pcStruct.defaultDriver->interfaceDevices[interfaceDeviceIndex];

					Logger::Log(
					    "INFO Direct3D - Hardware driver >>>%s, %s<<< selected\n",
					    g_pcStruct.defaultDriver->interfaceDevices[interfaceDeviceIndex].baseName,
					    g_pcStruct.defaultDriver->interfaceDevices[interfaceDeviceIndex].description
					);
				}
			}

			if ( deviceCount <= 0 )
			{
				g_pcStruct.defaultDriver = g_pcStruct.examineDevices;

				Logger::Log("INFO Direct3D - No suitable hardware drivers found, scanning software renderers.\n");
				Logger::Log(
				    "INFO DirectDraw - Resetting to default driver >>>%s,%s<<<.\n", g_pcStruct.defaultDriver->driverName,
				    g_pcStruct.defaultDriver->driverDesc
				);

				if ( g_checkAvailableMem && g_pcStruct.defaultDriver->deviceCount )
				{
					g_pcStruct.selectedInterfaceDevice = g_pcStruct.defaultDriver->interfaceDevices;
					g_pcStruct.fullscreenMode = 0;
					g_pcStruct.softwareRenderMode = 0;

					Logger::Log("INFO Direct3D - Using software renderer.\n");
				}
				else
				{
					g_pcStruct.fullscreenMode = 1;
					g_pcStruct.softwareRenderMode = 1;

					Logger::Log("INFO Direct3D - Not using any drivers, resetting to native software renderer.\n");
				}
			}
		}

		if ( g_pcStruct.selectedInterfaceDevice )
		{
			const char* d3dHardwareStatus = "has";

			if ( ! g_pcStruct.selectedInterfaceDevice->isHardwareAccelerated )
				d3dHardwareStatus = "has no";

			Logger::Log("INFO Direct3D - Device %s hardware acceleration.\n", d3dHardwareStatus);
		}
		else
		{
			Logger::Log("INFO No available Direct3D devices.\n");
		}

		Logger::Log("\n");
		const char* fullscreenStatus = "TRUE";

		if ( ! g_pcStruct.fullscreenMode )
			fullscreenStatus = "FALSE";

		Logger::Log("INFO Fullscreen mode is %s.\n", fullscreenStatus);
		const char* softwareRenderStatus = "TRUE";

		if ( ! g_pcStruct.softwareRenderMode )
			softwareRenderStatus = "FALSE";

		Logger::Log("INFO Native software render mode is %s.\n", softwareRenderStatus);

		ExamineDevice* defaultDriver = g_pcStruct.defaultDriver;
		int32_t softwareRenderMode = g_pcStruct.softwareRenderMode;

		g_pcStruct.mode = g_pcStruct.defaultDriver->displayModes;

		if ( g_pcStruct.softwareRenderMode )
		{
			int32_t displayModeLoopIndex = 0;

			if ( g_pcStruct.defaultDriver->displayModeCount > 0 )
			{
				int32_t displayModeCounter = 0;

				do
				{
					if ( defaultDriver->displayModes[displayModeCounter].width == 640 &&
					     defaultDriver->displayModes[displayModeCounter].height == 480 &&
					     defaultDriver->displayModes[displayModeCounter].rgbBitCount == 16 )
					{
						g_pcStruct.mode = defaultDriver->displayModes;
						defaultDriver->selectedDisplayModeIndex = displayModeCounter;
						defaultDriver = g_pcStruct.defaultDriver;
					}

					displayModeCounter = ++displayModeLoopIndex;
				}
				while ( displayModeLoopIndex < defaultDriver->displayModeCount );

				softwareRenderMode = g_pcStruct.softwareRenderMode;
			}
		}
		else
		{
			int32_t deviceLoopIndex = 0;

			if ( g_pcStruct.defaultDriver->displayModeCount > 0 )
			{
				while ( 1 )
				{
					int32_t defaultWidth = defaultDriver->displayModes[deviceLoopIndex].width;

					if ( g_checkAvailableMem )
					{
						if ( defaultWidth == 320 && defaultDriver->displayModes[deviceLoopIndex].height == 240 &&
						     defaultDriver->displayModes[deviceLoopIndex].rgbBitCount == 16 )
						{
							g_pcStruct.mode = &defaultDriver->displayModes[deviceLoopIndex];
							defaultDriver->selectedDisplayModeIndex = deviceLoopIndex;
							defaultDriver = g_pcStruct.defaultDriver;
						}
					}
					else if ( defaultWidth == 640 && defaultDriver->displayModes[deviceLoopIndex].height == 480 &&
					          defaultDriver->displayModes[deviceLoopIndex].rgbBitCount == 16 )
					{
						g_pcStruct.mode = &defaultDriver->displayModes[deviceLoopIndex];
						defaultDriver->selectedDisplayModeIndex = deviceLoopIndex;
						defaultDriver = g_pcStruct.defaultDriver;
					}

					if ( ++deviceLoopIndex >= defaultDriver->displayModeCount )
						break;
				}

				softwareRenderMode = g_pcStruct.softwareRenderMode;
			}
		}

		g_renderMode = (RenderMode)(2 - (softwareRenderMode != 0));
		const char* renderMode = "native software render";

		if ( g_renderMode != SoftwareRenderer )
			renderMode = "Direct3d render";

		Logger::Log("INFO RenderMode set to %s mode.\n", renderMode);
		Logger::Log("INFO Screen mode %dx%dx%d selected.\n", g_pcStruct.mode->width, g_pcStruct.mode->height, g_pcStruct.mode->rgbBitCount);
		Logger::Log("\n");

		DestroyWindow(g_windowData.mainHwnd);

		while ( GetMessageA(&g_windowData.wndEventMsg, 0, 0, 0) )
		{
			if ( g_windowData.wndEventMsg.message == WM_QUIT )
				break;

			if ( ! g_windowData.mainHwnd || ! TranslateAcceleratorA(g_windowData.mainHwnd, g_windowData.hAccTable, &g_windowData.wndEventMsg) )
			{
				TranslateMessage(&g_windowData.wndEventMsg);
				DispatchMessageA(&g_windowData.wndEventMsg);
			}
		}

		Logger::Log("END SELECT DRIVERS\n");
		Logger::Log("------------------\n\n");

		return 1;
	}

	// $FUNC 004A6B30 [IMPLEMENTED]
	int32_t BuildWindow()
	{
		memset(&g_windowData.wndClass, 0, sizeof(g_windowData.wndClass));

		static int32_t g_unused1 = 0;
		static int32_t g_unused2 = 0;

		g_windowData.wndClass.cbSize = 48;

		CoInitialize(0);

		g_windowData.wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		g_windowData.wndClass.lpfnWndProc = NormalWndProc;
		g_windowData.wndClass.cbClsExtra = 0;
		g_windowData.wndClass.cbWndExtra = 0;
		g_windowData.wndClass.hInstance = g_windowData.hInstance;
		g_windowData.wndClass.hIcon = LoadIconA(g_windowData.hInstance, "AppIcon");
		g_windowData.wndClass.lpszMenuName = "AppMenu";
		g_windowData.wndClass.lpszClassName = "Toy2";
		g_windowData.wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

		if ( RegisterClassExA(&g_windowData.wndClass) )
		{
			HWND window =
			    CreateWindowExA(WS_EX_TOPMOST, "Toy2", "Toy2", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, g_windowData.hInstance, 0);

			g_windowData.mainHwnd = window;
			g_d3dAppI.hwnd = window;

			if ( window )
			{
				UpdateWindow(window);
				g_windowData.hAccTable = LoadAcceleratorsA(g_windowData.hInstance, "AppAccel");

				if ( SystemParametersInfoA(SPI_SETSCREENSAVERRUNNING, 1u, &g_sysParamsInfo, 0) )
					atexit(SysParmsOnExit);

				static int32_t g_windowCreationError = 0;

				return 1;
			}
			else
			{
				Logger::LogLn("CreateWindowEx failed");
				return 0;
			}
		}
		else
		{
			int32_t lastError = GetLastError();

			char* buffer = NULL;
			FormatMessageA(
			    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, lastError,
			    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 0, 0
			);
			MessageBoxA(0, buffer, "GetLastError", MB_ICONINFORMATION);
			LocalFree(buffer);
			return 0;
		}
	}

	// $FUNC 00408D30 [IMPLEMENTED]
	int32_t WINAPI EnumerateDevices(LPGUID guid, LPSTR driverDesc, LPSTR driverName, LPVOID lpContext)
	{
		memset(&g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount], 0, sizeof(g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount]));

		ExamineDevice* curDevice = &g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount];

		LPDIRECTDRAW lpDD;
		if ( DirectDrawCreate(guid, &lpDD, 0) < 0 )
		{
			curDevice->dd2->Release();
			memset(&g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount], 0, sizeof(g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount]));
			return 1;
		}

		HRESULT query1Result = lpDD->QueryInterface(IID_IDirectDraw2, (void**)&curDevice->dd2);

		if ( query1Result < 0 )
			Logger::LogDDError("tempDD->QueryInterface(IID_IDirectDraw2, (LPVOID*)&(ddinfo->DDDevice))", query1Result);

		if ( lpDD )
		{
			lpDD->Release();
			lpDD = 0;
		}

		g_changingCoopLevel = 1;
		curDevice->dd2->SetCooperativeLevel(g_windowData.mainHwnd, 81);
		g_changingCoopLevel = 0;

		memset(&curDevice->ddCaps, 0, sizeof(curDevice->ddCaps));

		curDevice->ddCaps.dwSize = 380;

		if ( curDevice->dd2->GetCaps(&curDevice->ddCaps, 0) < 0 )
		{
			curDevice->dd2->Release();
			memset(&g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount], 0, sizeof(g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount]));
			return 1;
		}

		DDSCAPS ddsCaps;
		ddsCaps.dwCaps = 4096;

		// TODO: Honestly who knows whats going on here (compiler artifact?)
		if ( guid && guid != (GUID*)2 && g_checkAvailableMem != 1 )
		{
			HRESULT availMemResult =
			    curDevice->dd2->GetAvailableVidMem(&ddsCaps, (LPDWORD)&curDevice->texVidMemTot, (LPDWORD)&curDevice->texVidMemFree);

			if ( availMemResult < 0 )
				Logger::LogDDError("ddinfo->DDDevice->GetAvailableVidMem(&caps, &ddinfo->TexVidMemTotal, &ddinfo->TexVidMemFree)", availMemResult);

			int32_t texVidMemFree = curDevice->texVidMemFree;

			if ( texVidMemFree < 0x200000 )
			{
				Logger::Log(
				    "DIRECT DRAW DEVICE : Device %s,%s reports less than %d texture memory free - skipping.\n\n", driverName, driverDesc, 0x200000
				);

				curDevice->dd2->Release();
				memset(&g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount], 0, sizeof(g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount]));
				return 1;
			}

			curDevice->hasLowVideoMem = texVidMemFree < 0x400000;
		}

		int32_t dwCaps = curDevice->ddCaps.dwCaps;

		curDevice->valid = 1;
		curDevice->has3DSupport = dwCaps & 1;
		curDevice->hasHardwareAccel = (~dwCaps >> 25) & 1;

		int32_t dwSVBCaps = curDevice->ddCaps.dwSVBCaps;

		curDevice->hasVideoDMA = dwSVBCaps >> 31;

		if ( dwSVBCaps < 0 )
		{
			int32_t dwSVBCKeyCaps = curDevice->ddCaps.dwSVBCKeyCaps;
			int32_t dwSVBFXCaps = curDevice->ddCaps.dwSVBFXCaps;

			curDevice->svbCaps = dwSVBCaps;
			curDevice->svbcKeyCaps = dwSVBCKeyCaps;
			curDevice->svbFxCaps = dwSVBFXCaps;
		}

		strncpy(curDevice->driverName, driverName, 64u);
		strncpy(curDevice->driverDesc, driverDesc, 64u);

		if ( ! guid || guid == (GUID*)2 )
		{
			curDevice->hasHardwareAccel = 0;
		}
		else
		{
			curDevice->isPrimaryDisplay = 0;
			curDevice->deviceGUID = *guid;
		}

		HRESULT enumModesResult = curDevice->dd2->EnumDisplayModes(0, 0, curDevice, EnumDisplayModes);

		if ( enumModesResult < 0 )
			Logger::LogDDError("ddinfo->DDDevice->EnumDisplayModes(0, 0, (LPVOID)ddinfo, ExamineDDModesEnumCallback)", enumModesResult);

		qsort(curDevice->displayModes, curDevice->displayModeCount, 12, SortDisplayModes);

		Logger::Log("DIRECT DRAW DEVICE BASE NAME : %s.\n", curDevice->driverName);
		Logger::Log("DIRECT DRAW DEVICE DESCRIPTION : %s.\n", curDevice->driverDesc);

		const char* has3D = "TRUE";

		if ( ! curDevice->has3DSupport )
			has3D = "FALSE";

		Logger::Log("DIRECT DRAW DEVICE 3D SUPPORT : %s.\n", has3D);

		const char* hasDDHardware = "TRUE";

		if ( ! curDevice->hasHardwareAccel )
			hasDDHardware = "FALSE";

		Logger::Log("DIRECT DRAW DEVICE HARDWARE : %s.\n", hasDDHardware);

		const char* hasVideoDMA = "TRUE";

		if ( ! curDevice->hasVideoDMA )
			hasVideoDMA = "FALSE";

		Logger::Log("DIRECT DRAW SYSTEM-VIDEO DMA : %s.\n", hasVideoDMA);

		if ( guid )
		{
			const char* hasLowVidMem = "TRUE";

			if ( ! curDevice->hasLowVideoMem )
				hasLowVidMem = "FALSE";

			Logger::Log("DIRECT DRAW LOW VIDEO MEMORY STATUS  : %s.\n", hasLowVidMem);
		}

		Logger::Log("DIRECT DRAW DEVICE SCREEN MODES.\n");
		Logger::Log("\n");

		int32_t iter = 0;

		if ( curDevice->displayModeCount > 0 )
		{
			DisplayMode* item = &curDevice->displayModes[0];

			do
			{
				Logger::Log("DDMODE %i - %dx%dx%d bit.\n", iter++, item->rgbBitCount, item->width, item->height);
				++item;
			}
			while ( iter < curDevice->displayModeCount );
		}

		HRESULT query3D2Result = curDevice->dd2->QueryInterface(IID_IDirect3D2, (void**)&curDevice->dd3D);

		if ( query3D2Result < 0 )
			Logger::LogDDError("ddinfo->DDDevice->QueryInterface(IID_IDirect3D2, (LPVOID *) &ddinfo->D3DDevice)", query3D2Result);

		HRESULT enumDevicesResult = curDevice->dd3D->EnumDevices(EnumDevices, curDevice);

		if ( enumDevicesResult < 0 )
			Logger::LogDDError("ddinfo->D3DDevice->EnumDevices(ExamineD3DEnumCallback, (LPVOID*)ddinfo)", enumDevicesResult);

		++g_pcStruct.ddDeviceCount;

		if ( curDevice->dd3D )
		{
			curDevice->dd3D->Release();
			curDevice->dd3D = 0;
		}

		if ( curDevice->dd2 )
		{
			curDevice->dd2->Release();
			curDevice->dd2 = 0;
		}

		return 1;
	}

	// $FUNC 00408D30 [IMPLEMENTED]
	HRESULT WINAPI EnumDisplayModes(LPDDSURFACEDESC surfaceDesc, LPVOID context)
	{
		ExamineDevice* examineContext = (ExamineDevice*)context;

		int32_t dwWidth = surfaceDesc->dwWidth;

		if ( dwWidth < 320 || surfaceDesc->dwHeight < 200 )
			return 1;

		DisplayMode* displayMode = &examineContext->displayModes[examineContext->displayModeCount];

		displayMode->width = dwWidth;
		displayMode->height = surfaceDesc->dwHeight;
		displayMode->rgbBitCount = surfaceDesc->ddpfPixelFormat.dwRGBBitCount;

		int32_t curDisplayModeCount = examineContext->displayModeCount + 1;
		examineContext->displayModeCount = curDisplayModeCount;

		return curDisplayModeCount != 128;
	}

	// $FUNC 00409130 [IMPLEMENTED]
	HRESULT WINAPI
	EnumDevices(LPGUID guid, LPSTR deviceDesc, LPSTR deviceName, LPD3DDEVICEDESC d3DHWDeviceDesc, LPD3DDEVICEDESC d3DHELDeviceDesc, LPVOID context)
	{
		LPD3DDEVICEDESC hwDeviceDesc = d3DHWDeviceDesc;
		ExamineDevice* examineContext = (ExamineDevice*)context;

		InterfaceDevice* item = &examineContext->interfaceDevices[examineContext->deviceCount];

		memset(item, 0, sizeof(InterfaceDevice));

		item->isHardwareAccelerated = d3DHWDeviceDesc->dcmColorModel != 0;

		item->guid = *guid;

		lstrcpyA(item->baseName, deviceName);
		lstrcpyA(item->description, deviceDesc);

		if ( ! d3DHWDeviceDesc->dcmColorModel )
			hwDeviceDesc = d3DHELDeviceDesc;

		memcpy(&item->hwDeviceDesc, hwDeviceDesc, sizeof(item->hwDeviceDesc));

		int32_t dwDeviceZBufferBitDepth = item->hwDeviceDesc.dwDeviceZBufferBitDepth;
		int32_t hasTexturing = item->hwDeviceDesc.dpcTriCaps.dwTextureCaps & 1;

		item->isSquareTexturesOnly = (item->hwDeviceDesc.dpcTriCaps.dwTextureCaps >> 5) & 1;

		int32_t hasAlphaBlending = (item->hwDeviceDesc.dpcTriCaps.dwSrcBlendCaps >> 11) & 1;

		item->hasTexturing = hasTexturing;
		item->hasAlphaBlending = hasAlphaBlending;
		item->hasZBuffer = dwDeviceZBufferBitDepth != 0;

		Logger::Log("\n");
		Logger::Log("DIRECT 3D DEVICE BASE NAME : %s.\n", item->baseName);
		Logger::Log("DIRECT 3D DEVICE DESCRIPTION : %s.\n", item->description);

		const char* texFlag = "TRUE";
		if ( ! item->hasTexturing )
			texFlag = "FALSE";

		Logger::Log("DIRECT 3D DEVICE TextureFlag\t:\t%s.\n", texFlag);

		const char* zBufferFlag = "TRUE";
		if ( ! item->hasZBuffer )
			zBufferFlag = "FALSE";

		Logger::Log("DIRECT 3D DEVICE ZBufferFlag : %s.\n", zBufferFlag);

		const char* hardwareAccelFlag = "TRUE";
		if ( ! item->isHardwareAccelerated )
			hardwareAccelFlag = "FALSE";

		Logger::Log("DIRECT 3D DEVICE HardwareAccel : %s.\n", hardwareAccelFlag);

		const char* squareOnlyFlag = "TRUE";
		if ( ! item->isSquareTexturesOnly )
			squareOnlyFlag = "FALSE";

		Logger::Log("DIRECT 3D DEVICE SquareOnly : %s.\n", squareOnlyFlag);

		const char* alphaBlending = "ON";
		if ( ! item->hasAlphaBlending )
			alphaBlending = "OFF";

		Logger::Log("DIRECT 3D DEVICE Alpha blending is %s.\n", alphaBlending);
		Logger::Log("\n");

		item->valid = 1;
		++examineContext->deviceCount;

		return 1;
	}

	// $FUNC 00408CD0 [IMPLEMENTED]
	int32_t SortDisplayModes(const void* modeA, const void* modeB)
	{
		DisplayMode* displayModeA = (DisplayMode*)modeA;
		DisplayMode* displayModeB = (DisplayMode*)modeB;

		int32_t bitCountA = displayModeA->rgbBitCount;
		int32_t bitCountB = displayModeB->rgbBitCount;

		if ( bitCountB > bitCountA )
			return -1;

		if ( bitCountB < bitCountA )
			return 1;

		if ( displayModeB->width > displayModeA->width )
			return -1;

		if ( displayModeB->width < displayModeA->width )
			return 1;

		int32_t heightA = displayModeA->height;
		int32_t heightB = displayModeB->height;

		if ( heightA >= heightB )
			return heightB < heightA;
		else
			return -1;
	}

	// $FUNC 00409360 [IMPLEMENTED]
	LRESULT WINAPI ProfileWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if ( msg == WM_DESTROY )
		{
			g_windowData.mainHwnd = 0;
			PostQuitMessage(0);
		}

		return DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	// $FUNC 004A6D40 [UNIMPLEMENTED]
	LRESULT WINAPI NormalWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return LRESULT(); }

	// $FUNC 004A6B10 [IMPLEMENTED]
	void SysParmsOnExit() { SystemParametersInfoA(SPI_SETSCREENSAVERRUNNING, g_sysParamsInfo, &g_sysParamsInfo, 0); }
}
