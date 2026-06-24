#include "Nu3D/Camera.h"
#include "Renderer/Renderer.h"

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

		// GLOBAL: TOY2 0x0052ADB8
		uint8_t g_targetTintBlue;

		// GLOBAL: TOY2 0x0052AD8C
		uint8_t g_targetTintGreen;

		// GLOBAL: TOY2 0x0052C820
		uint8_t g_targetTintRed;

		// GLOBAL: TOY2 0x00529E50
		uint8_t g_targetTintFadeSpeed;

		// GLOBAL: TOY2 0x00557A9C
		int16_t g_tintBlend;

		// FUNCTION: TOY2 0x004A1BB0
		void SetTint(uint8_t blue, uint8_t green, uint8_t red, uint8_t fadeSpeed)
		{
			g_targetTintBlue = blue;
			g_targetTintGreen = green;
			g_targetTintRed = red;
			g_targetTintFadeSpeed = fadeSpeed;
			g_tintBlend = -1;
		}

		// STUB: TOY2 0x0044DF90
		void InitViewMatrixGlobals() {}

		// FUNCTION: TOY2 0x004A1BE0
		void FadeToTargetTint()
		{
			int32_t anyChannelChanged = 0;
			int32_t fadeStep = Renderer::g_frameDelta * g_targetTintFadeSpeed / 2;

			if (! fadeStep)
			{
				g_targetTintFadeSpeed = 0;
				return;
			}

			if (g_cameraTintBlue != g_targetTintBlue)
			{
				anyChannelChanged = 1;

				if (g_cameraTintBlue >= g_targetTintBlue)
				{
					g_cameraTintBlue -= fadeStep;

					if (g_cameraTintBlue >= g_targetTintBlue)
						goto LBL_FADE_GREEN;
				}
				else
				{
					g_cameraTintBlue += fadeStep;

					if (g_cameraTintBlue <= g_targetTintBlue)
						goto LBL_FADE_GREEN;
				}
				g_cameraTintBlue = g_targetTintBlue;
			}

		LBL_FADE_GREEN:

			if (g_cameraTintGreen == g_targetTintGreen)
				goto LBL_FADE_RED;

			anyChannelChanged = 1;

			if (g_cameraTintGreen >= g_targetTintGreen)
			{
				g_cameraTintGreen -= fadeStep;
				if (g_cameraTintGreen >= g_targetTintGreen)
					goto LBL_FADE_RED;
			}
			else
			{
				g_cameraTintGreen += fadeStep;

				if (g_cameraTintGreen <= g_targetTintGreen)
					goto LBL_FADE_RED;
			}

			g_cameraTintGreen = g_targetTintGreen;

		LBL_FADE_RED:

			if (g_cameraTintRed == g_targetTintRed)
			{
				if (anyChannelChanged)
					return;

				g_targetTintFadeSpeed = 0;
				return;
			}

			if (g_cameraTintRed >= g_targetTintRed)
			{
				g_cameraTintRed -= fadeStep;
				if (g_cameraTintRed < g_targetTintRed)
					g_cameraTintRed = g_targetTintRed;
			}
			else
			{
				g_cameraTintRed += fadeStep;
				if (g_cameraTintRed > g_targetTintRed)
					g_cameraTintRed = g_targetTintRed;
			}
		}

		// STUB: TOY2 0x004CE050
		void ApplyTransformToCamera(ActiveCameraTransform* camera) {}
	}
}