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
		void CalculatePlaneFromTriangle(Vector3F* point1, Vector3F* point2, Vector3F* point3, Plane* plane);
		void VertexAdd(Vector3F* result, Vector3F* v1, Vector3F* v2);
		void VertexSubtract(Vector3F* result, Vector3F* v1, Vector3F* v2);
		void VertexCrossProduct(Vector3F* result, Vector3F* v1, Vector3F* v2);
		float VectorNormalize(Vector3F* output, Vector3F* vector);
		void CalculatePlaneFromTriangle(Vector3F* point1, Vector3F* point2, Vector3F* point3, Plane* plane);
	}
}