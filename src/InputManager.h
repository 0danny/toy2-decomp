#pragma once

#include "Common.h"

enum GameControlId
{
	INPUT_SECRET_MENU = 0x1,
	INPUT_MENU = 0x8,
	INPUT_UP = 0x10,
	INPUT_RIGHT = 0x20,
	INPUT_DOWN = 0x40,
	INPUT_LEFT = 0x80,
	INPUT_CAMERA_LEFT = 0x100,
	INPUT_CAMERA_RIGHT = 0x200,
	INPUT_VISOR_TOGGLE = 0x400,
	INPUT_TARGET_LOCK = 0x800,
	INPUT_CANCEL = 0x1000,
	INPUT_SPIN = 0x2000,
	INPUT_JUMP = 0x4000,
	INPUT_FIRE = 0x8000,
};

namespace InputManager
{
	extern int16_t g_curButtonsPressed;
	extern int16_t g_prevButtonsPressed;
	extern int16_t g_directionInputState;
	extern int16_t g_prevDirectionInputState;
	extern int16_t g_directionInputState2Frames;
	extern int16_t g_directionInputState3Frames;

	void Init();
}