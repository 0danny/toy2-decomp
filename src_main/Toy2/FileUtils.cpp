#include "FileUtils.h"
#include "D3DApp.h"
#include "Logger.h"

#include <cstdio>
#include <windows.h>

namespace FileUtils
{
	char    g_pathRegValue[512];
	char    g_cdPathRegValue[512];
	int32_t g_registryKeysRead;

	// $FUNC 004A6390 [IMPLEMENTED]
	void ValidateInstall()
	{
		HKEY    l_keyHandle;
		HKEY    l_phkResult;
		int32_t l_allow32B;
		char    l_fileNameBuffer[1024];

		g_cdPathRegValue[0] = 0;
		g_pathRegValue[0]   = 0;

		if ( ! RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software", 0, KEY_ALL_ACCESS, &l_phkResult) )
		{
			if ( ! RegOpenKeyExA(l_phkResult, "TravellersTalesToyStory2", 0, KEY_ALL_ACCESS, &l_keyHandle) )
			{
				DWORD l_dataSize = 512;

				if ( ! RegQueryValueExA(l_keyHandle, "path", 0, 0, (LPBYTE)g_pathRegValue, &l_dataSize) )
					g_pathRegValue[l_dataSize] = 0;

				l_dataSize = 512;

				if ( RegQueryValueExA(l_keyHandle, "cdpath", 0, 0, (LPBYTE)g_cdPathRegValue, &l_dataSize) )
					strcpy(g_cdPathRegValue, g_pathRegValue);
				else
					g_cdPathRegValue[l_dataSize] = 0;

				l_dataSize = 4;

				if ( ! RegQueryValueExA(l_keyHandle, "allow32bit", 0, 0, (LPBYTE)&l_allow32B, &l_dataSize) &&
				     l_allow32B )
					D3DApp::g_allow32BitColors = 0;

				RegCloseKey(l_keyHandle);
			}

			RegCloseKey(l_phkResult);
		}

		g_registryKeysRead = 1;

		if ( ! g_pathRegValue[0] || ! g_cdPathRegValue[0] )
			Logger::GetErrorHandler("C:\\projects\\toy2\\Win95.cpp", 217)(
			    "Toy Story 2 is not correctly installed,\r\nplease re-install."
			);

		strcpy(l_fileNameBuffer, g_cdPathRegValue);
		strcat(l_fileNameBuffer, "validate.tta");

		FILE* l_validateFile = fopen(l_fileNameBuffer, "rb");

		if ( l_validateFile )
		{
			fclose(l_validateFile);
		}
		else
		{
			Logger::GetErrorHandler("C:\\projects\\toy2\\Win95.cpp", 227)(
			    "Unable to find file \r\n\"%s\".\r\nPlease ensure your Toy Story 2 "
			    "CD\r\nis in the specified drive.",
			    l_fileNameBuffer
			);
		}
	}
}
