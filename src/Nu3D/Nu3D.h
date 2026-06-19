#pragma once

#include "Common.h"
#include "Numerics.h"

namespace Nu3D
{
	struct Vertex
	{
		Vector3F position;
		Vector3F normals;
		RGBA diffuse;
		Vector2F coords;
	};

	struct VertexTL
	{
		Vector3F position;
		float rhw;
		RGBA diffuse;
		RGBA specular;
		Vector2F uv;
	};

	void SetIsSoftwareRendering(int32_t value);

	STATIC_ASSERT(sizeof(VertexTL) == 32);
	STATIC_ASSERT(sizeof(Vertex) == 36);
}