#include "Nu3D/Nu3D.h"

namespace Nu3D
{
	// $GLOBAL 00884470
	int32_t g_isSoftwareRendering;

	// $FUNC 004B1880 [IMPLEMENTED]
	void SetIsSoftwareRendering(int32_t value) { g_isSoftwareRendering = value; }
}