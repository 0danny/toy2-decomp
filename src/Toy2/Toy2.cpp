#include "Toy2.h"
#include "D3DApp.h"
#include "Logger.h"

#include <windows.h>
#include <fstream>

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

	// $GLOBAL 00882F40
	char g_pathRegValue[512];

	// $GLOBAL 00883144
	char g_cdPathRegValue[512];

	// $GLOBAL 00882F3C
	int32_t g_registryKeysRead;

	// $GLOBAL 0088278C
	int32_t g_levelFileIndex;

	// $GLOBAL 0052AD9C
	int32_t g_returnedToTitle;

	// $GLOBAL 0052ADA0
	int32_t g_attractModeTimer;

	// $GLOBAL 00A4C454
	int32_t g_isElevatorHopLevel;
}

namespace Toy2
{
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

	// $FUNC 004A6390 [IMPLEMENTED]
	void ValidateInstall()
	{
		HKEY keyHandle;
		HKEY phkResult;
		int32_t allow32B;
		char fileNameBuffer[1024];

		g_cdPathRegValue[0] = 0;
		g_pathRegValue[0] = 0;

		if (! RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software", 0, KEY_ALL_ACCESS, &phkResult))
		{
			if (! RegOpenKeyExA(phkResult, "TravellersTalesToyStory2", 0, KEY_ALL_ACCESS, &keyHandle))
			{
				DWORD dataSize = 512;

				if (! RegQueryValueExA(keyHandle, "path", 0, 0, (LPBYTE)g_pathRegValue, &dataSize))
					g_pathRegValue[dataSize] = 0;

				dataSize = 512;

				if (RegQueryValueExA(keyHandle, "cdpath", 0, 0, (LPBYTE)g_cdPathRegValue, &dataSize))
					strcpy(g_cdPathRegValue, g_pathRegValue);
				else
					g_cdPathRegValue[dataSize] = 0;

				dataSize = 4;

				if (! RegQueryValueExA(keyHandle, "allow32bit", 0, 0, (LPBYTE)&allow32B, &dataSize) && allow32B)
					D3DApp::g_allow32BitColors = 0;

				RegCloseKey(keyHandle);
			}

			RegCloseKey(phkResult);
		}

		g_registryKeysRead = 1;

		if (! g_pathRegValue[0] || ! g_cdPathRegValue[0])
			Logger::GetErrorHandler("C:\\projects\\toy2\\Win95.cpp", 217)("Toy Story 2 is not correctly installed,\r\nplease re-install.");

		strcpy(fileNameBuffer, g_cdPathRegValue);
		strcat(fileNameBuffer, "validate.tta");

		FILE* validateFile = fopen(fileNameBuffer, "rb");

		if (validateFile)
		{
			fclose(validateFile);
		}
		else
		{
			Logger::GetErrorHandler("C:\\projects\\toy2\\Win95.cpp", 227)(
				"Unable to find file \r\n\"%s\".\r\nPlease ensure your Toy Story 2 "
				"CD\r\nis in the specified drive.",
				fileNameBuffer);
		}
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

	Toy2::ValidateInstall();
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
