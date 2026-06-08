#pragma once

#include <minhook.h>

#include "Utilities.h"
#include "NGNLoader/NGNLoader.h"

namespace GamePatches
{
	void InjectHooks();
	void PollInput();

	// globals

	// game globals

	// typedef hooks
	typedef void(__cdecl* TestMethod)(FILE* stream, NGNLoader::NGNImage* ngnImage);

	// original functions

	// addresses of functions
	static LPVOID TestAddress = (LPVOID)0x004C3BE0;

	// function detours
	void __cdecl TestMethodHook(FILE* stream, NGNLoader::NGNImage* ngnImage);
}
