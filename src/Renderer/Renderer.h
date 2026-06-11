#pragma once

#include "Common.h"
#include "RenderType.h"
#include <directx7/d3d.h>

namespace Nu3D
{
	struct Primitive;
	struct Material;
	struct InstanceData;
}

namespace Renderer
{
	struct RenderEntry
	{
		RenderEntry* next;
		Renderer::RenderType type;
		float distanceSquared;
		Nu3D::Primitive* primitive;
		Nu3D::InstanceData* instanceData;
		Nu3D::Material* material;
	};

	extern int32_t g_isSoftwareRendering;

	void Init();
	void SetIsSoftwareRendering(int32_t value);

	STATIC_ASSERT(sizeof(RenderEntry) == 0x18);
}