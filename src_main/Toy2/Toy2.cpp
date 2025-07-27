#include "Toy2.h"
#include "D3DApp.h"
#include "Logger.h"

#include <windows.h>
#include <cstdio>

namespace Toy2
{
    char g_pathRegValue[512];
    char g_cdPathRegValue[512];
    int32_t g_registryKeysRead;

    // $FUNC 004A6390 [IMPLEMENTED]
    void ReadRegistry()
    {
        HKEY l_keyHandle;
        HKEY l_phkResult;
        int32_t l_allow32B;
        char l_fileNameBuffer[1024];

        g_cdPathRegValue[0] = 0;
        g_pathRegValue[0] = 0;

        if (!RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software", 0, KEY_ALL_ACCESS, &l_phkResult))
        {
            if (!RegOpenKeyExA(l_phkResult, "TravellersTalesToyStory2", 0, KEY_ALL_ACCESS, &l_keyHandle))
            {
                DWORD l_dataSize = 512;

                if (!RegQueryValueExA(l_keyHandle, "path", 0, 0, (LPBYTE)g_pathRegValue, &l_dataSize))
                    g_pathRegValue[l_dataSize] = 0;

                l_dataSize = 512;

                if (RegQueryValueExA(l_keyHandle, "cdpath", 0, 0, (LPBYTE)g_cdPathRegValue, &l_dataSize))
                    strcpy(g_cdPathRegValue, g_pathRegValue);
                else
                    g_cdPathRegValue[l_dataSize] = 0;

                l_dataSize = 4;

                if (!RegQueryValueExA(l_keyHandle, "allow32bit", 0, 0, (LPBYTE)&l_allow32B, &l_dataSize) && l_allow32B)
                    D3DApp::g_allow32BitColors = 0;

                RegCloseKey(l_keyHandle);
            }

            RegCloseKey(l_phkResult);
        }

        g_registryKeysRead = 1;

        if (!g_pathRegValue[0] || !g_cdPathRegValue[0])
            Logger::GetErrorHandler("C:\\projects\\toy2\\Win95.cpp", 217)("Toy Story 2 is not correctly installed,\r\nplease re-install.");

        strcpy(l_fileNameBuffer, g_cdPathRegValue);
        strcat(l_fileNameBuffer, "validate.tta");

        FILE *l_validateFile = fopen(l_fileNameBuffer, "rb");

        if (l_validateFile)
        {
            fclose(l_validateFile);
        }
        else
        {
            Logger::GetErrorHandler("C:\\projects\\toy2\\Win95.cpp", 227) (
                "Unable to find file \r\n\"%s\".\r\nPlease ensure your Toy Story 2 CD\r\nis in the specified drive.", 
                l_fileNameBuffer
            );
        }
    }

    // $FUNC 0048E730 [UNFINISHED]
    void OneInit()
    {
    }

    // $FUNC 00490730 [UNFINISHED]
    void CheckForQuit()
    {
    }

    // $FUNC 004CE810 [UNFINISHED]
    int32_t ReadCfg()
    {
        return 0;
    }

    // $FUNC 00412D70 [UNFINISHED]
    int32_t ShowModeSelect()
    {
        return 0;
    }

    // $FUNC 0049D910 [UNFINISHED]
    int32_t Run(int32_t p_argCount, char **p_argList)
    {
        return 0;
    }
}

// $FUNC 004316C0 [UNFINISHED]
int32_t WINAPI WinMain(HINSTANCE p_hInstance, HINSTANCE p_hPrev, char *p_cmdLine, int32_t p_cmdShow)
{
    // g_leftOverVar0 = 0;

    // memset(&g_softwareRenderer, 0, sizeof(g_softwareRenderer));

    D3DApp::g_allow32BitColors = 1;

    Toy2::ReadRegistry();

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
    char *l_tokenEntries[8];
    char *l_currentToken = strtok(p_cmdLine, " ");

    if (l_currentToken)
    {
        l_tokenCount = 1;
        char *l_nextToken = strtok(0, " ");

        if (l_nextToken)
        {
            char **l_tokenArrayPtr = l_tokenEntries;

            do
            {
                *l_tokenArrayPtr = l_nextToken;
                ++l_tokenCount;
                ++l_tokenArrayPtr;
                l_nextToken = strtok(0, " ");
            } while (l_nextToken);
        }
    }

    Toy2::Run(l_tokenCount, &l_currentToken);

    // g_appIsExiting = 1;

    Toy2::CheckForQuit();

    return 0;
}