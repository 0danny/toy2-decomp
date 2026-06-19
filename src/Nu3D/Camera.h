#pragma once

#include "Common.h"

namespace Nu3D
{
	namespace Camera
	{
		extern int16_t g_cameraTintBlue;
		extern int16_t g_cameraTintGreen;
		extern int16_t g_cameraTintRed;

		void SetTint(uint8_t blue, uint8_t green, uint8_t red, uint8_t fadeSpeed);
		void InitViewMatrixGlobals();
        void FadeToTargetTint();
	}
}