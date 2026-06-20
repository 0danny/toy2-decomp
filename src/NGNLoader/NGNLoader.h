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
	int32_t GetTextureDataIndex(uint32_t textureIndex);
	void RetrieveTextureData(
		int32_t texDataIndex, uint32_t* bitmapWidthOut, uint32_t* bitmapHeightOut, uint32_t* textureWidth, uint32_t* textureHeight, uint32_t** textureData);
}
