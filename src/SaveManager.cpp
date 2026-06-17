#include "SaveManager.h"

namespace SaveManager
{
	// GLOBAL: TOY2 0x0052EF90
	Save0Data g_save0Data;

	// GLOBAL: TOY2 0x00529B08
	Save99Data g_save99Data;

    // GLOBAL: TOY2 0x00830CA8
	uint32_t g_curLevelTokenData;

	// STUB: TOY2 0x004A2C20
	void InitProgressData(Save0Data* save) {}

	// STUB: TOY2 0x004A2CC0
	void LoadProgressData(Save0Data* save) {}
}