#pragma once

#include "Common.h"

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
			int16_t flags,
			int32_t scaleX,
			int32_t scaleY);
	}
}