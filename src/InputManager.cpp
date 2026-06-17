#include "InputManager.h"

namespace InputManager
{
	// GLOBAL: TOY2 0x0088279C
	int16_t g_curButtonsPressed;

	// GLOBAL: TOY2 0x00882794
	int16_t g_prevButtonsPressed;

	// GLOBAL: TOY2 0x0052AD88
	int16_t g_directionInputState;

	// GLOBAL: TOY2 0x0052F2FE
	int16_t g_prevDirectionInputState;

	// GLOBAL: TOY2 0x0053C630
	int16_t g_directionInputState2Frames;

	// GLOBAL: TOY2 0x0053C632
	int16_t g_directionInputState3Frames;

	// STUB: TOY2 0x004152E0
	void Init() {}
}