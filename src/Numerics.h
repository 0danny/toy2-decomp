#pragma once

#include "Common.h"

struct Vector3F
{
	float x;
	float y;
	float z;
};

struct Vector3I
{
	int32_t x;
	int32_t y;
	int32_t z;
};

struct Vector2F
{
	float x;
	float y;
};

struct Vector2I
{
	int32_t x;
	int32_t y;
};

namespace Numerics
{
	extern float* g_trigLUT;

	void VertexSubtract(Vector3F *result, Vector3F *v1, Vector3F *v2);
	int32_t RoundUpToPowerOf2(int32_t number);
	void InitTrigLut();
}