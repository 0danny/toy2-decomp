#pragma once

#include "Common.h"
#include "Numerics.h"

namespace Nu3D
{
	namespace Camera
	{
		struct ActiveCameraTransform
		{
			Vector3I pos;
			Angles angles;
			int16_t roll;
			int16_t unkShort;
		};

		extern int16_t g_cameraTintBlue;
		extern int16_t g_cameraTintGreen;
		extern int16_t g_cameraTintRed;

		void SetTint(uint8_t blue, uint8_t green, uint8_t red, uint8_t fadeSpeed);
		void InitViewMatrixGlobals();
		void FadeToTargetTint();
		void ApplyTransformToCamera(ActiveCameraTransform* camera);

		STATIC_ASSERT(sizeof(ActiveCameraTransform) == 0x14);
	}
}