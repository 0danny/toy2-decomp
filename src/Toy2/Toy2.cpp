#include "Toy2.h"
#include "D3DApp.h"
#include "Logger.h"
#include "FileUtils.h"

#include <windows.h>
#include <fstream>

namespace Toy2
{
	ToyCfg g_toyCfgData = {
		7, 1, 2.0, -1, -1, -1,
	};

	// $FUNC 0048E730 [UNFINISHED]
	void OneInit() {}

	// $FUNC 00490730 [UNFINISHED]
	void CheckForQuit() {}

	// $FUNC 004CE810 [IMPLEMENTED]
	int32_t ReadCfg()
	{
		InitCfg();

		FILE* fileHandle = fopen("toy2.cfg", "rb");

		if ( ! fileHandle )
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

	// $FUNC 00412D70 [UNFINISHED]
	int32_t ShowModeSelect() { return 0; }

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

// $FUNC DEBUG METHOD
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

// $FUNC 004316C0 [UNFINISHED]
int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, char* cmdLine, int32_t cmdShow)
{
	AllocateConsole();

	// g_leftOverVar0 = 0;

	memset(&D3DApp::g_d3dAppI, 0, sizeof(D3DApp::g_d3dAppI));

	D3DApp::g_allow32BitColors = 1;

	FileUtils::ValidateInstall();

	// dword_88278C = 1;

	// memset(&g_wndEventMsg, 0, 200u);

	D3DApp::g_hInstance = hInstance;
	D3DApp::g_hPrev = hPrev;
	D3DApp::g_lpCmdLine = cmdLine;
	D3DApp::g_nShowCmd = cmdShow;

	/*
    g_leftOverVar1 = 0;
    g_leftOverVar2 = 1;
    g_leftOverVar3 = 1;
    g_leftOverVar4 = 1;

    g_appIsExiting = 0;*/

	Toy2::OneInit();

	/*
    dword_52AD9C = 0;
    dword_52ADA0 = -1;
    dword_52ADA4 = 2;
    dword_52ADA8 = 0;*/

	Toy2::ReadCfg();
	D3DApp::BuildProfileMachine();
	D3DApp::BuildWindow();
	Toy2::ShowModeSelect();

	if ( D3DApp::g_renderMode == SoftwareRenderer )
	{
		Toy2::InitSoftwareRenderer();
	}
	else if ( D3DApp::g_renderMode == D3DRenderer )
	{
		Toy2::InitDirect3DRenderer();
	}

	int32_t tokenCount = 0;
	char* tokenEntries[8];
	char* currentToken = strtok(cmdLine, " ");

	if ( currentToken )
	{
		tokenCount = 1;
		char* nextToken = strtok(0, " ");

		if ( nextToken )
		{
			char** tokenArrayPtr = tokenEntries;

			do
			{
				*tokenArrayPtr = nextToken;
				++tokenCount;
				++tokenArrayPtr;
				nextToken = strtok(0, " ");
			}
			while ( nextToken );
		}
	}

	Toy2::Run(tokenCount, &currentToken);

	// g_appIsExiting = 1;

	Toy2::CheckForQuit();

	return 0;
}
