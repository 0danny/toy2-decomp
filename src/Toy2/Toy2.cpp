#include "Toy2.h"
#include "D3DApp.h"
#include "Logger.h"
#include "FileUtils.h"
#include "InputManager.h"
#include "DrawingDevice.h"
#include "ModeSelect.h"

#include "Nu3D/Font.h"
#include "Renderer/Renderer.h"
#include "AudioManager/AudioManager.h"

#include <windows.h>
#include <fstream>
#include <Numerics.h>

namespace Toy2
{
	// $GLOBAL 00508D70
	ToyCfg g_toyCfgData = {
		7, /* flags */
		1, /* detail */
		2.0, /* gamma correction */
		-1, /* driver index */
		-1, /* device index */
		-1, /* display mode index */
	};

	// $GLOBAL 0088278C
	int32_t g_levelFileIndex;

	// $GLOBAL 0052AD9C
	int32_t g_returnedToTitle;

	// $GLOBAL 0052ADA0
	int32_t g_attractModeTimer;

	// $GLOBAL 00A4C454
	int32_t g_isElevatorHopLevel;

	// $GLOBAL 004FCDB4
	int32_t g_cdBaseTrack = 2;

	// $GLOBAL 005281C8
	int32_t g_modeSelectFinished;
}

namespace Toy2
{
	namespace Graphics
	{
		// $FUNC 004CDD90 [IMPLEMENTED]
		uint32_t AddDetailLevel()
		{
			uint32_t detail = g_toyCfgData.detail + 1;

			if ((g_toyCfgData.detail + 1) >= 2)
				detail = 2;

			g_toyCfgData.detail = detail;

			return detail;
		}

		// $FUNC 004CDDB0 [IMPLEMENTED]
		uint32_t RemoveDetailLevel()
		{
			uint32_t detail = (g_toyCfgData.detail - 1) <= 0 ? 0 : g_toyCfgData.detail - 1;
			g_toyCfgData.detail = detail;

			return detail;
		}
	}

	// $FUNC 0048E730 [UNFINISHED]
	void OneInit() {}

	// $FUNC 00490730 [UNFINISHED]
	void CheckForQuit() {}

	// $FUNC 004CE810 [IMPLEMENTED]
	int32_t ReadCfg()
	{
		InitCfg();

		FILE* fileHandle = fopen("toy2.cfg", "rb");

		if (! fileHandle)
			return 0;

		fread(&g_toyCfgData, 1, sizeof(ToyCfg), fileHandle);
		fclose(fileHandle);

		return 1;
	}

	// $FUNC 004CE760 [IMPLEMENTED]
	void InitCfg()
	{
		memset(&g_toyCfgData, 0, sizeof(g_toyCfgData));

		uint32_t flags = g_toyCfgData.flags;

		g_toyCfgData.driverIndex = -1;

		flags = g_toyCfgData.flags | 7;

		g_toyCfgData.detail = 1;
		g_toyCfgData.flags = flags;
		g_toyCfgData.gammaCorrection = 2.0;
	}

	// $FUNC 00412B50 [IMPLEMENTED]
	void RunModeSelect()
	{
		if (! g_modeSelectFinished)
		{
			atexit(DrawingDevice::Quit);
			ModeSelect::SetForceFullscreen_T(0);

			if (ModeSelect::EnumerateDrivers_T(ModeSelect::DeviceFilterCallback) < 0)
				Logger::GetErrorHandler("C:\\projects\\toy2\\direct6.cpp", 103)("Unable to enumerate a suitable device");

			ModeSelect::Show();

			DrawingDevice::DDAppDevice* primaryDevice;
			DrawingDevice::DDAppDevice::App* ddApp;

			if (DrawingDevice::GetChosenDevice_T(&ddApp, &primaryDevice))
				Logger::GetErrorHandler("C:\\projects\\toy2\\direct6.cpp", 111)("Unable to create D3D device\r\n try a lower resolution or screen depth");

			int32_t canDoWindowed = primaryDevice->canRenderWindowedOnPrimary;
			int32_t fullscreenExclusive = (ModeSelect::g_unusedFlag1 != 0 ? 2 : 0) | (canDoWindowed == 0) | (ModeSelect::g_unusedFlag2 != 0 ? 4 : 0);

			if (! canDoWindowed)
				Logger::g_showMsgBoxOnThrow = 1;

			if (primaryDevice->isHardwareAccelerated)
			{
				Renderer::SetIsSoftwareRendering(0);
			}
			else
			{
				Renderer::SetIsSoftwareRendering(1);
				while (Toy2::Graphics::RemoveDetailLevel()) {};
			}

			if (! primaryDevice->isHardwareAccelerated && primaryDevice->canRenderWindowedOnPrimary)
			{
				RECT adjustedRect;
				adjustedRect.top = 0;
				adjustedRect.left = 0;
				adjustedRect.right = 320;
				adjustedRect.bottom = 240;

				AdjustWindowRect(&adjustedRect, 0, 0);
				SetWindowPos(D3DApp::g_windowData.mainHwnd, 0, 0, 0, adjustedRect.right, adjustedRect.bottom, 2);
			}

			ShowWindow(D3DApp::g_windowData.mainHwnd, 3);

			if (DrawingDevice::Build(D3DApp::g_windowData.mainHwnd, &ddApp->guid, primaryDevice, primaryDevice->primaryDisplayMode, fullscreenExclusive) >= 0)
				g_modeSelectFinished = 1;
		}
	}

	// $FUNC 0047D8D0 [IMPLEMENTED]
	void UnusedInit()
	{
		// $GLOBAL 00725F20
		static int32_t g_unusedInit;

		g_unusedInit = 2;
	}

	// $FUNC 00412D70 [IMPLEMENTED]
	int32_t ShowModeSelect()
	{
		char cdFileName[8];
		char cdTrackBuffer[1024];

		strcpy(cdFileName, "cd.txt");
		memset(cdTrackBuffer, 0, sizeof(cdTrackBuffer));

		int32_t baseCDTrack;

		if (FileUtils::LoadFile(cdFileName, cdTrackBuffer))
			baseCDTrack = atoi(cdTrackBuffer);
		else
			baseCDTrack = 2;

		g_cdBaseTrack = baseCDTrack;
		Logger::Log("CONFIG : Base CD track is %d.\n", baseCDTrack);

		AudioManager::Init();
		UnusedInit();
		Numerics::InitTrigLut();

		RunModeSelect();

		InputManager::Init();
		Renderer::Init();
		DrawingDevice::InitViewport();

		RECT* destRect = DrawingDevice::GetDestRect();

		Nu3D::Font::Init();
		Nu3D::Font* font = Nu3D::Font::Build("ariel", 20, 0);

		if (font)
		{
			Nu3D::Font::SetFont(font);
			Nu3D::Font::SetTextColor(0xFFFFFFFF);
			Nu3D::Font::SetTextClipRect(destRect->left, destRect->top, destRect->right - 1, destRect->bottom - 1);
		}

		while (ShowCursor(0) >= 0) {};

		return 1;
	}

	// $FUNC 0049D910 [UNFINISHED]
	int32_t Run(int32_t argCount, char** argList) { return 0; }

	// $FUNC 0047CC90 [UNFINISHED]
	void InitSoftwareRenderer()
	{
		// Some back story on this, the game has two rendering modes. Hardware accelerated (DirectX) or full software based rendering.
		// This is normal for the time, considering it was uncommon for people to have dedicated graphics hardware as it was expensive.
		//
		// The software renderer will be decompiled last, as it is extremely math heavy and optimized, making it difficult to produce
		// code that can be read by human eyes.
		return;
	}

	// $FUNC 00499950 [UNFINISHED]
	void InitDirect3DRenderer()
	{
		// Weird method, a good portion of these variables are never even used in the game
	}
}

// $FUNC DEBUG
void AllocateConsole()
{
	AllocConsole();

	FILE* fp;

	// redirect STDOUT
	fp = freopen("CONOUT$", "w", stdout);
	fp = freopen("CONOUT$", "w", stderr);

	// redirect STDIN
	fp = freopen("CONIN$", "r", stdin);

	printf("[Debug Console Allocated!]\n");
}

// $FUNC 004316C0 [IMPLEMENTED]
int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, char* cmdLine, int32_t cmdShow)
{
	// $GLOBAL 00534550
	static int32_t g_unused0;
	// $GLOBAL 0052ADA4
	static int32_t g_unused1;
	// $GLOBAL 0052ADA8
	static int32_t g_unused2;

	AllocateConsole();

	g_unused0 = 0;

	memset(&D3DApp::g_d3dAppI, 0, sizeof(D3DApp::g_d3dAppI));

	D3DApp::g_allow32BitColors = 1;

	FileUtils::ValidateInstall();
	Toy2::g_levelFileIndex = 1;

	memset(&D3DApp::g_windowData, 0, sizeof(D3DApp::g_windowData));

	D3DApp::g_windowData.hInstance = hInstance;
	D3DApp::g_windowData.hPrev = hPrev;
	D3DApp::g_windowData.lpCmdLine = cmdLine;
	D3DApp::g_windowData.nShowCmd = cmdShow;

	D3DApp::g_windowData.unkInt8 = 0;
	D3DApp::g_windowData.unkInt3 = 1;
	D3DApp::g_windowData.unkInt4 = 1;
	D3DApp::g_windowData.unkInt5 = 1;

	D3DApp::g_windowData.wndIsExiting = 0;

	Toy2::OneInit();

	Toy2::g_returnedToTitle = 0;
	Toy2::g_attractModeTimer = -1;

	g_unused1 = 2;
	g_unused2 = 0;

	Toy2::ReadCfg();
	D3DApp::BuildProfileMachine();
	D3DApp::BuildWindow();
	Toy2::ShowModeSelect();

	if (D3DApp::g_renderMode == SoftwareRenderer)
	{
		Toy2::InitSoftwareRenderer();
	}
	else if (D3DApp::g_renderMode == D3DRenderer)
	{
		Toy2::InitDirect3DRenderer();
	}

	int32_t tokenCount = 0;
	char* tokenEntries[8];
	char* currentToken = strtok(cmdLine, " ");

	if (currentToken)
	{
		tokenCount = 1;
		char* nextToken = strtok(0, " ");

		if (nextToken)
		{
			char** tokenArrayPtr = tokenEntries;

			do
			{
				*tokenArrayPtr = nextToken;
				++tokenCount;
				++tokenArrayPtr;

				nextToken = strtok(0, " ");
			} while (nextToken);
		}
	}

	Toy2::Run(tokenCount, &currentToken);

	D3DApp::g_windowData.wndIsExiting = 1;

	Toy2::CheckForQuit();

	return 0;
}
