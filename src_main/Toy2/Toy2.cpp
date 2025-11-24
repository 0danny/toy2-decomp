#include "Toy2.h"
#include "D3DApp.h"
#include "Logger.h"
#include "FileUtils.h"

#include <windows.h>
#include <fstream>

namespace Toy2
{
	ToyCfg g_toyCfgData;

	// $FUNC 0048E730 [UNFINISHED]
	void OneInit() {}

	// $FUNC 00490730 [UNFINISHED]
	void CheckForQuit() {}

	// $FUNC 004CE810 [IMPLEMENTED]
	int32_t ReadCfg()
	{
		InitCfg();

		FILE* l_fileHandle = fopen("toy2.cfg", "rb");

		if ( ! l_fileHandle )
			return 0;

		fread(&g_toyCfgData, 1, sizeof(ToyCfg), l_fileHandle);
		fclose(l_fileHandle);

		return 1;
	}

	// $FUNC 004CE760 [IMPLEMENTED]
	void InitCfg()
	{
		memset(&g_toyCfgData, 0, sizeof(g_toyCfgData));

		uint32_t l_flags = g_toyCfgData.flags;

		g_toyCfgData.driverIndex = -1;

		l_flags = g_toyCfgData.flags | 7;

		g_toyCfgData.detail = 1;
		g_toyCfgData.flags = l_flags;
		g_toyCfgData.gammaCorrection = 2.0;
	}

	// $FUNC 00412D70 [UNFINISHED]
	int32_t ShowModeSelect() { return 0; }

	// $FUNC 0049D910 [UNFINISHED]
	int32_t Run(int32_t p_argCount, char** p_argList) { return 0; }
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
int32_t WINAPI WinMain(HINSTANCE p_hInstance, HINSTANCE p_hPrev, char* p_cmdLine, int32_t p_cmdShow)
{
	AllocateConsole();

	// g_leftOverVar0 = 0;

	memset(&D3DApp::g_d3dAppI, 0, sizeof(D3DApp::g_d3dAppI));

	D3DApp::g_allow32BitColors = 1;

	FileUtils::ValidateInstall();

	// dword_88278C = 1;

	// memset(&g_wndEventMsg, 0, 200u);

	D3DApp::g_hInstance = p_hInstance;
	D3DApp::g_hPrev = p_hPrev;
	D3DApp::g_lpCmdLine = p_cmdLine;
	D3DApp::g_nShowCmd = p_cmdShow;

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

	/*
    if ( g_renderMode == 1 )
    {
        Toy2::InitSoftwareRenderer();
    }
    else if ( g_renderMode == 2 )
    {
        Toy2::InitDirect3DRenderer();
    }*/

	int32_t l_tokenCount = 0;
	char* l_tokenEntries[8];
	char* l_currentToken = strtok(p_cmdLine, " ");

	if ( l_currentToken )
	{
		l_tokenCount = 1;
		char* l_nextToken = strtok(0, " ");

		if ( l_nextToken )
		{
			char** l_tokenArrayPtr = l_tokenEntries;

			do
			{
				*l_tokenArrayPtr = l_nextToken;
				++l_tokenCount;
				++l_tokenArrayPtr;
				l_nextToken = strtok(0, " ");
			}
			while ( l_nextToken );
		}
	}

	Toy2::Run(l_tokenCount, &l_currentToken);

	// g_appIsExiting = 1;

	Toy2::CheckForQuit();

	return 0;
}
