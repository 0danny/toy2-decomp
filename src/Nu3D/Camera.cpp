#include "Nu3D/Camera.h"

namespace Nu3D
{
	namespace Camera
	{
		// GLOBAL: TOY2 0x0054DE9C
		int16_t g_cameraTintBlue;

		// GLOBAL: TOY2 0x00554038
		int16_t g_cameraTintGreen;

		// GLOBAL: TOY2 0x0054DD6C
		int16_t g_cameraTintRed;

		// STUB: TOY2 0x004A1BB0
		void SetTint(uint8_t blue, uint8_t green, uint8_t red, uint8_t fadeSpeed) {}

		// STUB: TOY2 0x0044DF90
		void InitViewMatrixGlobals() {}

		// STUB: TOY2 0x004A1BE0
		void FadeToTargetTint() {}

		// STUB: TOY2 0x004CE050
		void ApplyTransformToCamera(ActiveCameraTransform* camera) {}
	}
}