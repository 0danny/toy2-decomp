#pragma once

#include "Common.h"

namespace Nu3D
{
    namespace Viewport
    {
        struct ViewportRect
        {
            float top;
            float bottom;
            float left;
            float right;
        };
    }

	STATIC_ASSERT(sizeof(Viewport::ViewportRect) == 0x10);
}