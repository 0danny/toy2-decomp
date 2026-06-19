#pragma once

#include "Common.h"
#include "Numerics.h"
#include "Nu3D/Viewport.h"
#include "Renderer/RenderType.h"
#include <directx6/ddraw.h>
#include <directx6/d3d.h>

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

	struct InstanceData
	{
		D3DMATRIX matrices[4];
		int32_t renderFlags;
		float lodFactor;
		float horzOffset;
		float vertOffset;
		int32_t renderModeFlags;
		int32_t unkInt6;
		RGB32 vertexModColor;
		Viewport::ViewportRect clipRect;
		Sprite sprite;
	};

	STATIC_ASSERT(sizeof(InstanceData) == 0x1B8);
	STATIC_ASSERT(sizeof(Sprite) == 0x84);
}