#pragma once

#include "Portal.h"

namespace Nu3D
{
	namespace Portal
	{
		// $GLOBAL 00A4C414
		uint8_t g_visibleAreaFlags[64];

		// $FUNC 004BC120 [IMPLEMENTED]
		void ClearVisibleAreaFlags()
		{
			g_visibleAreaFlags[0] = 1;
			memset(&g_visibleAreaFlags[1], 0, sizeof(g_visibleAreaFlags) - 1);
		}
	}
}