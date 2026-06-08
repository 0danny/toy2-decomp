#include "Numerics.h"

namespace Numerics
{
    // $FUNC 004A91E0 [IMPLEMENTED]
	void VertexSubtract(Vector3F* result, Vector3F* v1, Vector3F* v2)
	{
		result->x = v1->x - v2->x;
		result->y = v1->y - v2->y;
		result->z = v1->z - v2->z;
	}
}