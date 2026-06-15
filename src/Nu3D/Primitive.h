#pragma once

#include "Common.h"
#include "Nu3D/Material.h"
#include "Nu3D/Patch.h"

namespace Nu3D
{
	struct Primitive
	{
		struct Header
		{
			int32_t drawType;
			int32_t indexCount;
			uint16_t* indices;
			uint8_t* triMeta;
		};

		Primitive* next;
		Primitive* prev;
		Primitive* listNext;
		int32_t renderFlags;
		Material* material;
		Patch::PatchVertices patchVerts;
		int32_t headerCount;
		Header* header;
		float originRadius;
		float originRadiusSq;
		Vector3F boundsCenter;
		float boundSphereRadius;
		float boundSphereRadiusSq;
	};

	extern Primitive *g_primListHead;

	STATIC_ASSERT(sizeof(Primitive) == 0x4C);
	STATIC_ASSERT(sizeof(Primitive::Header) == 0x10);
}