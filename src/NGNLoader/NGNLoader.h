#pragma once

#include "NGNLoader/NGNTypes.h"
#include <windows.h>
#include <cstdio>

namespace NGNLoader
{
	extern NGNImage* g_ngnImage;
	extern NGNTextureData g_textureDataFreeList[2000];

	void SetNewImage(char* fileName);
	void Init();
	void DetectBackdropTextures();
}
