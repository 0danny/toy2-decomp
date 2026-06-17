#include "Nu3D/Nu3D.h"

namespace Nu3D
{
	// GLOBAL: TOY2 0x00884470
	int32_t g_isSoftwareRendering;

	// FUNCTION: TOY2 0x004B1880
	void SetIsSoftwareRendering(int32_t value) { g_isSoftwareRendering = value; }
}