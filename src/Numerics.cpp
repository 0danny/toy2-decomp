#include "Numerics.h"
#include <cstdlib>
#include <cmath>

namespace Numerics
{
	// $GLOBAL 0088334C
	float* g_trigLUT;

	// $FUNC 004A91E0 [IMPLEMENTED]
	void VertexSubtract(Vector3F* result, Vector3F* v1, Vector3F* v2)
	{
		result->x = v1->x - v2->x;
		result->y = v1->y - v2->y;
		result->z = v1->z - v2->z;
	}

	// $FUNC 004B0740 [IMPLEMENTED]
	int32_t RoundUpToPowerOf2(int32_t number)
	{
		int32_t result;

		for (result = 16; result < number; result *= 2) {};

		return result;
	}

	// $FUNC 004A88B0 [IMPLEMENTED]
	void InitTrigLut()
	{
		if (! g_trigLUT)
		{
			g_trigLUT = (float*)malloc(sizeof(float) * 0x10000);

			int32_t index = 0;
			int32_t nextIndex = 0;

			do
			{
				int32_t currentIndex = nextIndex;
				nextIndex = ++index;

				g_trigLUT[index - 1] = sin(currentIndex * 0.000095873802);
			} while (index < 0x10000);
		}
	}
}