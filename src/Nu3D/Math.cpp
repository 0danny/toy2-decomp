#include "Nu3D/Math.h"

namespace Nu3D
{
	namespace Math
	{
		// NOTE: Methods in this class are kind of critical to get correct, since minor changes in how
		// math is calculated could cause weeks of debugging effort to fix. So I take extra care ensuring
		// that most, if not all of these methods are instruction matched.

		// GLOBAL: TOY2 0x004DDA48
		D3DMATRIX g_identityMatrix = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };

		// FUNCTION: TOY2 0x004A9400 [MATCHED]
		void BuildIdentityMatrix(D3DMATRIX* matrix) { memcpy(matrix, &g_identityMatrix, sizeof(D3DMATRIX)); }

		// FUNCTION: TOY2 0x004A9860 [MATCHED]
		void ScaleMatrixByVector(D3DMATRIX* matrix, Vector3F* vector)
		{
			matrix->_11 = matrix->_11 * vector->x;
			matrix->_12 = vector->y * matrix->_12;
			matrix->_13 = vector->z * matrix->_13;
			matrix->_21 = matrix->_21 * vector->x;
			matrix->_22 = matrix->_22 * vector->y;
			matrix->_23 = matrix->_23 * vector->z;
			matrix->_31 = matrix->_31 * vector->x;
			matrix->_32 = matrix->_32 * vector->y;
			matrix->_33 = matrix->_33 * vector->z;
			matrix->_41 = matrix->_41 * vector->x;
			matrix->_42 = matrix->_42 * vector->y;
			matrix->_43 = matrix->_43 * vector->z;
		}

		// FUNCTION: TOY2 0x004A9D50 [MATCHED]
		void RotateZFromLut(D3DMATRIX* matrix, int32_t trigOffset)
		{
			float cos = Numerics::g_trigLUT[(trigOffset + 0x4000) & 0xFFFF];
			float sin = Numerics::g_trigLUT[trigOffset & 0xFFFF];

			float r1 = matrix->_11;
			float r0 = r1 * cos - sin * matrix->_12;
			float r2 = matrix->_21;
			float r3 = matrix->_31;

			matrix->_11 = r0;

			float r4 = matrix->_41;
			matrix->_12 = r1 * sin + cos * matrix->_12;

			matrix->_21 = r2 * cos - sin * matrix->_22;
			matrix->_22 = cos * matrix->_22 + r2 * sin;

			matrix->_31 = r3 * cos - sin * matrix->_32;
			matrix->_32 = cos * matrix->_32 + r3 * sin;

			matrix->_41 = r4 * cos - sin * matrix->_42;
			matrix->_42 = cos * matrix->_42 + r4 * sin;
		}

		// FUNCTION: TOY2 0x004A9B40 [MATCHED]
		void RotateYFromLut(D3DMATRIX* matrix, int32_t trigOffset)
		{
			float cos = Numerics::g_trigLUT[(trigOffset + 0x4000) & 0xFFFF];
			float sin = Numerics::g_trigLUT[trigOffset & 0xFFFF];

			float _11 = matrix->_11;

			float _21 = matrix->_21;
			float _31 = matrix->_31;

			matrix->_11 = matrix->_13 * sin + _11 * cos;

			float _41 = matrix->_41;
			matrix->_13 = matrix->_13 * cos - _11 * sin;

			matrix->_21 = matrix->_23 * sin + _21 * cos;
			matrix->_23 = matrix->_23 * cos - _21 * sin;

			matrix->_31 = matrix->_33 * sin + _31 * cos;
			matrix->_33 = matrix->_33 * cos - _31 * sin;

			matrix->_41 = matrix->_43 * sin + _41 * cos;
			matrix->_43 = matrix->_43 * cos - _41 * sin;
		}

		// FUNCTION: TOY2 0x004A9930 [MATCHED]
		void PostRotateXFromLut(D3DMATRIX* matrix, int32_t trigOffset)
		{
			float cosAngle = Numerics::g_trigLUT[(trigOffset + 0x4000) & 0xFFFF];
			float sinAngle = Numerics::g_trigLUT[trigOffset & 0xFFFF];

			float temp12 = matrix->_12;
			float temp13 = temp12 * cosAngle - sinAngle * matrix->_13;

			float temp22 = matrix->_22;
			float temp32 = matrix->_32;

			matrix->_12 = temp13;

			float temp42 = matrix->_42;
			matrix->_13 = cosAngle * matrix->_13 + temp12 * sinAngle;

			matrix->_22 = temp22 * cosAngle - sinAngle * matrix->_23;
			matrix->_23 = cosAngle * matrix->_23 + temp22 * sinAngle;

			matrix->_32 = temp32 * cosAngle - sinAngle * matrix->_33;
			matrix->_33 = cosAngle * matrix->_33 + temp32 * sinAngle;

			matrix->_42 = temp42 * cosAngle - sinAngle * matrix->_43;
			matrix->_43 = temp42 * sinAngle + cosAngle * matrix->_43;
		}

		// FUNCTION: TOY2 0x004A9750 [MATCHED]
		void AddWorldSpaceTransform(D3DMATRIX* matrix, Vector3F* offset)
		{
			matrix->_41 = offset->x + matrix->_41;
			matrix->_42 = offset->y + matrix->_42;
			matrix->_43 = offset->z + matrix->_43;
		}

		// FUNCTION: TOY2 0x004A8D20 [MATCHED]
		void TransformVectorByMatrix(Vector3F* result, Vector3F* sourceVector, D3DMATRIX* matrix)
		{
			float transformedY = matrix->_32 * sourceVector->z + matrix->_12 * sourceVector->x + matrix->_22 * sourceVector->y;
			float tempZ = matrix->_33 * sourceVector->z + matrix->_13 * sourceVector->x + matrix->_23 * sourceVector->y;

			float transformedX = matrix->_31 * sourceVector->z + matrix->_21 * sourceVector->y;
			float transformedZ = sourceVector->x * matrix->_11;

			result->x = transformedX + transformedZ;
			result->y = transformedY;
			result->z = tempZ;
		}
	}
}