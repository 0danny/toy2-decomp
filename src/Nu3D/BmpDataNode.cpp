#include "BmpDataNode.h"
#include "DrawingDevice.h"

namespace Nu3D
{
	// $GLOBAL 00884044
	uint8_t g_lastBmpPalette[1024];

	// $GLOBAL 00884448
	int32_t g_minTextureSize;

	// $GLOBAL 00508214
	int32_t g_maxTextureSize = 256;

	// $GLOBAL 00508210
	int32_t g_unusedTexShift = 1;

	// $GLOBAL 00884444
	BmpDataNode* g_bmpDataHead;

	// $FUNC 004AFF80 [UNFINISHED]
	void CopyTextureToSurface(BmpDataNode* bitmapDataNode) {}

	// $FUNC 004B0400 [IMPLEMENTED]
	uint32_t CountAlphaBits(LPDDPIXELFORMAT pixelFormat)
	{
		DWORD alphaBitMask = pixelFormat->dwRGBAlphaBitMask;

		uint32_t result;

		for (result = 0; alphaBitMask; alphaBitMask >>= 1)
		{
			if ((alphaBitMask & 1) != 0)
				++result;
		}

		return result;
	}

	// $FUNC 004B0380 [IMPLEMENTED]
	LONG WINAPI FindSuitablePixelFormat(LPDDPIXELFORMAT pixelFormat, void* context)
	{
		if (! pixelFormat)
			return 1;

		if (! context)
			return 1;

		DWORD flags = pixelFormat->dwFlags;

		if ((flags & 0xE0000) != 0)
			return 1;

		DWORD bitCount = pixelFormat->dwRGBBitCount;

		if (bitCount < 0x10)
			return 1;

		if (pixelFormat->dwFourCC)
			return 1;

		FindPixelFormat* findPixelFormat = (FindPixelFormat*)context;

		uint32_t needAlpha = findPixelFormat->needAlpha;

		if (needAlpha == 1 && (flags & 1) == 0)
			return 1;

		if (! needAlpha && (flags & 1) != 0 || bitCount != findPixelFormat->bpp || needAlpha && CountAlphaBits(pixelFormat) < findPixelFormat->minAlphaBits)
		{
			return 1;
		}

		memcpy(findPixelFormat->out, pixelFormat, sizeof(DDPIXELFORMAT));

		findPixelFormat->valid = 1;

		return 0;
	}

	// $FUNC 004AFBA0 [IMPLEMENTED]
	void DestroyBmpDataNode(BmpDataNode* bmpDataNode)
	{
		LPDIRECT3DTEXTURE2 d3dTexture = bmpDataNode->d3dTexture;

		if (d3dTexture)
		{
			d3dTexture->Release();
			bmpDataNode->d3dTexture = 0;
		}

		if (bmpDataNode->surface)
		{
			if ((bmpDataNode->flags & 64) == 0)
			{
				bmpDataNode->surface->Release();
				bmpDataNode->surface = 0;
			}
		}
	}

	// $FUNC 004B0200 [IMPLEMENTED]
	int32_t InitialiseTextureSurface(BmpDataNode* bmpDataNode)
	{
		FindPixelFormat findPixelFormat;
		DDSURFACEDESC2 surfaceDesc;
		D3DDEVICEDESC deviceDesc2;
		D3DDEVICEDESC deviceDesc;

		if ((bmpDataNode->flags & 0x40) != 0)
			return 0;

		DestroyBmpDataNode(bmpDataNode);

		LPDIRECTDRAW4 ddraw4 = DrawingDevice::GetDDraw4();

		// yes, this just gets called
		DrawingDevice::GetD3D();

		LPDIRECT3DDEVICE3 d3dDevice = DrawingDevice::GetD3DDevice();

		deviceDesc.dwSize = 252;
		deviceDesc2.dwSize = 252;

		if (d3dDevice->GetCaps(&deviceDesc, &deviceDesc2))
			return 0;

		memset(&surfaceDesc, 0, sizeof(surfaceDesc));
		uint32_t width = bmpDataNode->textureWidth;
		uint32_t height = bmpDataNode->textureHeight;

		surfaceDesc.ddsCaps.dwCaps2 = 16;

		findPixelFormat.bpp = 16;

		surfaceDesc.dwSize = 124;
		findPixelFormat.out = &surfaceDesc.ddpfPixelFormat;

		int32_t flags = bmpDataNode->flags;

		surfaceDesc.ddpfPixelFormat.dwSize = 32;
		surfaceDesc.dwFlags = 0x101007;
		surfaceDesc.ddsCaps.dwCaps = 4096;
		surfaceDesc.dwTextureStage = 0;
		surfaceDesc.dwWidth = width;
		surfaceDesc.dwHeight = height;
		memset(&findPixelFormat.minAlphaBits, 0, 12);

		if ((flags & 0xF) != 0)
		{
			findPixelFormat.needAlpha = 1;
			findPixelFormat.minAlphaBits = (flags & 1) != 0 ? 4 : 1;
		}

		d3dDevice->EnumTextureFormats(FindSuitablePixelFormat, &findPixelFormat);

		if (! findPixelFormat.valid || ddraw4->CreateSurface(&surfaceDesc, &bmpDataNode->surface, 0)
			|| bmpDataNode->surface->QueryInterface(IID_IDirect3DTexture2, (LPVOID*)&bmpDataNode->d3dTexture))
		{
			return 0;
		}

		uint32_t* texData = bmpDataNode->texData;

		memcpy(&bmpDataNode->surfaceDesc, &surfaceDesc, sizeof(bmpDataNode->surfaceDesc));

		if (texData)
			CopyTextureToSurface(bmpDataNode);

		return 1;
	}

	// $FUNC 004B0760 [IMPLEMENTED]
	int32_t GetBitmapWidth(HANDLE bmpHandle)
	{
		BITMAP bmp;
		GetObjectA(bmpHandle, sizeof(BITMAP), &bmp);
		return bmp.bmWidth;
	}

	// $FUNC 004B0780 [IMPLEMENTED]
	int32_t GetBitmapHeight(HANDLE bmpHandle)
	{
		BITMAP bmp;
		GetObjectA(bmpHandle, sizeof(BITMAP), &bmp);
		return bmp.bmHeight;
	}

	// $FUNC 004AFC10 [IMPLEMENTED]
	BmpDataNode* AllocateBmpDataNode()
	{
		BmpDataNode* bmpDataStruct = (BmpDataNode*)malloc(sizeof(BmpDataNode));

		if (bmpDataStruct)
		{
			memset(bmpDataStruct, 0, sizeof(BmpDataNode));
			bmpDataStruct->next = g_bmpDataHead;

			if (g_bmpDataHead)
				g_bmpDataHead->prev = bmpDataStruct;

			bmpDataStruct->prev = 0;

			g_bmpDataHead = bmpDataStruct;

			bmpDataStruct->refCount = 0;
		}

		return bmpDataStruct;
	}

	// $FUNC 004B0870 [IMPLEMENTED]
	RGBA SampleBitmapPixel(BITMAP* mainBmp, BITMAP* alphaBmp, int32_t x, int32_t y, int32_t textureSize, uint8_t flags)
	{
		RGBA color;
		int32_t bitsPerPixel = mainBmp->bmBitsPixel;

		if (bitsPerPixel == 8)
		{
			int32_t scaledY2 = y * mainBmp->bmHeight / textureSize;

			color.a = -1;

			int32_t paletteIndex = 4 * *((uint8_t*)mainBmp->bmBits + x * mainBmp->bmWidth / textureSize + mainBmp->bmWidthBytes * scaledY2);

			color.r = g_lastBmpPalette[paletteIndex];
			color.b = g_lastBmpPalette[paletteIndex + 2];
			color.g = g_lastBmpPalette[paletteIndex + 1];

			if ((flags & 2) != 0)
			{
				color.a = -(color.value != -1);
				return color;
			}

			if ((flags & 4) != 0)
			{
				color.a = -(color.value != 0xFF000000);
				return color;
			}

			if ((flags & 8) != 0 && color.value == 0xFF00FF00)
			{
				RGBA empty = {0};
				return empty;
			}
		}
		else if (bitsPerPixel == 24)
		{
			int32_t scaledY = y * mainBmp->bmHeight / textureSize;

			color.a = 0xFF;

			uint8_t* pixelOffset = (uint8_t*)mainBmp->bmBits + 3 * (x * mainBmp->bmWidth / textureSize) + mainBmp->bmWidthBytes * scaledY;

			color.g = pixelOffset[1];
			color.r = *pixelOffset;
			color.b = pixelOffset[2];

			if ((flags & 2) != 0)
			{
				color.a = -(color.value != -1);
			}
			else if ((flags & 4) != 0)
			{
				color.a = -(color.value != 0xFF000000);
			}
			else if ((flags & 8) != 0 && color.value == 0xFF00FF00)
			{
				color.value = 0;
			}

			if (alphaBmp && alphaBmp->bmBitsPixel == 8)
			{
				color.a = *((uint8_t*)alphaBmp->bmBits + x * alphaBmp->bmWidth / textureSize + alphaBmp->bmWidthBytes * (y * alphaBmp->bmHeight / textureSize));
				return color;
			}
		}

		return color;
	}

	// $FUNC 004B0B60 [IMPLEMENTED]
	HBITMAP ProcessBmpInfoFromStream(FILE* stream)
	{
		int32_t offset = ftell(stream);

		BITMAPFILEHEADER bmpHeader;
		BITMAPINFO bmpInfo;

		fread(&bmpHeader, 1, sizeof(BITMAPFILEHEADER), stream);
		fread(&bmpInfo, 1, sizeof(BITMAPINFO), stream);

		size_t pixelDataSize;
		uint32_t calcHeight;

		switch (bmpInfo.bmiHeader.biBitCount)
		{
			case 1:
				fread(bmpInfo.bmiColors, 2, 4, stream);
				calcHeight = bmpInfo.bmiHeader.biHeight;
				break;
			case 4:
				fread(bmpInfo.bmiColors, 16, 4, stream);
				pixelDataSize = bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biHeight / 2;
				goto READ_BMDATA;
			case 8:
				fread(bmpInfo.bmiColors, 256, 4, stream);

				memcpy(Nu3D::g_lastBmpPalette, bmpInfo.bmiColors, sizeof(Nu3D::g_lastBmpPalette));
				pixelDataSize = bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biHeight;

				goto READ_BMDATA;
			default:
				calcHeight = bmpInfo.bmiHeader.biHeight * bmpInfo.bmiHeader.biBitCount;
				break;
		}

		pixelDataSize = bmpInfo.bmiHeader.biWidth * calcHeight / 8;
	READ_BMDATA:

		fseek(stream, offset + bmpHeader.bfOffBits, 0);

		void* bitmapBits;
		HBITMAP hBitmap = CreateDIBSection(0, &bmpInfo, 0, &bitmapBits, 0, 0);

		if (hBitmap)
			fread(bitmapBits, pixelDataSize, 1, stream);

		return hBitmap;
	}

	// $FUNC 004B06D0 [IMPLEMENTED]
	int32_t CalculateTexSize(HANDLE bmp, int32_t flags)
	{
		BITMAP bitmap;
		GetObjectA(bmp, sizeof(BITMAP), &bitmap);

		int32_t height = bitmap.bmHeight;

		if (bitmap.bmWidth > bitmap.bmHeight)
			height = bitmap.bmWidth;

		int32_t textureSize;

		if (g_minTextureSize <= Numerics::RoundUpToPowerOf2(height))
			textureSize = Numerics::RoundUpToPowerOf2(height);
		else
			textureSize = g_minTextureSize;

		if (textureSize >= g_maxTextureSize)
			textureSize = g_maxTextureSize;

		// compiler artifact?
		if ((flags == 0 & 16) != 0)
			textureSize >>= g_unusedTexShift;

		return textureSize;
	}

	// $FUNC 004B07A0 [IMPLEMENTED]
	uint32_t* ProcessBmpPixelData(HANDLE mainBmp, HANDLE alphaBmp, int32_t flags)
	{
		int32_t texSize = CalculateTexSize(mainBmp, flags);

		BITMAP mainBmpObj;
		BITMAP alphaBmpObj;

		GetObjectA(mainBmp, sizeof(BITMAP), &mainBmpObj);

		if (alphaBmp)
			GetObjectA(alphaBmp, sizeof(BITMAP), &alphaBmpObj);

		uint32_t* buffer = (uint32_t*)malloc(4 * texSize * texSize);

		if (! buffer)
			return buffer;

		int32_t row = 0;

		if (texSize > 0)
		{
			int32_t rowOffset = 0;

			do
			{
				for (int32_t col = 0; col < texSize; ++col)
				{
					uint32_t pixelColor;

					if (alphaBmp)
						pixelColor = SampleBitmapPixel(&mainBmpObj, &alphaBmpObj, col, row, texSize, flags).value;
					else
						pixelColor = SampleBitmapPixel(&mainBmpObj, 0, col, row, texSize, flags).value;

					buffer[col + rowOffset] = pixelColor;
				}

				++row;
				rowOffset += texSize;

			} while (row < texSize);
		}

		return buffer;
	}

	// $FUNC 004B0A30 [IMPLEMENTED]
	BmpDataNode* LoadTextureByStream(FILE* handle, const char* rawTexStr, int32_t flags)
	{
		HBITMAP mainBmp = ProcessBmpInfoFromStream(handle);
		HBITMAP alphaBmp = 0;

		if (! mainBmp)
			return 0;

		if ((flags & 1) != 0)
			alphaBmp = ProcessBmpInfoFromStream(handle);

		uint32_t* texDataBuffer = ProcessBmpPixelData(mainBmp, alphaBmp, flags);

		if (! texDataBuffer)
		{
			if (alphaBmp)
				DeleteObject(alphaBmp);

			DeleteObject(mainBmp);
			return 0;
		}

		BmpDataNode* bmpDataNode = AllocateBmpDataNode();

		if (! bmpDataNode)
		{
			free(texDataBuffer);

			if (alphaBmp)
				DeleteObject(alphaBmp);

			DeleteObject(mainBmp);
			return 0;
		}

		bmpDataNode->texData = texDataBuffer;

		int32_t texSize = CalculateTexSize(mainBmp, flags);

		bmpDataNode->textureHeight = texSize;
		bmpDataNode->textureWidth = texSize;
		bmpDataNode->bitmapWidth = GetBitmapWidth(mainBmp);
		bmpDataNode->bitmapHeight = GetBitmapHeight(mainBmp);
		bmpDataNode->flags = flags;

		strcpy(bmpDataNode->texName, rawTexStr);

		if (alphaBmp)
			DeleteObject(alphaBmp);

		if ((flags & 32) != 0)
			bmpDataNode->bitmapHandle = mainBmp;
		else
			DeleteObject(mainBmp);

		InitialiseTextureSurface(bmpDataNode);

		return bmpDataNode;
	}
}