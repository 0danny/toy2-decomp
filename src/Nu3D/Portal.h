#pragma once

#include "Nu3D.h"
#include "Link.h"

namespace Nu3D
{
	namespace Portal
	{
		struct AreaPortal
		{
			int32_t portalId;
			int32_t vertexCount;
			Vector3F center;
			float radiusSquared;
			float radius;
			Vector3F* vertices;

			static void CalculateBoundingSphere(Nu3D::Portal::AreaPortal *portal);
		};

		struct PortalState
		{
			PortalState* next;
			AreaPortal* portal;
			int32_t sourceAreaIdx;
			int32_t targetAreaIdx;
			int32_t visited;
		};

		struct ScalerEntry
		{
			ScalerEntry* next;
			Nu3D::Link::DynamicScaler* scaler;
		};

		struct PortalBucket
		{
			ScalerEntry* scalerHead;
			PortalState* portalStateHead;
		};

		struct PortalHashTable
		{
			PortalBucket buckets[64];
		};

		extern uint8_t g_visibleAreaFlags[64];

		void ClearVisibleAreaFlags();

		STATIC_ASSERT(sizeof(AreaPortal) == 0x20);
		STATIC_ASSERT(sizeof(PortalState) == 0x14);
		STATIC_ASSERT(sizeof(PortalBucket) == 0x8);
		STATIC_ASSERT(sizeof(ScalerEntry) == 0x8);
		STATIC_ASSERT(sizeof(PortalHashTable) == 0x200);
	}
}