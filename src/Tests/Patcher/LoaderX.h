#pragma once

#include "Utilities.h"
#include "GamePatches.h"

#include <minhook.h>

namespace Loader
{
	// hooking functions
	DWORD WINAPI InjectHooks(LPVOID lpParam);
	void OpenConsole();
	void Cleanup();

	// addresses to hook

	// typedef hooks

	// original functions

	// function detours
}
