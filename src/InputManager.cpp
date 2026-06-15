#include "InputManager.h"

namespace InputManager
{
	// $GLOBAL 0088279C
	int16_t g_curButtonsPressed;

	// $GLOBAL 00882794
	int16_t g_prevButtonsPressed;

	// $GLOBAL 0052AD88
	int16_t g_directionInputState;

	// $GLOBAL 0052F2FE
	int16_t g_prevDirectionInputState;

	// $GLOBAL 0053C630
	int16_t g_directionInputState2Frames;

	// $GLOBAL 0053C632
	int16_t g_directionInputState3Frames;

	// $FUNC 004152E0 [UNFINISHED]
	void Init() {}
}