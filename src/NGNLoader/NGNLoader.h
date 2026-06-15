#pragma once

#include "NGNLoader/NGNTypes.h"
#include <windows.h>
#include <cstdio>

namespace NGNLoader
{
	extern NGNImage* g_ngnImage;

	void SetNewImage(char* fileName);
	void Init();
}
