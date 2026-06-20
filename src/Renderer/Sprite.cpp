#include "Renderer/Sprite.h"
#include "Renderer/Renderer.h"
#include "Renderer/SpriteSheets.h"
#include "Nu3D/Sprite.h"
#include "SoftwareRenderer.h"
#include "NGNLoader/NGNLoader.h"

#include <STDIO.H>

namespace Renderer
{
	namespace Sprite
	{
		// GLOBAL: TOY2 0x005087E8
		float g_parallaxDepthZPos = 0.99989998;

		// GLOBAL: TOY2 0x00884AC0
		Nu3D::Sprite* g_queued2DSprite;

		// STUB: TOY2 0x004B8CC0
		void Queue2DSprite(float xPosition,
			float yPosition,
			float width,
			float height,
			Vector2F* uvTopLeft,
			Vector2F* uvBottomRight,
			int32_t textureIndex,
			RGBA color,
			int32_t flags)
		{}

		// FUNCTION: TOY2 0x00493F40
		int16_t DrawScaled(int16_t xPos,
			int16_t yPos,
			int32_t sheetIndex,
			int32_t tileIndex,
			uint32_t red,
			uint32_t green,
			uint32_t blue,
			int16_t flags,
			int32_t scaleX,
			int32_t scaleY)
		{
			SpriteSheet* sheet;

			if ((sheetIndex & 0x8000) == 0)
				sheet = g_spriteSheets[sheetIndex];
			else
				sheet = g_fallbackSpriteSheet;

			if (! sheet)
				return 1;

			int32_t textureDataIndex = NGNLoader::GetTextureDataIndex(sheet->texIndex);

			Vector2F uvMin;
			Vector2F uvMax;

			if (textureDataIndex)
			{
				uint32_t bitmapWidth;
				uint32_t bitmapHeight;
				NGNLoader::RetrieveTextureData(textureDataIndex, &bitmapWidth, &bitmapHeight, 0, 0, 0);

				uvMin.x = sheet->tiles[tileIndex].x / (double)bitmapWidth;
				uvMin.y = sheet->tiles[tileIndex].y / (double)bitmapHeight;

				int32_t maxX = sheet->tileWidth + sheet->tiles[tileIndex].x;
				uvMax.x = maxX / (double)bitmapWidth;
				int32_t maxY = sheet->tileHeight + sheet->tiles[tileIndex].y;
				uvMax.y = maxY / (double)bitmapHeight;
			}

			RGBA color;
			color.b = blue;
			color.g = green;
			color.r = red;
			color.a = 0;

			int32_t blendMode = flags & 96;
			int32_t renderFlags;

			if (blendMode == 0)
			{
				color.a = 128;
				renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_DEFAULT;
			}
			else if (blendMode == 32)
			{
				color.a = 255;
				renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_CUSTOM;
			}
			else if (blendMode == 64)
			{
				color.a = 255;
				renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_ALT;
			}
			else
			{
				color.a = 255 - ((flags >> 8) & 0xFF);
				renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_DEFAULT;
			}

			float invHeight = 1.0 / g_virtualScreenHeight;
			float invWidth = 1.0 / g_virtualScreenWidth;

			float h = ((scaleY * sheet->tileHeight) >> 12) * invHeight;
			float w = ((scaleX * sheet->tileWidth) >> 12) * invWidth;
			float y = yPos * invHeight;
			float x = xPos * invWidth;

			Queue2DSprite(x, y, w, h, &uvMin, &uvMax, textureDataIndex, color, renderFlags);
			return 1;
		}

		// STUB: TOY2 0x004B6300
		void ResetQueue() {}

		// STUB: TOY2 0x004B6AD0
		void DispatchCommand(Nu3D::Sprite* command) { DECOMP_PRINT(("Dispatching draw for type %d\n", command->type)); }

		// FUNCTION: TOY2 0x004B8460
		void DrawQueuedSprite()
		{
			DispatchCommand(g_queued2DSprite);
			ResetQueue();

			if (g_isSoftwareRendering)
			{
				SoftwareRenderer::UnkFunc31();
				SoftwareRenderer::UnkFunc32();
			}
		}
	}
}