#pragma once

#include "Nu3D/Portal.h"
#include "Numerics.h"
#include <cmath>

namespace Nu3D
{
	// $GLOBAL 00A4C414
	uint8_t Portal::g_visibleAreaFlags[64];

	// $FUNC 004B33B0 [IMPLEMENTED]
	void Portal::AreaPortal::CalculateBoundingSphere(Nu3D::Portal::AreaPortal* portal)
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

		Numerics::VertexSubtract(&vector, &vector, &portal->center);

		double radSqrt = vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;

		// TODO: naming is wrong here
		portal->radius = radSqrt;
		portal->radiusSquared = sqrt(radSqrt);
	}

	// $FUNC 004BC120 [IMPLEMENTED]
	void Portal::ClearVisibleAreaFlags()
	{
		g_visibleAreaFlags[0] = 1;
		memset(&g_visibleAreaFlags[1], 0, sizeof(g_visibleAreaFlags) - 1);
	}
}