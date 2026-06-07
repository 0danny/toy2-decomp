#pragma once

#include "Common.h"
#include <directx7/d3d.h>
#include "RenderType.h"
#include "Nu3D/Sprite.h"

namespace Nu3D
{
	struct Primitive;
	struct Material;
}

namespace Renderer
{
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
		Nu3D::Viewport::ViewportRect clipRect;
		Nu3D::Sprite sprite;
	};

	struct RenderEntry
	{
		RenderEntry* next;
		Renderer::RenderType type;
		float distanceSquared;
		Nu3D::Primitive* primitive;
		InstanceData* instanceData;
		Nu3D::Material* material;
	};

	STATIC_ASSERT(sizeof(InstanceData) == 0x1B8);
	STATIC_ASSERT(sizeof(RenderEntry) == 0x18);
}