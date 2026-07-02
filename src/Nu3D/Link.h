#pragma once

#include "Numerics.h"
#include <directx6/ddraw.h>
#include <directx6/d3d.h>

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
			Vector3F boundsCenterWorld;
			int32_t packedAreaData;
			int32_t areaIndex;
			int32_t packedFlags;
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
	{}
}