#pragma once

#include "Common.h"

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