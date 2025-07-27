#pragma once

#include "utilities.h"

#include <ddraw.h>
#include <minhook.h>

namespace GamePatches
{
	void InjectHooks();

	// globals

	// game globals

	// typedef hooks
	typedef void(__cdecl* Nu3DLog)(char* p_format, ...);

	// original functions

	// addresses of functions
	static LPVOID Nu3DLogAddress = (LPVOID)0x004A66A0;

	// function detours
	void __cdecl Nu3DLogHook(char* p_format, ...);
}
