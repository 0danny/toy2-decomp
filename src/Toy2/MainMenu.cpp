#include "Toy2/MainMenu.h"
#include "Toy2/Toy2.h"
#include "AudioManager/AudioManager.h"
#include "InputManager.h"
#include "Nu3D/Camera.h"
#include "SoftwareRenderer.h"
#include "Renderer/Renderer.h"
#include "Nullsub.h"

namespace Toy2
{
	namespace MainMenu
	{
		// GLOBAL: TOY2 0x0053CA60
		int32_t g_fadeTimer;

		// GLOBAL: TOY2 0x0053E4A8
		int32_t g_nextScreen;

		// STUB: TOY2 0x00437C40
		int32_t Draw() { return 0; }

		// FUNCTION: TOY2 0x00437FB0
		int32_t Tick()
		{
			if (g_mainMenuState != -1)
				goto LBL_INIT_TITLE_SCREEN;

			int32_t nextScreenResult;

			do
			{
				nextScreenResult = Draw();
				g_nextScreen = nextScreenResult;

				if (nextScreenResult != 1)
					break;

			LBL_INIT_TITLE_SCREEN:

				InputManager::g_curButtonsPressed = 0;
				InputManager::g_prevButtonsPressed = 0;

				g_fadeTimer = 0;
				g_nextScreen = 0;

				Nu3D::Camera::g_cameraTintBlue = 0;
				Nu3D::Camera::g_cameraTintGreen = 0;
				Nu3D::Camera::g_cameraTintRed = 0;

				Nu3D::Camera::SetTint(128, 128, 128, 12);
				SoftwareRenderer::UnkFunc67(0, 0);

				int32_t showPressJumpPrompt = 1;
				g_frameDelta = 1;

				int32_t timeoutTicks;

				if (g_returnedToTitle == 1)
				{
					timeoutTicks = 300;
					showPressJumpPrompt = 0;
				}
				else
				{
					timeoutTicks = 900;
				}

				SetBackdropByIndex(0);

				int32_t elapsedTicks = 0;

				AudioManager::PlayMusicOneShot(20);

				while (! g_nextScreen)
				{
				LBL_TITLE_FRAME:

					Nu3D::Camera::FadeToTargetTint();

					if (g_fadeTimer > 0)
					{
						g_fadeTimer -= g_frameDelta;

						if (g_fadeTimer <= 0)
							g_fadeTimer = 0;
					}

					elapsedTicks += g_frameDelta;
					int32_t pulsePhase = elapsedTicks & 63;

					if (pulsePhase > 31)
						pulsePhase = 63 - pulsePhase;

					if (showPressJumpPrompt)
						Renderer::DrawMainMenuText(204, "press jump", 4 * pulsePhase);

					Nullsub3();

					Renderer::RenderMenu();

					if (g_attractModeTimer < 0)
					{
						if (elapsedTicks > timeoutTicks && ! g_nextScreen)
						{
							g_nextScreen = 1;

						LBL_BEGIN_FADE_OUT:

							g_fadeTimer = 23;
							Nu3D::Camera::SetTint(0, 0, 0, 12);
						}
					}
					else if (((InputManager::g_curButtonsPressed & INPUT_SECRET_MENU) != 0 || elapsedTicks > timeoutTicks) && ! g_nextScreen)
					{
						if (showPressJumpPrompt)
							g_nextScreen = 10;
						else
							g_nextScreen = (InputManager::g_curButtonsPressed & INPUT_SECRET_MENU) != 0 ? 10 : 1;

						goto LBL_BEGIN_FADE_OUT;
					}

					if ((InputManager::g_curButtonsPressed & INPUT_JUMP) != 0 && (InputManager::g_prevButtonsPressed & INPUT_JUMP) == 0)
					{
						if (g_nextScreen)
							break;

						if (showPressJumpPrompt)
						{
							if (elapsedTicks > 30)
							{
								g_nextScreen = 2;
								g_fadeTimer = 23;

								Nu3D::Camera::SetTint(0, 0, 0, 12);
								AudioManager::PlayOneShotSoundGlobal(0, 4608, 80, 80);
							}
						}
					}
				}

				if (g_fadeTimer)
					goto LBL_TITLE_FRAME;

				AudioManager::StopAndWait();

				if (g_attractModeTimer >= 0)
					return g_nextScreen - 1;

				nextScreenResult = g_nextScreen;

			} while (g_nextScreen == 2);

			return nextScreenResult - 1;
		}

		// STUB: TOY2 0x004371B0
		void ShowSettings() {}
	}
}