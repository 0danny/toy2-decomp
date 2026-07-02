#include "Numerics.h"
#include <cstdlib>
#include <cmath>

namespace Numerics
{
	// GLOBAL: TOY2 0x0088334C
	float* g_trigLUT;

	// FUNCTION: TOY2 0x004A91E0
	void VertexSubtract(Vector3F* result, Vector3F* v1, Vector3F* v2)
	{
		result->x = v1->x - v2->x;
		result->y = v1->y - v2->y;
		result->z = v1->z - v2->z;
	}

	// FUNCTION: TOY2 0x004B0740
	int32_t RoundUpToPowerOf2(int32_t number)
	{
		int32_t result;

		for (result = 16; result < number; result *= 2) {};

		return result;
	}

	// FUNCTION: TOY2 0x004A88B0 [MATCHED]
	void InitTrigLut()
	{
		if (! g_trigLUT)
		{
			g_trigLUT = (float*)malloc(sizeof(float) * 0x10000);

			int32_t i = 0;

			do
			{
				g_trigLUT[i] = sin(i * 9.58738019107841e-05f);
				++i;
			} while (i < 0x10000);
		}
	}
}