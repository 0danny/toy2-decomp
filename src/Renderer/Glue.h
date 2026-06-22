#pragma once

#include "Common.h"
#include <WINDOWS.H>

namespace Renderer
{
	namespace Glue
	{
		int32_t BackdropBltFast();
		HBITMAP SetBackdrop(int32_t textureIndex);
	}
}