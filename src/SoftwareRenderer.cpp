#include "SoftwareRenderer.h"

namespace SoftwareRenderer
{
	// GLOBAL: TOY2 0x004F7400
	PointI g_unk4F7400 = { -32768, -32768 };

	// GLOBAL: TOY2 0x00500A1C
	int32_t g_unk500A1C = 0xFFFFFFFF;

	// GLOBAL: TOY2 0x00A4CC74
	int32_t g_levelFileIndex;

	// STUB: TOY2 0x00452130
	void SwapRenderBuffer() {}

	// FUNCTION: TOY2 0x004C20E0
	void SetLevelFileIndex(int32_t index) { g_levelFileIndex = index; }
}