#pragma once

#include "Common.h"
#include <windows.h>

// Looks like these methods are purely for debug methods that are never called
namespace Nu3D
{
	struct GlyphInfo
	{
		float uvMinX;
		float uvMinY;
		float uvMaxX;
		float uvMaxY;
		int16_t width;
		int16_t height;
	};

	struct Font
	{
		Nu3D::Font* next;
		Nu3D::Font* prev;
		int16_t unkInt1;
		int16_t unkInt2;
		char fontName[12];
		int16_t fontId;
		int16_t fontHeight;
		int16_t fontAscent;
		int16_t unkInt3;
		int16_t atlasWidth;
		int16_t atlasHeight;
		int16_t numGlyphs;
		int16_t unkInt4;
		HBITMAP bmpHandle;
		int32_t texIndex;
		uint8_t charToGlyphIndex[256];
		GlyphInfo glyphs[];

		static int32_t Init();
		static Font* Build(const char* fontName, int32_t fontSize, const char* charSet);
		static void SetFont(Font* font);
		static void SetTextColor(int32_t color);
		static void SetTextClipRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
	};

	STATIC_ASSERT(sizeof(Font) == 0x130);
	STATIC_ASSERT(sizeof(GlyphInfo) == 0x14);
}