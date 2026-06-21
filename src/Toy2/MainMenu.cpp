#include "Toy2/MainMenu.h"
#include "Toy2/Toy2.h"
#include "AudioManager/AudioManager.h"
#include "InputManager.h"
#include "Nu3D/Camera.h"
#include "SoftwareRenderer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Sprite.h"
#include "Nullsub.h"

namespace Toy2
{
	namespace MainMenu
	{
		// GLOBAL: TOY2 0x0053CA60
		int32_t g_fadeTimer;

		// GLOBAL: TOY2 0x0053E4A8
		int32_t g_nextScreen;

		// GLOBAL: TOY2 0x00559E84
		RGB32 g_menuClearColor;

		// FUNCTION: TOY2 0x00441980
		void RenderMenu()
		{
			Toy2::ProcessMiscEventsEx();

			RGBA color;
			color.a = -1;
			color.b = g_menuClearColor.r;
			color.g = g_menuClearColor.g;
			color.r = g_menuClearColor.b;

			Toy2::g_showBlackFrames = 1;
			Renderer::Sprite::g_parallaxDepthZPos = 0.80000001;
			Renderer::ClearScreen(color, 2);

			if (Renderer::BeginScene())
			{
				Renderer::ResetParallax();
				Renderer::RenderParallaxBackground(0);

				Renderer::DrawTintOverlay();

				Renderer::FlushRenderQueues();
				Renderer::Sprite::DrawQueuedSprite();
				DevDraw::DrawSlots();

				Renderer::DoFrameDelay(0);
				Renderer::EndScene(1);
			}
		}

		// FUNCTION: TOY2 0x00437C40
		int32_t Draw()
		{
			InputManager::g_curButtonsPressed = 0;
			InputManager::g_prevButtonsPressed = 0;

			g_fadeTimer = 0;
			g_nextScreen = 0;

			Nu3D::Camera::g_cameraTintBlue = 0;
			Nu3D::Camera::g_cameraTintGreen = 0;
			Nu3D::Camera::g_cameraTintRed = 0;

			Nu3D::Camera::SetTint(128, 128, 128, 12);
			SoftwareRenderer::UnkFunc67(0, 0);
			Renderer::g_frameDelta = 1;
			Toy2::SetBackdropByIndex(4);

			int32_t selectedY = 120;
			int32_t idleTimer = 0;
			int32_t cursorY = 120;
			bool cursorBelowTarget;

			AudioManager::PlayMusicOneShot(19);

			while (! g_nextScreen)
			{
			LBL_FRAME_START:

				Nu3D::Camera::FadeToTargetTint();

				uint32_t frameDelta = Renderer::g_frameDelta;

				if (g_fadeTimer > 0)
				{
					g_fadeTimer -= Renderer::g_frameDelta;

					if (g_fadeTimer <= 0)
						g_fadeTimer = 0;
				}

				idleTimer += Renderer::g_frameDelta;

				if (g_nextScreen)
				{
					cursorBelowTarget = cursorY < selectedY;
					goto LBL_ANIMATE_CURSOR;
				}

				cursorBelowTarget = cursorY < selectedY;

				if (cursorY != selectedY)
					goto LBL_ANIMATE_CURSOR;

				if ((InputManager::g_curButtonsPressed & INPUT_JUMP) == 0 || (InputManager::g_prevButtonsPressed & INPUT_JUMP) != 0 || idleTimer <= 30)
				{
					if ((InputManager::g_curButtonsPressed & INPUT_DOWN) != 0 && (InputManager::g_prevButtonsPressed & INPUT_DOWN) == 0 && selectedY < 200)
					{
						selectedY += 20;
						AudioManager::PlayOneShotSoundGlobal(1, 4608, 80, 80);

						frameDelta = Renderer::g_frameDelta;
					}

					if ((InputManager::g_curButtonsPressed & INPUT_UP) == 0 || (InputManager::g_prevButtonsPressed & INPUT_UP) != 0 || selectedY <= 120)
					{
						cursorBelowTarget = cursorY < selectedY;
						goto LBL_ANIMATE_CURSOR;
					}

					selectedY -= 20;

					AudioManager::PlayOneShotSoundGlobal(1, 4608, 80, 80);
				}
				else
				{
					switch (cursorY)
					{
						case 120:
							g_nextScreen = 2;
							break;

						case 140:
							g_nextScreen = 3;
							break;

						case 160:
							g_nextScreen = 4;
							break;
						case 180:

							g_nextScreen = 5;
							break;

						case 200:
							exit(-1);
					}

					g_fadeTimer = 23;
					Nu3D::Camera::SetTint(0, 0, 0, 12);
					AudioManager::PlayOneShotSoundGlobal(0, 4608, 80, 80);
				}

				frameDelta = Renderer::g_frameDelta;
				cursorBelowTarget = cursorY < selectedY;

			LBL_ANIMATE_CURSOR:

				if (cursorBelowTarget)
				{
					cursorY += 2 * frameDelta;

					if (cursorY < selectedY)
						goto LBL_DRAW_FRAME;
				}
				else
				{
					cursorY -= 2 * frameDelta;

					if (cursorY > selectedY)
						goto LBL_DRAW_FRAME;
				}

				cursorY = selectedY;

			LBL_DRAW_FRAME:

				uint32_t bounce = idleTimer & 63;

				if (bounce)
					bounce = 63 - bounce;

				uint32_t spriteScale = 4 * bounce;

				Renderer::Sprite::DrawScaled(64, cursorY, 62, 0, 4 * bounce, 4 * bounce, 128, 255, 2048, 2048);
				Renderer::Sprite::DrawScaled(240, cursorY, 62, 1, spriteScale, spriteScale, 128, 255, 2048, 2048);

				if (Toy2::g_saveLoaded)
					Renderer::DrawMainMenuText(120, "continue game", 128);
				else
					Renderer::DrawMainMenuText(120, "start game", 128);

				Renderer::DrawMainMenuText(140, "options", 128);
				Renderer::DrawMainMenuText(160, "load game", 128);
				Renderer::DrawMainMenuText(180, "movie viewer", 128);
				Renderer::DrawMainMenuText(200, "exit", 128);

				Nullsub3();

				RenderMenu();

				if (g_attractModeTimer >= 0 && ((InputManager::g_curButtonsPressed & INPUT_SECRET_MENU) != 0 || idleTimer >= g_attractModeTimer))
				{
					if (g_nextScreen)
						break;

					g_nextScreen = 1;
					g_fadeTimer = 23;

					Nu3D::Camera::SetTint(0, 0, 0, 12);
				}
			}

			if (g_fadeTimer)
				goto LBL_FRAME_START;

			AudioManager::StopAndWait();

			return g_nextScreen;
		}

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
				Renderer::g_frameDelta = 1;

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
						g_fadeTimer -= Renderer::g_frameDelta;

						if (g_fadeTimer <= 0)
							g_fadeTimer = 0;
					}

					elapsedTicks += Renderer::g_frameDelta;
					int32_t pulsePhase = elapsedTicks & 63;

					if (pulsePhase > 31)
						pulsePhase = 63 - pulsePhase;

					if (showPressJumpPrompt)
						Renderer::DrawMainMenuText(204, "press jump", 4 * pulsePhase);

					Nullsub3();
					RenderMenu();

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