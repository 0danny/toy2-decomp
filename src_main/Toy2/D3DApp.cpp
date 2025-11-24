#include "D3DApp.h"
#include "Logger.h"

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

	static int32_t g_changingCoopLevel = 0;
	static int32_t g_checkAvailableMem = 1;
	static int32_t g_sysParamsInfo;

	// $FUNC 004093A0 [UNFINISHED]
	int32_t BuildProfileMachine()
	{
		memset(&g_pcStruct, 0, sizeof(g_pcStruct));

		Logger::Log("---------------------\n");
		Logger::Log("BEGIN EXAMINE MACHINE\n\n");

		int32_t l_count = 0;

		WNDCLASSA l_wndClass;
		l_wndClass.style = CS_HREDRAW | CS_VREDRAW;
		l_wndClass.lpfnWndProc = ProfileWndProc;
		l_wndClass.cbClsExtra = 0;
		l_wndClass.cbWndExtra = 0;
		l_wndClass.hInstance = g_windowData.hInstance;
		l_wndClass.hIcon = LoadIconA(0, IDI_WINLOGO);
		l_wndClass.hCursor = LoadCursorA(0, IDC_ARROW);
		l_wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		l_wndClass.lpszMenuName = "";
		l_wndClass.lpszClassName = "Examining Machine";

		if ( ! RegisterClassA(&l_wndClass) )
			return 0;

		int32_t l_curDeviceCount = 1;

		g_windowData.mainHwnd = CreateWindowExA(
		    WS_EX_APPWINDOW, "Examining Machine", "Examining Machine",
		    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		    CW_USEDEFAULT, 0, 0, g_windowData.hInstance, 0
		);

		ShowWindow(g_windowData.mainHwnd, 1);
		UpdateWindow(g_windowData.mainHwnd);
		SetFocus(g_windowData.mainHwnd);

		g_pcStruct.ddDeviceCount = 0;

		HRESULT l_ddEnumResult = DirectDrawEnumerateA(EnumerateDevices, 0);
		if ( l_ddEnumResult < 0 )
			Logger::LogDDError("DirectDrawEnumerateA(ExamineDDEnumCallback, 0)", l_ddEnumResult);

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
			int32_t l_deviceIter = 1;

			do
			{
				ExamineDevice* l_curDevice = &g_pcStruct.examineDevices[l_deviceIter];

				if ( l_curDevice->valid && g_pcStruct.examineDevices[l_deviceIter].hasHardwareAccel &&
				     g_pcStruct.examineDevices[l_deviceIter].has3DSupport )
				{
					g_pcStruct.defaultDriver = l_curDevice;

					Logger::Log(
					    "INFO DirectDraw - Override default driver, new driver >>>%s,%s<<< selected.\n", l_curDevice->driverName,
					    l_curDevice->driverDesc
					);
				}

				l_deviceIter = ++l_curDeviceCount;
			}
			while ( l_curDeviceCount < g_pcStruct.ddDeviceCount );
		}

		if ( g_pcStruct.defaultDriver )
		{
			const char* l_hasHardwareAccel = "has";

			if ( ! g_pcStruct.defaultDriver->hasHardwareAccel )
				l_hasHardwareAccel = "has no";

			Logger::Log("INFO DirectDraw - Device %s hardware acceleration.\n", l_hasHardwareAccel);
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
			ExamineDevice* l_defaultDriver = g_pcStruct.defaultDriver;

			int32_t hardwareRGBCount = 0;
			int32_t softwareRGBCount = 0;

			InterfaceDevice* hardwareRGBDevice;
			InterfaceDevice* softwareRGBDevice;

			if ( g_pcStruct.defaultDriver->deviceCount > 0 )
			{
				int32_t l_curInterfaceDev = 0;

				InterfaceDevice** softwareDevices = &softwareRGBDevice;
				InterfaceDevice** hardwareDevices = &hardwareRGBDevice;

				do
				{
					InterfaceDevice* l_interfaceDevice = &g_pcStruct.defaultDriver->interfaceDevices[l_curInterfaceDev];

					if ( l_interfaceDevice->isHardwareAccelerated )
					{
						if ( (l_interfaceDevice->hwDeviceDesc.dcmColorModel & 2) != 0 )
						{
							*hardwareDevices++ = l_interfaceDevice;
							++hardwareRGBCount;
						}
					}
					else if ( (l_interfaceDevice->hwDeviceDesc.dcmColorModel & 2) != 0 )
					{
						*softwareDevices = l_interfaceDevice;
						++softwareRGBCount;
						++softwareDevices;
					}

					l_curInterfaceDev = ++l_count;
				}
				while ( l_count < l_defaultDriver->deviceCount );
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
						l_defaultDriver = g_pcStruct.defaultDriver;
					}

					l_defaultDriver->hasHardwareAccel = 0;
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
		int32_t l_softwareRenderMode = g_pcStruct.softwareRenderMode;

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

				l_softwareRenderMode = g_pcStruct.softwareRenderMode;
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

				l_softwareRenderMode = g_pcStruct.softwareRenderMode;
			}
		}

		g_renderMode = (RenderMode)(2 - (l_softwareRenderMode != 0));
		const char* l_renderMode = "native software render";

		if ( g_renderMode != SoftwareRenderer )
			l_renderMode = "Direct3d render";

		Logger::Log("INFO RenderMode set to %s mode.\n", l_renderMode);
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
			HWND l_window =
			    CreateWindowExA(WS_EX_TOPMOST, "Toy2", "Toy2", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, g_windowData.hInstance, 0);

			g_windowData.mainHwnd = l_window;
			g_d3dAppI.hwnd = l_window;

			if ( l_window )
			{
				UpdateWindow(l_window);
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
			int32_t l_lastError = GetLastError();

			char* l_buffer = NULL;
			FormatMessageA(
			    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, l_lastError,
			    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), l_buffer, 0, 0
			);
			MessageBoxA(0, l_buffer, "GetLastError", MB_ICONINFORMATION);
			LocalFree(l_buffer);
			return 0;
		}
	}

	// $FUNC 00408D30 [IMPLEMENTED]
	int32_t WINAPI EnumerateDevices(LPGUID p_guid, LPSTR p_driverDesc, LPSTR p_driverName, LPVOID p_lpContext)
	{
		memset(&g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount], 0, sizeof(g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount]));

		ExamineDevice* l_curDevice = &g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount];

		LPDIRECTDRAW l_lpDD;
		if ( DirectDrawCreate(p_guid, &l_lpDD, 0) < 0 )
		{
			l_curDevice->dd2->Release();
			memset(&g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount], 0, sizeof(g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount]));
			return 1;
		}

		HRESULT l_query1Result = l_lpDD->QueryInterface(IID_IDirectDraw2, (void**)&l_curDevice->dd2);

		if ( l_query1Result < 0 )
			Logger::LogDDError("tempDD->QueryInterface(IID_IDirectDraw2, (LPVOID*)&(ddinfo->DDDevice))", l_query1Result);

		if ( l_lpDD )
		{
			l_lpDD->Release();
			l_lpDD = 0;
		}

		g_changingCoopLevel = 1;
		l_curDevice->dd2->SetCooperativeLevel(g_windowData.mainHwnd, 81);
		g_changingCoopLevel = 0;

		memset(&l_curDevice->ddCaps, 0, sizeof(l_curDevice->ddCaps));

		l_curDevice->ddCaps.dwSize = 380;

		if ( l_curDevice->dd2->GetCaps(&l_curDevice->ddCaps, 0) < 0 )
		{
			l_curDevice->dd2->Release();
			memset(&g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount], 0, sizeof(g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount]));
			return 1;
		}

		DDSCAPS l_ddsCaps;
		l_ddsCaps.dwCaps = 4096;

		// Honestly who knows whats going on here (compiler artifact?)
		if ( p_guid && p_guid != (GUID*)2 && g_checkAvailableMem != 1 )
		{
			HRESULT l_availMemResult =
			    l_curDevice->dd2->GetAvailableVidMem(&l_ddsCaps, (LPDWORD)&l_curDevice->texVidMemTot, (LPDWORD)&l_curDevice->texVidMemFree);

			if ( l_availMemResult < 0 )
				Logger::LogDDError("ddinfo->DDDevice->GetAvailableVidMem(&caps, &ddinfo->TexVidMemTotal, &ddinfo->TexVidMemFree)", l_availMemResult);

			int32_t l_texVidMemFree = l_curDevice->texVidMemFree;

			if ( l_texVidMemFree < 0x200000 )
			{
				Logger::Log(
				    "DIRECT DRAW DEVICE : Device %s,%s reports less than %d texture memory free - skipping.\n\n", p_driverName, p_driverDesc, 0x200000
				);

				l_curDevice->dd2->Release();
				memset(&g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount], 0, sizeof(g_pcStruct.examineDevices[g_pcStruct.ddDeviceCount]));
				return 1;
			}

			l_curDevice->hasLowVideoMem = l_texVidMemFree < 0x400000;
		}

		int32_t l_dwCaps = l_curDevice->ddCaps.dwCaps;

		l_curDevice->valid = 1;
		l_curDevice->has3DSupport = l_dwCaps & 1;
		l_curDevice->hasHardwareAccel = (~l_dwCaps >> 25) & 1;

		int32_t l_dwSVBCaps = l_curDevice->ddCaps.dwSVBCaps;

		l_curDevice->hasVideoDMA = l_dwSVBCaps >> 31;

		if ( l_dwSVBCaps < 0 )
		{
			int32_t l_dwSVBCKeyCaps = l_curDevice->ddCaps.dwSVBCKeyCaps;
			int32_t l_dwSVBFXCaps = l_curDevice->ddCaps.dwSVBFXCaps;

			l_curDevice->svbCaps = l_dwSVBCaps;
			l_curDevice->svbcKeyCaps = l_dwSVBCKeyCaps;
			l_curDevice->svbFxCaps = l_dwSVBFXCaps;
		}

		strncpy(l_curDevice->driverName, p_driverName, 64u);
		strncpy(l_curDevice->driverDesc, p_driverDesc, 64u);

		if ( ! p_guid || p_guid == (GUID*)2 )
		{
			l_curDevice->hasHardwareAccel = 0;
		}
		else
		{
			l_curDevice->isPrimaryDisplay = 0;
			l_curDevice->deviceGUID = *p_guid;
		}

		HRESULT l_enumModesResult = l_curDevice->dd2->EnumDisplayModes(0, 0, l_curDevice, EnumDisplayModes);

		if ( l_enumModesResult < 0 )
			Logger::LogDDError("ddinfo->DDDevice->EnumDisplayModes(0, 0, (LPVOID)ddinfo, ExamineDDModesEnumCallback)", l_enumModesResult);

		qsort(l_curDevice->displayModes, l_curDevice->displayModeCount, 12, SortDisplayModes);

		Logger::Log("DIRECT DRAW DEVICE BASE NAME : %s.\n", l_curDevice->driverName);
		Logger::Log("DIRECT DRAW DEVICE DESCRIPTION : %s.\n", l_curDevice->driverDesc);

		const char* l_has3D = "TRUE";

		if ( ! l_curDevice->has3DSupport )
			l_has3D = "FALSE";

		Logger::Log("DIRECT DRAW DEVICE 3D SUPPORT : %s.\n", l_has3D);

		const char* l_hasDDHardware = "TRUE";

		if ( ! l_curDevice->hasHardwareAccel )
			l_hasDDHardware = "FALSE";

		Logger::Log("DIRECT DRAW DEVICE HARDWARE : %s.\n", l_hasDDHardware);

		const char* l_hasVideoDMA = "TRUE";

		if ( ! l_curDevice->hasVideoDMA )
			l_hasVideoDMA = "FALSE";

		Logger::Log("DIRECT DRAW SYSTEM-VIDEO DMA : %s.\n", l_hasVideoDMA);

		if ( p_guid )
		{
			const char* l_hasLowVidMem = "TRUE";

			if ( ! l_curDevice->hasLowVideoMem )
				l_hasLowVidMem = "FALSE";

			Logger::Log("DIRECT DRAW LOW VIDEO MEMORY STATUS  : %s.\n", l_hasLowVidMem);
		}

		Logger::Log("DIRECT DRAW DEVICE SCREEN MODES.\n");
		Logger::Log("\n");

		int32_t l_iter = 0;

		if ( l_curDevice->displayModeCount > 0 )
		{
			DisplayMode* l_item = &l_curDevice->displayModes[0];

			do
			{
				Logger::Log("DDMODE %i - %dx%dx%d bit.\n", l_iter++, l_item->rgbBitCount, l_item->width, l_item->height);
				++l_item;
			}
			while ( l_iter < l_curDevice->displayModeCount );
		}

		HRESULT l_query3D2Result = l_curDevice->dd2->QueryInterface(IID_IDirect3D2, (void**)&l_curDevice->dd3D);

		if ( l_query3D2Result < 0 )
			Logger::LogDDError("ddinfo->DDDevice->QueryInterface(IID_IDirect3D2, (LPVOID *) &ddinfo->D3DDevice)", l_query3D2Result);

		HRESULT l_enumDevicesResult = l_curDevice->dd3D->EnumDevices(EnumDevices, l_curDevice);

		if ( l_enumDevicesResult < 0 )
			Logger::LogDDError("ddinfo->D3DDevice->EnumDevices(ExamineD3DEnumCallback, (LPVOID*)ddinfo)", l_enumDevicesResult);

		++g_pcStruct.ddDeviceCount;

		if ( l_curDevice->dd3D )
		{
			l_curDevice->dd3D->Release();
			l_curDevice->dd3D = 0;
		}

		if ( l_curDevice->dd2 )
		{
			l_curDevice->dd2->Release();
			l_curDevice->dd2 = 0;
		}

		return 1;
	}

	// $FUNC 00408D30 [IMPLEMENTED]
	HRESULT WINAPI EnumDisplayModes(LPDDSURFACEDESC p_surfaceDesc, LPVOID p_context)
	{
		ExamineDevice* l_context = (ExamineDevice*)p_context;

		int32_t l_dwWidth = p_surfaceDesc->dwWidth;

		if ( l_dwWidth < 320 || p_surfaceDesc->dwHeight < 200 )
			return 1;

		DisplayMode* l_displayMode = &l_context->displayModes[l_context->displayModeCount];

		l_displayMode->width = l_dwWidth;
		l_displayMode->height = p_surfaceDesc->dwHeight;
		l_displayMode->rgbBitCount = p_surfaceDesc->ddpfPixelFormat.dwRGBBitCount;

		int32_t l_curDisplayModeCount = l_context->displayModeCount + 1;
		l_context->displayModeCount = l_curDisplayModeCount;

		return l_curDisplayModeCount != 128;
	}

	// $FUNC 00409130 [IMPLEMENTED]
	HRESULT WINAPI EnumDevices(
	    LPGUID p_guid,
	    LPSTR p_deviceDesc,
	    LPSTR p_deviceName,
	    LPD3DDEVICEDESC p_d3DHWDeviceDesc,
	    LPD3DDEVICEDESC p_d3DHELDeviceDesc,
	    LPVOID p_context
	)
	{
		LPD3DDEVICEDESC l_hwDeviceDesc = p_d3DHWDeviceDesc;
		ExamineDevice* l_context = (ExamineDevice*)p_context;

		InterfaceDevice* l_item = &l_context->interfaceDevices[l_context->deviceCount];

		memset(l_item, 0, sizeof(InterfaceDevice));

		l_item->isHardwareAccelerated = p_d3DHWDeviceDesc->dcmColorModel != 0;

		l_item->guid = *p_guid;

		lstrcpyA(l_item->baseName, p_deviceName);
		lstrcpyA(l_item->description, p_deviceDesc);

		if ( ! p_d3DHWDeviceDesc->dcmColorModel )
			l_hwDeviceDesc = p_d3DHELDeviceDesc;

		memcpy(&l_item->hwDeviceDesc, l_hwDeviceDesc, sizeof(l_item->hwDeviceDesc));

		int32_t l_dwDeviceZBufferBitDepth = l_item->hwDeviceDesc.dwDeviceZBufferBitDepth;
		int32_t l_hasTexturing = l_item->hwDeviceDesc.dpcTriCaps.dwTextureCaps & 1;

		l_item->isSquareTexturesOnly = (l_item->hwDeviceDesc.dpcTriCaps.dwTextureCaps >> 5) & 1;

		int32_t l_hasAlphaBlending = (l_item->hwDeviceDesc.dpcTriCaps.dwSrcBlendCaps >> 11) & 1;

		l_item->hasTexturing = l_hasTexturing;
		l_item->hasAlphaBlending = l_hasAlphaBlending;
		l_item->hasZBuffer = l_dwDeviceZBufferBitDepth != 0;

		Logger::Log("\n");
		Logger::Log("DIRECT 3D DEVICE BASE NAME : %s.\n", l_item->baseName);
		Logger::Log("DIRECT 3D DEVICE DESCRIPTION : %s.\n", l_item->description);

		const char* l_texFlag = "TRUE";
		if ( ! l_item->hasTexturing )
			l_texFlag = "FALSE";

		Logger::Log("DIRECT 3D DEVICE TextureFlag\t:\t%s.\n", l_texFlag);

		const char* l_zBufferFlag = "TRUE";
		if ( ! l_item->hasZBuffer )
			l_zBufferFlag = "FALSE";

		Logger::Log("DIRECT 3D DEVICE ZBufferFlag : %s.\n", l_zBufferFlag);

		const char* l_hardwareAccelFlag = "TRUE";
		if ( ! l_item->isHardwareAccelerated )
			l_hardwareAccelFlag = "FALSE";

		Logger::Log("DIRECT 3D DEVICE HardwareAccel : %s.\n", l_hardwareAccelFlag);

		const char* l_squareOnlyFlag = "TRUE";
		if ( ! l_item->isSquareTexturesOnly )
			l_squareOnlyFlag = "FALSE";

		Logger::Log("DIRECT 3D DEVICE SquareOnly : %s.\n", l_squareOnlyFlag);

		const char* l_alphaBlending = "ON";
		if ( ! l_item->hasAlphaBlending )
			l_alphaBlending = "OFF";

		Logger::Log("DIRECT 3D DEVICE Alpha blending is %s.\n", l_alphaBlending);
		Logger::Log("\n");

		l_item->valid = 1;
		++l_context->deviceCount;

		return 1;
	}

	// $FUNC 00408CD0 [IMPLEMENTED]
	int32_t SortDisplayModes(const void* p_modeA, const void* p_modeB)
	{
		DisplayMode* l_modeA = (DisplayMode*)p_modeA;
		DisplayMode* l_modeB = (DisplayMode*)p_modeB;

		int32_t l_bitCountA = l_modeA->rgbBitCount;
		int32_t l_bitCountB = l_modeB->rgbBitCount;

		if ( l_bitCountB > l_bitCountA )
			return -1;

		if ( l_bitCountB < l_bitCountA )
			return 1;

		if ( l_modeB->width > l_modeA->width )
			return -1;

		if ( l_modeB->width < l_modeA->width )
			return 1;

		int32_t l_heightA = l_modeA->height;
		int32_t l_heightB = l_modeB->height;

		if ( l_heightA >= l_heightB )
			return l_heightB < l_heightA;
		else
			return -1;
	}

	// $FUNC 00409360 [IMPLEMENTED]
	LRESULT WINAPI ProfileWndProc(HWND p_hWnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
	{
		if ( p_msg == WM_DESTROY )
		{
			g_windowData.mainHwnd = 0;
			PostQuitMessage(0);
		}

		return DefWindowProcA(p_hWnd, p_msg, p_wParam, p_lParam);
	}

	LRESULT __stdcall NormalWndProc(HWND p_hWnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam) { return LRESULT(); }

	void SysParmsOnExit() { SystemParametersInfoA(SPI_SETSCREENSAVERRUNNING, g_sysParamsInfo, &g_sysParamsInfo, 0); }
}
