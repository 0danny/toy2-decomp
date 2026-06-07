#pragma once

#include "Numerics.h"
#include <directx7/d3d.h>

namespace Nu3D
{
	namespace Link
	{
		struct DynamicScaler
		{
			DynamicScaler* next;
			DynamicScaler* prev;
			DynamicScaler* cellHead;
			Vector3F translation;
			Vector3I rotation;
			Vector3F scale;
			D3DMATRIX transformMatrix;
			int32_t shapeId;
			Vector3F unkVector4;
			int32_t unkVar33;
			int32_t unkVar34;
			int32_t unkVar35;
			int32_t flags;
			int32_t gscaleType;
		};

		struct Linker
		{
			Vector3F currentPos;
			Vector3F targetPos;
			Vector3I currentRot;
			Vector3F currentScale;
			DynamicScaler* dynamicScaler;
			D3DMATRIX transformMatrix;
		};

		STATIC_ASSERT(sizeof(DynamicScaler) == 0x94);
		STATIC_ASSERT(sizeof(Linker) == 0x74);
	}

	namespace Spatial
	{
		
	}
}