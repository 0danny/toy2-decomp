#pragma once

#include "Numerics.h"
#include <directx6/ddraw.h>
#include <directx6/d3d.h>

namespace Nu3D
{
	namespace Math
	{
		void BuildIdentityMatrix(D3DMATRIX* matrix);
		void ScaleMatrixByVector(D3DMATRIX* matrix, Vector3F* vector);
		void RotateZFromLut(D3DMATRIX* matrix, int32_t trigOffset);
		void RotateYFromLut(D3DMATRIX* matrix, int32_t trigOffset);
		void PostRotateXFromLut(D3DMATRIX* matrix, int32_t trigOffset);
		void AddWorldSpaceTransform(D3DMATRIX* matrix, Vector3F* offset);
		void TransformVectorByMatrix(Vector3F* result, Vector3F* sourceVector, D3DMATRIX* matrix);
	}
}