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

	STATIC_ASSERT(sizeof(Vertex) == 0x24);
}