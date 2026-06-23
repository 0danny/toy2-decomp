#pragma once

#include "Common.h"
#include "Numerics.h"

namespace Renderer
{
	namespace Sprite
	{
		extern float g_parallaxDepthZPos;

		int16_t DrawScaled(int16_t xPos,
			int16_t yPos,
			int32_t sheetIndex,
			int32_t tileIndex,
			uint32_t red,
			uint32_t green,
			uint32_t blue,
			uint32_t flags,
			int32_t scaleX,
			int32_t scaleY);

		void Queue2DSprite(float xPosition,
			float yPosition,
			float width,
			float height,
			Vector2F* uvTopLeft,
			Vector2F* uvBottomRight,
			int32_t textureIndex,
			RGBA color,
			int32_t flags);

		void ResetQueue();
		void DrawQueuedSprite();
	}
}