#include "Toy2/Toy2.h"
#include "Toy2/D3DApp.h"
#include "Logger.h"
#include "FileUtils.h"
#include "InputManager.h"
#include "DrawingDevice.h"
#include "ModeSelect.h"
#include "Nullsub.h"
#include "SoftwareRenderer.h"
#include "SaveManager.h"
#include "Random.h"
#include "Toy2/LevelSelect.h"
#include "Toy2/Buzz.h"
#include "Toy2/Levels.h"
#include "Toy2/MainMenu.h"

#include "Nu3D/Font.h"
#include "Nu3D/Viewport.h"
#include "Nu3D/Camera.h"
#include "Renderer/Renderer.h"
#include "AudioManager/AudioManager.h"
#include "NGNLoader/NGNLoader.h"

#include <WINDOWS.H>
#include <STDIO.H>
#include <DINPUT.H>

#include <Numerics.h>

namespace Toy2
{
	// GLOBAL: TOY2 0x00508D70
	ToyCfg g_toyCfgData = {
		7, /* flags */
		1, /* detail */
		2.0, /* gamma correction */
		-1, /* driver index */
		-1, /* device index */
		-1, /* display mode index */
	};

	// GLOBAL: TOY2 0x0088278C
	int32_t g_levelFileIndex;

	// GLOBAL: TOY2 0x0052AD9C
	int32_t g_returnedToTitle;

	// GLOBAL: TOY2 0x0052ADA0
	int32_t g_attractModeTimer;

	// GLOBAL: TOY2 0x00A4C454
	int32_t g_isElevatorHopLevel;

	// GLOBAL: TOY2 0x005D2A90
	int32_t g_hasBackdrop;

	// GLOBAL: TOY2 0x004F7280
	int32_t g_showBlackFrames = 1;

	// GLOBAL: TOY2 0x004FCDB4
	int32_t g_cdBaseTrack = 2;

	// GLOBAL: TOY2 0x005281C8
	int32_t g_modeSelectFinished;

	// GLOBAL: TOY2 0x0052ADA4
	int32_t g_unused1;

	// GLOBAL: TOY2 0x0052ADA8
	int32_t g_unused2;

	// GLOBAL: TOY2 0x0072EFD8
	int32_t g_pastInitialBoot;

	// GLOBAL: TOY2 0x0052ADB4
	int32_t g_attractModeInputTimer;

	// GLOBAL: TOY2 0x0052C83C
	int32_t g_curDemoLevel;

	// GLOBAL: TOY2 0x0052B7DC
	int32_t g_levelTransition;

	// GLOBAL: TOY2 0x0052AD94
	int32_t g_demoMode;

	// GLOBAL: TOY2 0x0055A0E0
	int32_t g_hasStaticBackdrop;

	// GLOBAL: TOY2 0x00500A50
	int32_t g_nextBackdropId = 36;

	// GLOBAL: TOY2 0x00830C88
	int32_t g_mainMenuState;

	// GLOBAL: TOY2 0x00830D58
	int32_t g_saveLoaded;

	// GLOBAL: TOY2 0x0052AD8A
	int16_t g_levelIndex;

	// GLOBAL: TOY2 0x0050268C
	int32_t g_levelFileConversion[15] = { 1, 2, 6, 4, 5, 3, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	// GLOBAL: TOY2 0x0052AD7C
	int32_t g_demoPathWriteIdx;

	// GLOBAL: TOY2 0x0052EF40
	int32_t g_demoInputRunLength;

	// GLOBAL: TOY2 0x0052B818
	int16_t g_isPaused;

	// GLOBAL: TOY2 0x00529E48
	int16_t g_pauseMenuBlinkTimer;

	// GLOBAL: TOY2 0x0052F2DC
	int16_t g_levelTransitionTimer;

	// GLOBAL: TOY2 0x00830D50
	int32_t g_quitToTitleFlag;

	// GLOBAL: TOY2 0x0052F300
	Buzz::Toy2BuzzActor g_buzzActor;

	// GLOBAL: TOY2 0x00529388
	int32_t g_wndIsExitingUnused;

	// GLOBAL: TOY2 0x0072EFC8
	int32_t g_clearScreenSaveResult;

	// GLOBAL: TOY2 0x00500AA0
	int32_t g_setScreenSaveRunning = 1;

	// GLOBAL: TOY2 0x00830C64
	int32_t g_extraControlsUnused;

	// GLOBAL: TOY2 0x00830C1C
	int32_t g_inputSuppressFrames;

	// GLOBAL: TOY2 0x00500A58
	SectorBackdropTexIdTable g_sectorBackdropTexTable = {
		{ 36, 40, 41, 42, 43, 44, 45, 46, 47, 32 },
		{ 88, 89, 90, 91, 92, 93, 94, 95 },
	};

	// GLOBAL: TOY2 0x00534550
	int32_t g_unused0;
}

namespace Toy2
{
	namespace Graphics
	{
		// FUNCTION: TOY2 0x004CDD90
		int32_t AddDetailLevel()
		{
			int32_t detail = g_toyCfgData.detail + 1;

			if ((g_toyCfgData.detail + 1) >= 2)
				detail = 2;

			g_toyCfgData.detail = detail;

			return detail;
		}

		// FUNCTION: TOY2 0x004CDDB0
		int32_t RemoveDetailLevel()
		{
			int32_t detail = (g_toyCfgData.detail - 1) <= 0 ? 0 : g_toyCfgData.detail - 1;
			g_toyCfgData.detail = detail;

			return detail;
		}
	}

	namespace Game
	{
		// STUB: TOY2 0x0049E330
		void PauseLoop() {}

		// STUB: TOY2 0x0049DFE0
		void MainLoop() {}
	}

	namespace PostGameRecap
	{
		// STUB: TOY2 0x004398B0
		void Tick() {}
	}

	namespace GameOver
	{
		// STUB: TOY2 0x00437B20
		void Tick() {}
	}

	// STUB: TOY2 0x00454020
	void ShowPostGameSaveMenu() {}

	// STUB: TOY2 0x00453D90
	void ShowActClearScreen() {}

	// STUB: TOY2 0x0049EB50
	int32_t ComputeTokenProgress() { return 1; }

	// STUB: TOY2 0x00414720
	int32_t EnterLevel(int32_t levelIndex) { return 0; }

	// STUB: TOY2 0x004A3770
	void LoadPathBin() {}

	// FUNCTION: TOY2 0x00453CF0
	int32_t ShowLevelSelect()
	{
		int32_t prevLevelFileIdx = Toy2::g_levelFileIndex;

		g_levelFileIndex = 16;
		Levels::g_levelLoadConfig = 56;

		g_hasStaticBackdrop = 0;
		Renderer::g_virtualScreenWidth = 512.0;
		Renderer::g_virtualScreenHeight = 256.0;
		g_nextBackdropId = 36;
		Levels::InitLevelPlay(16);

		Renderer::g_frameDelta = 2;
		g_hasBackdrop = 0;
		MainMenu::g_menuClearColor.b = 140;
		MainMenu::g_menuClearColor.g = 140;
		MainMenu::g_menuClearColor.r = 140;

		int32_t newState = LevelSelect::Tick();

		g_levelFileIndex = prevLevelFileIdx;

		MainMenu::g_menuClearColor.b = 32;
		MainMenu::g_menuClearColor.g = 32;
		MainMenu::g_menuClearColor.r = 32;

		return newState;
	}

	// STUB: TOY2 0x00453FA0
	void ShowMovieViewer() {}

	// STUB: TOY2 0x00453F20
	int32_t ShowSaveScreen() { return 0; }

	// STUB: TOY2 0x0049EB20
	void UnlockAndPlayMovie(int32_t movieId, int32_t backgroundId, int32_t forcePlay) {}

	// STUB: TOY2 0x0049AB90
	int32_t PlayMovieWithTransition(int32_t movieId, int32_t backgroundId) { return 1; }

	// FUNCTION: TOY2 0x0048F1B0
	void SetBackdropByIndex(int32_t index)
	{
		int32_t sectorIdx = index + 1;
		Renderer::g_parallaxCurHorizScroll = 0.0;

		if (index + 1 >= 0 && sectorIdx < 9)
		{
			if (NGNLoader::GetTextureDataIndex(g_sectorBackdropTexTable.primary[sectorIdx]))
			{
				g_nextBackdropId = g_sectorBackdropTexTable.primary[sectorIdx];
				g_hasBackdrop = 1;
			}
			else
			{
				uint32_t l_textureId = g_sectorBackdropTexTable.secondary[sectorIdx - 1];
				int32_t* l_id = &g_sectorBackdropTexTable.secondary[sectorIdx - 1];

				if (NGNLoader::GetTextureDataIndex(l_textureId))
					g_nextBackdropId = *l_id;

				g_hasBackdrop = 1;
			}
		}
	}

	// STUB: TOY2 0x00438520
	int32_t ShowStaticScreen(int32_t backdropIndex) { return 0; }

	// STUB: TOY2 0x0043A380
	int32_t ShowCredits() { return 0; }

	// FUNCTION: TOY2 0x004381F0
	int32_t ScreenDispatcher(int32_t index)
	{
		int32_t defaultFadeFramesRemaining;

		switch (index)
		{
			case 1: {
				int32_t caseOneFadeFrames = 160;
				InputManager::g_curButtonsPressed = 0;
				InputManager::g_prevButtonsPressed = 0;

				MainMenu::g_fadeTimer = 0;
				MainMenu::g_nextScreen = 0;

				Nu3D::Camera::g_cameraTintBlue = 0;
				Nu3D::Camera::g_cameraTintGreen = 0;
				Nu3D::Camera::g_cameraTintRed = 0;

				Nu3D::Camera::SetTint(128, 128, 128, 12);
				SoftwareRenderer::UnkFunc67(0, 0);
				Renderer::g_frameDelta = 1;

				SetBackdropByIndex(1);

				while (true)
				{
					Nu3D::Camera::FadeToTargetTint();
					caseOneFadeFrames -= Renderer::g_frameDelta;

					if (caseOneFadeFrames <= 0)
						break;

					if (caseOneFadeFrames <= 23)
					{
						if ((caseOneFadeFrames + Renderer::g_frameDelta) > 23)
							Nu3D::Camera::SetTint(0, 0, 0, 12);
					}

				LBL_CHECK_FADE_COMPLETE:

					if (! caseOneFadeFrames)
						return 1;
				}

				caseOneFadeFrames = 0;

				if ((caseOneFadeFrames + Renderer::g_frameDelta) > 23)
					Nu3D::Camera::SetTint(0, 0, 0, 12);

				goto LBL_CHECK_FADE_COMPLETE;
			}

			case 2:
				g_mainMenuState = MainMenu::Tick();
				return 1;

			case 4:
				PostGameRecap::Tick();
				return 1;

			case 5:
				GameOver::Tick();
				return 1;

			case 6: {
				if (! g_returnedToTitle && g_attractModeTimer >= 0)
				{
					int32_t caseSixFadeFrames = 2 * g_attractModeTimer;
					InputManager::g_curButtonsPressed = 0;
					InputManager::g_prevButtonsPressed = 0;

					MainMenu::g_fadeTimer = 0;
					MainMenu::g_nextScreen = 0;

					Nu3D::Camera::g_cameraTintBlue = 0;
					Nu3D::Camera::g_cameraTintGreen = 0;
					Nu3D::Camera::g_cameraTintRed = 0;

					Nu3D::Camera::SetTint(128, 128, 128, 12);
					SoftwareRenderer::UnkFunc67(0, 0);
					Renderer::g_frameDelta = 1;
					SetBackdropByIndex(0);

					int32_t skipInputThreshold = caseSixFadeFrames - 120;

					if (! caseSixFadeFrames)
						return 1;

					while (true)
					{
						Nu3D::Camera::FadeToTargetTint();

						if (caseSixFadeFrames > 0)
						{
							caseSixFadeFrames -= Renderer::g_frameDelta;

							if (caseSixFadeFrames <= 0)
								break;
						}

						if (caseSixFadeFrames)
						{
							if ((caseSixFadeFrames + Renderer::g_frameDelta) > 23)
								Nu3D::Camera::SetTint(0, 0, 0, 12);
						}

					LBL_CHECK_OR_COMPLETE:

						if ((InputManager::g_curButtonsPressed & (INPUT_CANCEL | INPUT_SPIN | INPUT_JUMP | INPUT_FIRE)) != 0
							&& caseSixFadeFrames < skipInputThreshold && caseSixFadeFrames > 23)
						{
							caseSixFadeFrames = 24;
						}
						else if (! caseSixFadeFrames)
						{
							return 1;
						}
					}

					caseSixFadeFrames = 0;

					if ((caseSixFadeFrames + Renderer::g_frameDelta) > 23)
						Nu3D::Camera::SetTint(0, 0, 0, 12);

					goto LBL_CHECK_OR_COMPLETE;
				}

				defaultFadeFramesRemaining = 600;

				InputManager::g_curButtonsPressed = 0;
				InputManager::g_prevButtonsPressed = 0;

				MainMenu::g_fadeTimer = 0;
				MainMenu::g_nextScreen = 0;

				Nu3D::Camera::g_cameraTintBlue = 0;
				Nu3D::Camera::g_cameraTintGreen = 0;
				Nu3D::Camera::g_cameraTintRed = 0;

				Nu3D::Camera::SetTint(128, 128, 128, 12);
				SoftwareRenderer::UnkFunc67(0, 0);
				Renderer::g_frameDelta = 1;

				SetBackdropByIndex(0);
				break;
			}

			case 8:
				g_mainMenuState = MainMenu::g_nextScreen - 1;
				return 1;

			case 9:
				MainMenu::ShowSettings();
				return 1;

			case 10:
				ShowStaticScreen(2);
				ShowStaticScreen(3);
				return 1;

			case 11:
				ShowCredits();
				return 1;

			default:
				return 1;
		}

		do
		{
			Nu3D::Camera::FadeToTargetTint();

			defaultFadeFramesRemaining -= Renderer::g_frameDelta;

			if (defaultFadeFramesRemaining > 0)
			{
				if (defaultFadeFramesRemaining > 23)
					continue;
			}
			else
			{
				defaultFadeFramesRemaining = 0;
			}

			if ((defaultFadeFramesRemaining + Renderer::g_frameDelta) > 23)
				Nu3D::Camera::SetTint(0, 0, 0, 12);

		} while (defaultFadeFramesRemaining);

		return 1;
	}

	// STUB: TOY2 0x0048E730
	void OneInit()
	{
		g_randDatBufferPtr = g_randDatBuffer;
		SaveManager::Init();
	}

	// STUB: TOY2 0x00490730
	void CheckForQuit() {}

	// FUNCTION: TOY2 0x004CE760
	void InitCfg()
	{
		memset(&g_toyCfgData, 0, sizeof(g_toyCfgData));

		g_toyCfgData.driverIndex = -1;
		g_toyCfgData.detail = 1;
		g_toyCfgData.flags |= 7;
		g_toyCfgData.gammaCorrection = 2.0;
	}

	// FUNCTION: TOY2 0x004CE810
	int32_t ReadCfg()
	{
		InitCfg();

		FILE* fileHandle = fopen("toy2.cfg", "rb");

		if (fileHandle)
		{
			fread(&g_toyCfgData, 1, sizeof(ToyCfg), fileHandle);
			fclose(fileHandle);
			return 1;
		}

		return 0;
	}

	// FUNCTION: TOY2 0x00412B50
	void RunModeSelect()
	{
		if (! g_modeSelectFinished)
		{
			atexit(DrawingDevice::Quit);
			ModeSelect::SetForceFullscreen_T(0);

			if (ModeSelect::EnumerateDrivers_T(ModeSelect::DeviceFilterCallback) < 0)
				Logger::GetErrorHandler("C:\\projects\\toy2\\direct6.cpp", 103)("Unable to enumerate a suitable device");

			ModeSelect::Show();

			DrawingDevice::DDAppDevice* primaryDevice;
			DrawingDevice::DDAppDevice::App* ddApp;

			if (DrawingDevice::GetChosenDevice_T(&ddApp, &primaryDevice))
				Logger::GetErrorHandler("C:\\projects\\toy2\\direct6.cpp", 111)("Unable to create D3D device\r\n try a lower resolution or screen depth");

			int32_t canDoWindowed = primaryDevice->canRenderWindowedOnPrimary;
			int32_t fullscreenExclusive = (ModeSelect::g_unusedFlag1 != 0 ? 2 : 0) | (canDoWindowed == 0) | (ModeSelect::g_unusedFlag2 != 0 ? 4 : 0);

			if (! canDoWindowed)
				Logger::g_showMsgBoxOnThrow = 1;

			if (primaryDevice->isHardwareAccelerated)
			{
				Renderer::SetIsSoftwareRendering(0);
			}
			else
			{
				Renderer::SetIsSoftwareRendering(1);
				while (Toy2::Graphics::RemoveDetailLevel()) {};
			}

			if (! primaryDevice->isHardwareAccelerated && primaryDevice->canRenderWindowedOnPrimary)
			{
				RECT adjustedRect;
				adjustedRect.top = 0;
				adjustedRect.left = 0;
				adjustedRect.right = 320;
				adjustedRect.bottom = 240;

				AdjustWindowRect(&adjustedRect, 0, 0);
				SetWindowPos(D3DApp::g_windowData.mainHwnd, 0, 0, 0, adjustedRect.right, adjustedRect.bottom, 2);
			}

			ShowWindow(D3DApp::g_windowData.mainHwnd, SW_SHOWMAXIMIZED);

			if (DrawingDevice::CD3DFramework::Build(
					D3DApp::g_windowData.mainHwnd, &ddApp->guid, primaryDevice, primaryDevice->primaryDisplayMode, fullscreenExclusive)
				>= 0)
				g_modeSelectFinished = 1;
		}
	}

	// FUNCTION: TOY2 0x0047D8D0
	void UnusedInit()
	{
		// GLOBAL: TOY2 0x00725F20
		static int32_t g_unusedInit;

		g_unusedInit = 2;
	}

	// FUNCTION: TOY2 0x00412D70
	int32_t ShowModeSelect()
	{
		char cdFileName[8];
		char cdTrackBuffer[1024];

		strcpy(cdFileName, "cd.txt");
		memset(cdTrackBuffer, 0, sizeof(cdTrackBuffer));

		int32_t baseCDTrack;

		if (FileUtils::LoadFile(cdFileName, cdTrackBuffer))
			baseCDTrack = atoi(cdTrackBuffer);
		else
			baseCDTrack = 2;

		g_cdBaseTrack = baseCDTrack;
		Logger::Log("CONFIG : Base CD track is %d.\n", baseCDTrack);

		AudioManager::Init();
		UnusedInit();
		Numerics::InitTrigLut();

		RunModeSelect();

		InputManager::Init();
		Renderer::Init();
		Nu3D::Viewport::Init();

		RECT* destRect = DrawingDevice::GetDestRect();

		Nu3D::Font::Init();
		Nu3D::Font* font = Nu3D::Font::Build("ariel", 20, 0);

		if (font)
		{
			Nu3D::Font::SetFont(font);
			Nu3D::Font::SetTextColor(0xFFFFFFFF);
			Nu3D::Font::SetTextClipRect(destRect->left, destRect->top, destRect->right - 1, destRect->bottom - 1);
		}

		while (ShowCursor(FALSE) >= 0) {};

		return 1;
	}

	// FUNCTION: TOY2 0x0049D910
	int32_t Run(int32_t argCount, char** argList)
	{
		g_returnedToTitle = 0;

		g_unused1 = 2;
		g_unused2 = 0;

		g_attractModeTimer = -25;
		g_attractModeInputTimer = -50;

		SoftwareRenderer::SwapRenderBuffer();

		g_pastInitialBoot = 0;
		g_curDemoLevel = 0;
		g_levelTransition = 0;

		SaveManager::g_save0Data.unkData1 = 1;
		SaveManager::g_save0Data.unkData2 = 23;
		SaveManager::g_save0Data.cameraType = SaveManager::CAMERA_PASSIVE | SaveManager::CAMERA_ACTIVE;
		SaveManager::g_save0Data.musicVolume = 8;
		SaveManager::g_save0Data.soundVolume = 8;

		AudioManager::SetVolumesProcessed(8, 8);

		int32_t enteredLevelIdx;
		int32_t levelIdxCache = g_levelFileIndex;

	LBL_RESTART_GAME:

		g_demoMode = 0;
		g_mainMenuState = 0;
		g_levelFileIndex = 0;

		Levels::g_levelLoadConfig = 1084;

		Renderer::SetVirtualRatioTo54();
		Levels::InitLevelPlay(0);
		ScreenDispatcher(10);

		g_levelFileIndex = levelIdxCache;

		if (! PlayMovieWithTransition(2, 0) && ! PlayMovieWithTransition(0, 0) && ! PlayMovieWithTransition(1, 0))
			UnlockAndPlayMovie(0, 0, 1);

		g_pastInitialBoot = 1;

	LBL_RESTART_MENU_STATE:

		SaveManager::InitProgressData(&SaveManager::g_save0Data);
		SaveManager::LoadProgressData(&SaveManager::g_save0Data);

		g_mainMenuState = 0;
		g_saveLoaded = 0;

		LevelSelect::ResetCursor();

	LBL_REDO_MENU_LOOP:

		while (true)
		{
			int32_t savedLevelFileIndex = g_levelFileIndex;
			g_levelFileIndex = 0;
			Levels::g_levelLoadConfig = 1084;

			Renderer::SetVirtualRatioTo54();
			Levels::InitLevelPlay(0);
			ScreenDispatcher(2);

			g_levelFileIndex = savedLevelFileIndex;

			switch (g_mainMenuState)
			{
				case 0:
					g_demoMode = 1;
					goto LBL_DEMO_MODE;

				case 1:
					g_demoMode = 0;
					goto LBL_DEMO_MODE;

				case 2: // Options Screen
					g_levelFileIndex = 0;
					Levels::g_levelLoadConfig = 1212;

					Renderer::SetVirtualRatioTo54();
					Levels::InitLevelPlay(0);
					ScreenDispatcher(9);

					g_levelFileIndex = enteredLevelIdx;
					g_mainMenuState = -1;
					continue;

				case 3: // Save Screen
					if (ShowSaveScreen())
						g_saveLoaded = 1;

					g_mainMenuState = -1;
					continue;

				case 4: // Movie Viewer
					ShowMovieViewer();
					g_mainMenuState = -1;
					continue;

				case 8:
					g_levelFileIndex = 0;
					Levels::g_levelLoadConfig = 1084;

					Renderer::SetVirtualRatioTo54();
					Levels::InitLevelPlay(0);
					ScreenDispatcher(8);

					g_levelFileIndex = enteredLevelIdx;
					g_mainMenuState = 0;
					goto LBL_RESTART_MENU_STATE;

				case 9:
					Nullsub5();
					return 0;

				default:

				LBL_DEMO_MODE:
					g_mainMenuState = 0;

					if (g_demoMode == 1)
					{
						switch (g_curDemoLevel)
						{
							case 0:
								g_levelIndex = 0;
								break;
							case 1:
								g_levelIndex = 3;
								break;
							case 2:
								g_levelIndex = 7;
								break;
							case 3:
								g_levelIndex = 10;
								break;
							case 4:
								g_levelIndex = 13;
								break;
							default:
								break;
						}

						if (++g_curDemoLevel > 4)
							g_curDemoLevel = 0;

						g_levelFileIndex = g_levelFileConversion[g_levelIndex];

						LoadPathBin();

						int32_t levelIdxCache = g_levelIndex;

						g_demoPathWriteIdx = -2;
						g_demoInputRunLength = 0;

						SaveManager::InitProgressData(&SaveManager::g_save0Data);
						SaveManager::LoadProgressData(&SaveManager::g_save0Data);

						g_levelIndex = levelIdxCache;
					}

					if (! g_demoMode && g_attractModeTimer < 0)
						goto LBL_SHOW_LEVEL_SELECT;

					break;
			}

			break;
		}

		while (true)
		{
			g_levelFileIndex = g_levelFileConversion[g_levelIndex];

			if (EnterLevel(g_levelFileIndex))
				break;

			while (true)
			{
				g_attractModeInputTimer = 2 * g_attractModeTimer;

				InputManager::g_curButtonsPressed = 0;
				InputManager::g_prevButtonsPressed = 0;

				g_isPaused = 0;
				g_pauseMenuBlinkTimer = 0;

				InputManager::g_directionInputState = 0;
				InputManager::g_prevDirectionInputState = 0;
				InputManager::g_directionInputState2Frames = 0;
				InputManager::g_directionInputState3Frames = 0;

				Renderer::g_frameDelta = 1;
				g_levelTransition = 0;
				g_levelTransitionTimer = 90;

				Nu3D::Camera::SetTint(128, 128, 128, 6);

				while (! g_levelTransition || g_levelTransitionTimer)
				{
					if (g_demoMode)
						Renderer::g_frameDelta = 2; // Half frame rate in demo mode

					if (g_isPaused)
						Game::PauseLoop();
					else
						Game::MainLoop();
				}

				AudioManager::FlushSoundVoices();

				if (g_levelTransition != 2)
					AudioManager::StopAndWait();

				if (g_quitToTitleFlag)
				{
					if (g_levelTransition == 2)
						AudioManager::StopAndWait();

					goto LBL_RESTART_MENU_STATE;
				}

				if (g_levelTransition != 2)
					break;

				if (! g_buzzActor.lives)
				{
					AudioManager::StopAndWait();
					goto LBL_GAME_OVER;
				}

				Buzz::Respawn();
			}

			if (g_levelTransition == 3 || g_levelTransition == 4)
			{
				if (g_attractModeTimer >= 0)
					break;

				if (g_levelTransition != 3)
					goto LBL_RESTART_MENU_STATE;

				levelIdxCache = g_levelFileIndex;

				goto LBL_RESTART_GAME;
			}

			enteredLevelIdx = g_levelFileIndex;

			if (g_levelTransition != 5)
			{
				if (g_levelTransition != 1)
				{
					if (g_attractModeTimer >= 0)
						break;

					goto LBL_SHOW_LEVEL_SELECT;
				}

				goto LBL_SHOW_LEVEL_RESULTS;
			}

			if (g_buzzActor.health < 0)
			{
				if (! g_buzzActor.lives)
				{
				LBL_GAME_OVER:

					levelIdxCache = g_levelFileIndex;

					g_levelFileIndex = 0;
					Levels::g_levelLoadConfig = 1148;

					Renderer::SetVirtualRatioTo54();

					Levels::InitLevelPlay(0);
					ScreenDispatcher(5);

					g_levelFileIndex = levelIdxCache;

					if (g_attractModeTimer >= 0)
						break;

					goto LBL_RESTART_GAME;
				}

				--g_buzzActor.lives;
			}

			if (g_attractModeTimer >= 0)
				break;

			if (g_levelFileIndex % 3)
			{
				g_levelTransition = 1;

			LBL_SHOW_LEVEL_RESULTS:

				if (g_levelFileIndex % 3)
				{
					g_levelFileIndex = 0;
					Levels::g_levelLoadConfig = 1148;

					Renderer::SetVirtualRatioTo54();
					Levels::InitLevelPlay(0);
					ScreenDispatcher(4);
					g_levelFileIndex = enteredLevelIdx;

					if (g_attractModeTimer >= 0)
						break;

					int32_t movie17Status = SaveManager::g_save0Data.moviesUnlocked[17];

					if (SaveManager::g_curLevelTokenData != SaveManager::g_save0Data.tokens[g_levelFileConversion[g_levelIndex]]
						&& (ComputeTokenProgress() & 0xFF0000) == 0x320000)
					{
						SaveManager::g_save0Data.moviesUnlocked[17] = 1;
					}

					ShowPostGameSaveMenu();

					if (! movie17Status && SaveManager::g_save0Data.moviesUnlocked[17])
						UnlockAndPlayMovie(17, 16, 1);
				}
				else
				{
					ShowActClearScreen();

					int32_t levelMovieStatus = SaveManager::g_save0Data.moviesUnlocked[g_levelIndex + 1];

					SaveManager::g_save0Data.moviesUnlocked[g_levelIndex + 1] = 1;

					if (g_levelFileIndex == 15)
						SaveManager::g_save0Data.moviesUnlocked[18] = 1;

					ShowPostGameSaveMenu();

					if (! levelMovieStatus)
						UnlockAndPlayMovie(g_levelIndex + 1, 30, 1);

					if (g_levelFileIndex == 15)
					{
						UnlockAndPlayMovie(18, 31, 1);
						levelIdxCache = g_levelFileIndex;

						g_levelFileIndex = 0;
						Levels::g_levelLoadConfig = 1276;

						Renderer::SetVirtualRatioTo54();
						Levels::InitLevelPlay(0);
						ScreenDispatcher(11); // Show credits

						goto LBL_RESTART_GAME;
					}
				}
			}

			if (g_attractModeTimer >= 0)
				break;

		LBL_SHOW_LEVEL_SELECT:

			g_mainMenuState = 0;
			g_saveLoaded = 1;

			if (ShowLevelSelect())
			{
				g_mainMenuState = -1;
				goto LBL_REDO_MENU_LOOP;
			}

			if ((g_levelIndex + 1) % 3)
			{
				UnlockAndPlayMovie(g_levelIndex + 1, 0, 0);
			}
			else if (g_levelIndex == 11)
			{
				UnlockAndPlayMovie(16, 0, 0);
			}
		}

		Nullsub5();

		return 0;
	}

	// STUB: TOY2 0x00412E80
	int32_t CleanupManagers() { return 0; }

	// STUB: TOY2 0x0047D7C0
	void UpdateAudioChannels() {}

	// STUB: TOY2 0x00490BF0
	int16_t UpdateD3DState() { return 0; }

	// FUNCTION: TOY2 0x004909E0
	void ProcessMiscEventsEx()
	{
		Nu3D::Font::SetTextCursor(0, Nu3D::g_scaledFontAscent);
		DevDraw::g_vertexCount = 0;

		D3DApp::g_windowData.wndIsExiting = g_wndIsExitingUnused;

		if (g_wndIsExitingUnused)
		{
			Logger::Log("CheckForQuit : Starting shutdown now...\n");

			DestroyWindow(D3DApp::g_windowData.mainHwnd);

			if (D3DApp::g_renderMode == RENDERMODE_SOFTWARE)
			{
				SoftwareRenderer::Destroy();
			}
			else if (D3DApp::g_renderMode == RENDERMODE_D3D)
			{
				Logger::Log("QUIT : Destroying Direct3D renderer.\n");
			}

			CleanupManagers();
			D3DApp::PostQuitMessage();

			CoUninitialize();

			D3DApp::g_windowData.mainHwnd = 0;

			Logger::Log("CheckForQuit : Code shutdown.\n");

			g_clearScreenSaveResult = SystemParametersInfoA(SPI_SCREENSAVERRUNNING, 0, &g_setScreenSaveRunning, 0);

			if (g_clearScreenSaveResult)
				Logger::Log("Managed to clear SCREENSAVERRUNNING\n");
			else
				Logger::Log("Failed to clear SCREENSAVERRUNNING\n");

			exit(D3DApp::g_windowData.wndEventMsg.wParam);
		}

		D3DApp::ProcessWndEvents();

		if (D3DApp::g_windowData.wndIsExiting)
		{
			DrawingDevice::g_drawingDevice->RestoreToGDISurface(1);
			Logger::GetErrorHandler("C:\\projects\\toy2\\toy2.cpp", 4670)("");
		}

		if (InputManager::IsKeyPressed(DIK_F3))
		{
			SoftwareRenderer::ZoomOut();
			g_extraControlsUnused = 15;
		}

		if (InputManager::IsKeyPressed(DIK_F4))
		{
			SoftwareRenderer::ZoomIn();
			g_extraControlsUnused = 15;
		}

		if (InputManager::IsKeyPressed(DIK_F5))
		{
			Graphics::RemoveDetailLevel();
			g_extraControlsUnused = 15;
		}

		if (InputManager::IsKeyPressed(DIK_F6))
		{
			Graphics::AddDetailLevel();
			g_extraControlsUnused = 15;
		}

		InputManager::UpdateButtonStates();

		if (g_inputSuppressFrames)
		{
			InputManager::g_curButtonsPressed = 0;
			--g_inputSuppressFrames;
		}

		UpdateAudioChannels();

		SoftwareRenderer::g_unk830C60 = 0;

		if (D3DApp::g_renderMode == RENDERMODE_SOFTWARE)
		{
			SoftwareRenderer::UnkFunc2();
			SoftwareRenderer::UnkFunc3();
		}
		else if (D3DApp::g_renderMode == RENDERMODE_D3D)
		{
			UpdateD3DState();
		}
	}

	// STUB: TOY2 0x0047CC90
	void InitSoftwareRenderer()
	{
		// Some back story on this, the game has two rendering modes. Hardware accelerated (DirectX) or full software based rendering.
		// This is normal for the time, considering it was uncommon for people to have dedicated graphics hardware as it was expensive.
		//
		// The software renderer will be decompiled last, as it is extremely math heavy and optimized, making it difficult to produce
		// code that can be read by human eyes.
		return;
	}

	// STUB: TOY2 0x00499950
	void InitDirect3DRenderer()
	{
		// Weird method, a good portion of these variables are never even used in the game
	}
}

// $FUNC DEBUG
void AllocateConsole()
{
	AllocConsole();

	FILE* fp;

	// redirect STDOUT
	fp = freopen("CONOUT$", "w", stdout);
	fp = freopen("CONOUT$", "w", stderr);

	// redirect STDIN
	fp = freopen("CONIN$", "r", stdin);

	printf("[Debug Console Allocated!]\n");
}

// FUNCTION: TOY2 0x004316C0
int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, char* cmdLine, int32_t cmdShow)
{
#ifdef APPLY_FIXES
	AllocateConsole();
#endif

	Toy2::g_unused0 = 0;

	memset(&D3DApp::g_d3dAppI, 0, sizeof(D3DApp::g_d3dAppI));

	D3DApp::g_no32bitColors = 1;

	FileUtils::ValidateInstall();

	Toy2::g_levelFileIndex = 1;

	memset(&D3DApp::g_windowData, 0, sizeof(D3DApp::g_windowData));

	D3DApp::g_windowData.hInstance = hInstance;
	D3DApp::g_windowData.hPrev = hPrev;
	D3DApp::g_windowData.lpCmdLine = cmdLine;
	D3DApp::g_windowData.nShowCmd = cmdShow;

	D3DApp::g_windowData.unkInt8 = 0;
	D3DApp::g_windowData.unkInt3 = 1;
	D3DApp::g_windowData.unkInt4 = 1;
	D3DApp::g_windowData.unkInt5 = 1;
	D3DApp::g_windowData.wndIsExiting = 0;

	Toy2::OneInit();

	Toy2::g_returnedToTitle = 0;
	Toy2::g_attractModeTimer = -1;

	Toy2::g_unused1 = 2;
	Toy2::g_unused2 = 0;

	Toy2::ReadCfg();
	D3DApp::BuildProfileMachine();
	D3DApp::BuildWindow();
	Toy2::ShowModeSelect();

	switch (D3DApp::g_renderMode)
	{
		case RENDERMODE_SOFTWARE:
			Toy2::InitSoftwareRenderer();
			break;
		case RENDERMODE_D3D:
			Toy2::InitDirect3DRenderer();
			break;
	}

	int32_t tokenCount = 0;
	char* currentToken;
	char* tokenEntries[8];

	currentToken = strtok(cmdLine, " ");

	if (currentToken)
	{
		tokenCount = 1;
		char* nextToken = strtok(0, " ");

		if (nextToken)
		{
			char** tokenArrayPtr = tokenEntries;

			do
			{
				*tokenArrayPtr = nextToken;
				++tokenCount;
				++tokenArrayPtr;

				nextToken = strtok(0, " ");
			} while (nextToken);
		}
	}

	Toy2::Run(tokenCount, &currentToken);

	D3DApp::g_windowData.wndIsExiting = 1;

	Toy2::CheckForQuit();

	return 0;
}