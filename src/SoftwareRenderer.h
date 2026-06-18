#pragma once

#include "Common.h"
#include "Numerics.h"

namespace SoftwareRenderer
{
	extern PointI g_unk4F7400;
	extern int32_t g_unk500A1C;

	void SwapRenderBuffer();
	void SetLevelFileIndex(int32_t index);
}