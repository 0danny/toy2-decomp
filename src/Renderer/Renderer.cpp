#include "Renderer.h"

namespace Renderer
{
	// $GLOBAL 00884484
	int32_t g_isSoftwareRendering;

	// $FUNC 004B3630 [UNFINISHED]
	void Init() {}

	// $FUNC 004B3860 [IMPLEMENTED]
	void SetIsSoftwareRendering(int32_t value) { g_isSoftwareRendering = value; }
}