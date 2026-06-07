#pragma once

#include "Common.h"
#include "Numerics.h"
#include "Viewport.h"
#include "Renderer/RenderType.h"

namespace Nu3D
{
	struct Sprite
	{
		Sprite* next;
		Renderer::RenderType type;
		float distanceSquared;
		Vector3F position;
		Vector3F triVerts[3];
		int32_t trigIndex;
		float width;
		float height;
		Vector2F uvBottomLeft;
		Vector2F uvTopLeft;
		Vector2F uvBottomRight;
		Vector2F uvTopRight;
		int32_t textureIndex;
		RGBA color;
		int32_t renderFlags;
		Viewport::ViewportRect viewportRect;
	};

	STATIC_ASSERT(sizeof(Sprite) == 0x84);
}