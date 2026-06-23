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

	extern int32_t g_useAsDiffuseModulation;

	void SetIsSoftwareRendering(int32_t value);
	uint32_t GetHighResolutionTime();
	void PrecisionSleep(int32_t delayMs);

	void SetUseAsDiffuseModulation(int32_t option);
	void SetDefaultPrimFlags(int32_t option);

	STATIC_ASSERT(sizeof(VertexTL) == 32);
	STATIC_ASSERT(sizeof(Vertex) == 36);
}