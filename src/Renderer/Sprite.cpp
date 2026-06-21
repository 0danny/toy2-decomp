#include "Renderer/Sprite.h"
#include "Renderer/Renderer.h"
#include "Renderer/SpriteSheets.h"
#include "Nu3D/Sprite.h"
#include "SoftwareRenderer.h"
#include "NGNLoader/NGNLoader.h"
#include "DrawingDevice.h"
#include "Logger.h"

#include <STDIO.H>

namespace Renderer
{
	namespace Sprite
	{
		// GLOBAL: TOY2 0x005087E8
		float g_parallaxDepthZPos = 0.99989998;

		// GLOBAL: TOY2 0x00884AC0
		Nu3D::Sprite* g_queued2DSprite;

		// GLOBAL: TOY2 0x005086FC
		int32_t g_spriteBuffer2DCount = 2000;

		// GLOBAL: TOY2 0x00971F9C
		Nu3D::Sprite g_spriteBuffer2D[2000];

		// GLOBAL: TOY2 0x005087EC
		WORD g_2DSpriteIndices[4] = { 0, 1, 2, 3 };

		// FUNCTION: TOY2 0x004B68B0
		HRESULT Render2DSprite(Nu3D::Sprite* sprite)
		{
			int32_t xPos = DrawingDevice::GetDestWidth() * sprite->position.x;
			int32_t xOffset = DrawingDevice::GetDestWidth() * (sprite->width + sprite->position.x);
			int32_t yPos = DrawingDevice::GetDestHeight() * sprite->position.y;
			int32_t destHeight = DrawingDevice::GetDestHeight();

			int32_t renderFlags = sprite->renderFlags;
			int32_t yOffset = destHeight * (sprite->height + sprite->position.y);

			int32_t flags;
			double zPos;

			if (renderFlags == RENDER_PARALLAX_BG)
			{
				zPos = Sprite::g_parallaxDepthZPos;
				flags = RENDER_TEXTURE_WRAP_UV | RENDER_Z | RENDER_ZWRITE | RENDER_BILINEAR_FILTER | RENDER_CULL_NONE;
			}
			else
			{
				zPos = 0.0;
				flags = renderFlags | RENDER_Z | RENDER_ZWRITE | RENDER_CULL_NONE;
			}

			Nu3D::VertexTL vertexData[4];
			vertexData[1].position.y = yPos;
			vertexData[1].uv.y = sprite->uvTopLeft.y;
			vertexData[3].position.x = xOffset;
			vertexData[3].position.y = yPos;
			vertexData[1].position.z = zPos;
			vertexData[3].position.z = zPos;
			vertexData[3].uv.x = sprite->uvTopRight.x;
			vertexData[3].uv.y = sprite->uvTopRight.y;
			vertexData[0].position.x = xPos;
			vertexData[0].uv.x = sprite->uvBottomLeft.x;
			vertexData[0].position.y = yOffset;
			vertexData[0].uv.y = sprite->uvBottomLeft.y;
			vertexData[0].position.z = zPos;
			vertexData[1].position.x = xPos;
			vertexData[2].position.x = xOffset;
			vertexData[2].position.y = yOffset;
			vertexData[1].uv.x = sprite->uvTopLeft.x;

			RGBA color = sprite->color;

			color.a = 255;
			color.b = 255;
			color.r = 255;
			color.g = 255;

			vertexData[2].position.z = zPos;
			vertexData[2].uv.x = sprite->uvBottomRight.x;
			vertexData[1].diffuse = color;
			vertexData[1].rhw = 1.0;
			vertexData[3].diffuse = color;
			vertexData[3].rhw = 1.0;
			vertexData[0].diffuse = color;
			vertexData[0].rhw = 1.0;
			vertexData[2].uv.y = sprite->uvBottomRight.y;
			vertexData[2].diffuse = color;
			vertexData[2].rhw = 1.0;

			Renderer::InitRenderState(flags);
			SoftwareRenderer::g_unkE4D950 = 5;

			Renderer::BindTexture(sprite->textureIndex);

			Nu3D::BmpDataNode* dbgNode = sprite->textureIndex ? NGNLoader::g_textureDataFreeList[sprite->textureIndex].bmpDataNode : 0;

			SoftwareRenderer::g_viewportRect = &sprite->viewportRect;
			SoftwareRenderer::g_unk9F6008 = 1;

			return DrawingAPI::DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0x1C4, vertexData, 4, g_2DSpriteIndices, 4, 24);
		}

		// FUNCTION: TOY2 0x004B8DD0
		void UpdateQueued2DRender(Nu3D::Sprite* sprite)
		{
			Nu3D::Sprite* queuedSprite = g_queued2DSprite;

			if ((sprite->renderFlags & (RENDER_PRESET_COLOR_OVERLAY | RENDER_PRESET_FADE_OVERLAY)) != 0 && g_queued2DSprite)
			{
				for (Nu3D::Sprite* idx = g_queued2DSprite->next; idx; idx = idx->next)
					queuedSprite = idx;

				sprite->next = 0;
				queuedSprite->next = sprite;
			}
			else
			{
				sprite->next = g_queued2DSprite;
				g_queued2DSprite = sprite;
			}

			int32_t renderFlags = sprite->renderFlags;

			if (renderFlags == RENDER_PRESET_COLOR_OVERLAY)
			{
				sprite->renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_CUSTOM;
			}
			else if (renderFlags == RENDER_PRESET_FADE_OVERLAY)
			{
				uint8_t blue = sprite->color.b;
				uint8_t green = sprite->color.g;

				if (blue == green && blue == sprite->color.r)
				{
					sprite->renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_DEFAULT;
					sprite->color.a = green;
					sprite->color.r = 0;
					sprite->color.g = 0;
					sprite->color.b = 0;
				}
				else
				{
					sprite->renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_ALT;
				}
			}
		}

		// FUNCTION: TOY2 0x004B8CC0
		void Queue2DSprite(float xPosition,
			float yPosition,
			float width,
			float height,
			Vector2F* uvTopLeft,
			Vector2F* uvBottomRight,
			int32_t textureIndex,
			RGBA color,
			int32_t flags)
		{
			if (g_spriteBuffer2DCount)
			{
				RGBA modulatedColor = ModulateColorByAlpha(color, flags);

				Nu3D::Sprite* sprite;

				if (flags == RENDER_PARALLAX_BG)
					sprite = &g_spriteBuffer2D[g_spriteBuffer2DCount];
				else
					sprite = &g_spriteBuffer2D[g_spriteBuffer2DCount--];

				sprite->position.x = xPosition;
				sprite->position.y = yPosition;
				sprite->width = width;
				sprite->type = RENDER_2D_SPRITE;
				sprite->height = height;
				sprite->uvTopLeft = *uvTopLeft;
				sprite->uvBottomLeft.x = uvTopLeft->x;
				sprite->uvBottomLeft.y = uvBottomRight->y;
				sprite->uvTopRight.x = uvBottomRight->x;
				sprite->uvTopRight.y = uvTopLeft->y;
				sprite->uvBottomRight = *uvBottomRight;
				sprite->textureIndex = textureIndex;

				if (flags != RENDER_PARALLAX_BG)
					modulatedColor = ApplyGammaCorrection(modulatedColor);

				sprite->color = modulatedColor;
				sprite->renderFlags = flags;

				Nu3D::Viewport::GetViewClipRect(&sprite->viewportRect);

				if (flags == RENDER_PARALLAX_BG)
					Render2DSprite(sprite);
				else
					UpdateQueued2DRender(sprite);
			}
			else
			{
				Logger::DebugLog("sprite buffer underrun");
			}
		}

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
				sheet = g_spriteSheets[(int16_t)sheetIndex];
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

				int16_t ti = (int16_t)tileIndex;

				double dW = (double)(uint32_t)bitmapWidth;
				double dH = (double)(uint32_t)bitmapHeight;

				uvMin.x = sheet->tiles[ti].x / dW;
				uvMin.y = sheet->tiles[ti].y / dH;

				uvMax.x = ((double)sheet->tileWidth + (double)sheet->tiles[ti].x) / dW;
				uvMax.y = ((double)sheet->tileHeight + (double)sheet->tiles[ti].y) / dH;
			}

			RGBA packedColor;
			packedColor.g = (uint8_t)green;
			packedColor.r = (uint8_t)red;
			packedColor.b = (uint8_t)blue;

			uint8_t* alphaPtr = &packedColor.a;

			if (! alphaPtr)
				alphaPtr = (uint8_t*)&red;

			int32_t blendMode = flags & 96;
			int32_t renderFlags;

			if (blendMode != 0)
			{
				if (blendMode == 32)
				{
					*alphaPtr = 255;
					renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_CUSTOM;
				}
				else if (blendMode == 64)
				{
					*alphaPtr = 255;
					renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_ALT;
				}
				else
				{
					*alphaPtr = 255 - (uint8_t)(flags >> 8);
					renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_DEFAULT;
				}
			}
			else
			{
				*alphaPtr = 128;
				renderFlags = RENDER_ZWRITE | RENDER_CULL_NONE | RENDER_ALPHA_DEFAULT;
			}

			float invHeight = 1.0 / g_virtualScreenHeight;
			float invWidth = 1.0 / g_virtualScreenWidth;

			Queue2DSprite(xPos * invWidth,
				yPos * invHeight,
				((scaleX * sheet->tileWidth) >> 12) * invWidth,
				((scaleY * sheet->tileHeight) >> 12) * invHeight,
				&uvMin,
				&uvMax,
				textureDataIndex,
				packedColor,
				renderFlags);

			return 1;
		}

		// FUNCTION: TOY2 0x004B6300
		void ResetQueue()
		{
			g_queued2DSprite = 0;
			g_spriteBuffer2DCount = 2000;
		}

		// STUB: TOY2 0x004B6AD0
		void DispatchCommand(Nu3D::Sprite* command)
		{
			Nu3D::Sprite* commandPointer = command;

			if (commandPointer)
			{
				do
				{
					switch (commandPointer->type)
					{
						case RENDER_2D_SPRITE:
							Sprite::Render2DSprite(commandPointer);
						default:
							break;
					}

					commandPointer = commandPointer->next;

				} while (commandPointer);
			}
		}

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