#pragma once

#include "Common.h"
#include "Numerics.h"
#include <directx7/d3d.h>
#include <windows.h>
#include <stdio.h>

namespace Nu3D
{
	struct BmpDataNode
	{
		LPDIRECTDRAWSURFACE4 surface;
		LPDIRECT3DTEXTURE2 d3dTexture;
		DDSURFACEDESC2 surfaceDesc;
		uint32_t* texData;
		uint32_t textureWidth;
		uint32_t textureHeight;
		uint32_t bitmapWidth;
		uint32_t bitmapHeight;
		HBITMAP bitmapHandle;
		int32_t unkVar1;
		int32_t unkVar2;
		int32_t unkVar3;
		int32_t unkVar4;
		char texName[80];
		int32_t unkVar5;
		int32_t flags;
		int32_t refCount;
		BmpDataNode* next;
		BmpDataNode* prev;
	};

	struct FindPixelFormat
	{
		uint32_t bpp;
		uint32_t minAlphaBits;
		uint32_t needAlpha;
		uint32_t valid;
		DDPIXELFORMAT* out;
	};

	BmpDataNode* LoadTextureByStream(FILE* handle, const char* rawTexStr, int32_t flags);

	STATIC_ASSERT(sizeof(BmpDataNode) == 0x110);
}