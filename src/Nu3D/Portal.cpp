#pragma once

#include "Nu3D/Portal.h"
#include "Nu3D/Math.h"
#include "Numerics.h"
#include "NGNLoader/NGNLoader.h"
#include <cmath>

namespace Nu3D
{
	// GLOBAL: TOY2 0x00A4C414
	uint8_t Portal::g_visibleAreaFlags[64];

	// FUNCTION: TOY2 0x004B33B0
	void Portal::AreaPortal::CalculateBoundingSphere(Portal::AreaPortal* portal)
	{
		float z = 3.4028235e38;
		float y = 3.4028235e38;
		float x = 3.4028235e38;

		int32_t vertexCount = portal->vertexCount;

		Vector3F vector;
		vector.z = -3.4028235e38;
		vector.y = -3.4028235e38;
		vector.x = -3.4028235e38;

		if (vertexCount > 0)
		{
			Vector3F* vertices = portal->vertices;
			int32_t verticesLeft = vertexCount;

			do
			{
				if (vertices->x <= x)
					x = vertices->x;

				if (vertices->x >= vector.x)
					vector.x = vertices->x;

				if (vertices->y <= y)
					y = vertices->y;

				if (vertices->y >= vector.y)
					vector.y = vertices->y;

				if (z >= vertices->z)
					z = vertices->z;

				if (vertices->z >= vector.z)
					vector.z = vertices->z;

				++vertices;
				--verticesLeft;

			} while (verticesLeft);
		}

		portal->center.x = (vector.x + x) * 0.5;
		portal->center.y = (vector.y + y) * 0.5;
		portal->center.z = (vector.z + z) * 0.5;

		Nu3D::Math::VertexSubtract(&vector, &vector, &portal->center);

		double radSqrt = vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;

		// TODO: naming is wrong here
		portal->radius = radSqrt;
		portal->radiusSquared = sqrt(radSqrt);
	}

	// FUNCTION: TOY2 0x004BC3A0
	Portal::ScalerEntry* Portal::AreaPortal::AllocScalerEntry(NGNLoader::NGNImage* image)
	{
		if (image->scalerEntryCount < image->maxScalerEntries)
		{
			ScalerEntry* pool = image->scalerEntryPool;

			if (! pool)
				return 0;

			ScalerEntry* newAlloc = &pool[image->scalerEntryCount];
			
			image->scalerEntryCount = image->scalerEntryCount + 1;

			return newAlloc;
		}

		return 0;
	}

	// FUNCTION: TOY2 0x004BC360
	int32_t Portal::AreaPortal::BuildScalerEntry(NGNLoader::NGNImage* image, int32_t index, Link::DynamicScaler* scaler)
	{
		Portal::ScalerEntry* head = Portal::AreaPortal::AllocScalerEntry(image);

		if (! head)
			return 0;

		head->scaler = scaler;
		head->next = image->portalHashTable->buckets[index].scalerHead;

		image->portalHashTable->buckets[index].scalerHead = head;

		return 1;
	}

	// FUNCTION: TOY2 0x004BC120
	void Portal::ClearVisibleAreaFlags()
	{
		g_visibleAreaFlags[0] = 1;
		memset(&g_visibleAreaFlags[1], 0, sizeof(g_visibleAreaFlags) - 1);
	}
}