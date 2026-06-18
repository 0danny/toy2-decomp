#pragma once

#include "Common.h"
#include "Numerics.h"

namespace Toy2
{
	namespace Collision
	{
		struct CollisionMeshInstance
		{
			Vector3I origin;
			int32_t platformIdx;
			int32_t collisionTree;
			Vector3I boundsMin;
			Vector3I boundsExt;
			int16_t typeFlags;
			int16_t unk;
			int32_t boundingSqRadius;
		};

        extern CollisionMeshInstance g_collisionMeshInstances[300];
        extern uint8_t g_mathScratch[1024];

        void BuildCollisionWorld(int32_t level, uint8_t **buffer, int32_t terrainNum);

		STATIC_ASSERT(sizeof(CollisionMeshInstance) == 0x34);
	}
}