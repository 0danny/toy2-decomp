#include <iostream>
#include <windows.h>
#include <minhook.h>
#include <cmath>

#include "Common/DLLCommon.hpp"
#include "Common/GameStructs.hpp"

namespace
{
	// typedef hooks
	typedef int16_t(__cdecl* DrawSlotsHook)();

	// original functions
	DrawSlotsHook o_DrawSlotsHook;

	// addresses of functions
	LPVOID DrawSlotsAddress = (LPVOID)0x004907E0;

	auto g_drawingDevice = reinterpret_cast<CD3DFramework**>(0x00884008);
	auto g_levelActors = reinterpret_cast<Toy2Actor*>(0x0052C840);
	auto g_buzzActor = reinterpret_cast<Toy2BuzzActor*>(0x0052F300);
	auto g_activeCameraTransform = reinterpret_cast<ActiveCameraTransform*>(0x0052ADC0);
}

static void NopDrawSlots()
{
	// Draw slots is called inside of RenderMenu and RenderGame
	// We don't want the radar to show in the main menu
	BYTE* target = reinterpret_cast<BYTE*>(0x004419EF);
	const SIZE_T len = 5;

	DWORD oldProtect;
	if (VirtualProtect(target, len, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		memset(target, 0x90, len); // 5x NOP
		VirtualProtect(target, len, oldProtect, &oldProtect);

		// Flush the instruction cache so the CPU sees the new bytes
		FlushInstructionCache(GetCurrentProcess(), target, len);

		printf("[PATCH]: NOP'd DrawSlots call at 0x004419EF\n");
	}
	else
	{
		printf("[PATCH]: VirtualProtect failed: %lu\n", GetLastError());
	}
}

static int16_t __cdecl DrawSlotsFunc()
{
	int16_t result = o_DrawSlotsHook();

	// Called directly before EndScene, after the 3D render pass is done.
	auto device = g_drawingDevice[0];
	auto d3d = device->pd3dDevice;
    
    return result;
}

static void InitMinhook()
{
	if (MH_Initialize() != MH_OK)
	{
		printf("[LOADER]: Failed to initialize MinHook.\n");
		return;
	}

	printf("ToyRadar started!\n");

	NopDrawSlots();

	if (MH_CreateHook(DrawSlotsAddress, (LPVOID)&DrawSlotsFunc, reinterpret_cast<LPVOID*>(&o_DrawSlotsHook)) != MH_OK)
		printf("[HOOK]: Could not hook DrawSlots function.\n");

	MH_EnableHook(MH_ALL_HOOKS);
}

static DWORD WINAPI InjectHooks(LPVOID lpParam)
{
	OpenConsole("ToyRadar");
	InitMinhook();

	while (true)
		Sleep(16);
}

static void Cleanup()
{
	printf("[LOADER]: Cleaning up...\n");

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		static HANDLE g_thread = 0;
		static DWORD g_threadId = 0;

		g_thread = CreateThread(NULL, 0, InjectHooks, NULL, 0, &g_threadId);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		Cleanup();
	}

	return TRUE;
}