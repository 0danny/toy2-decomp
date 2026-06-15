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

struct Vector3I16
{
	int16_t x;
	int16_t y;
	int16_t z;
};

struct Angles
{
	uint16_t pitch;
	uint16_t yaw;
};

struct PosAndAngles
{
	Vector3I pos;
	Angles angles;
};

namespace Numerics
{
	extern float* g_trigLUT;

	void VertexSubtract(Vector3F* result, Vector3F* v1, Vector3F* v2);
	int32_t RoundUpToPowerOf2(int32_t number);
	void InitTrigLut();
}