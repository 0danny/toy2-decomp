#include "game_patches.h"

#include <cstdarg>
#include <cstdio>

namespace GamePatches
{
    void InjectHooks()
    {
        printf("[GAME_PATCHES]: Injecting hooks...\n");

        if (MH_CreateHook(Nu3DLogAddress, (LPVOID)&Nu3DLogHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook FSZip constructor.\n");
        }
    }

    void __cdecl Nu3DLogHook(char *p_format, ...)
    {
        va_list args;
        va_start(args, p_format);
        vprintf(p_format, args);
        va_end(args);
    }
}