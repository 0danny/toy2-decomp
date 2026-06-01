#pragma once

#include <windows.h>
#include <fstream>

inline void OpenConsole(LPCSTR consoleTitle)
{
    AllocConsole();

    FILE* stream = nullptr;

    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    freopen_s(&stream, "CONIN$", "r", stdin);

    printf("[LOADER]: Dll loaded.\n");

    SetConsoleTitleA(consoleTitle);
}
