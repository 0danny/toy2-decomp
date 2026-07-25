#include "Toy2/LevelSelect.h"
#include "Toy2/Toy2.h"
#include "Toy2/Levels.h"
#include "Renderer/Renderer.h"
#include "Renderer/Sprite.h"
#include "InputManager.h"
#include "Random.h"
#include "Nu3D/Link.h"
#include "Nu3D/Camera.h"
#include "Nullsub.h"
#include <MATH.H>

namespace Toy2
{
	namespace LevelSelect
	{
		// GLOBAL: TOY2 0x0055A0E4
		int32_t g_levelSelectCursor;

		// TODO: Label these
		int16_t g_hideLinks1[6] = { 1, 2, 3, 4, -1, 0 };

		int16_t g_showLinks1[6] = { 100, 101, 102, 103, 104, -1 };

		int16_t g_hideLinks2[6] = { 5, 6, 7, 8, -1, 0 };

		int16_t g_showLinks2[6] = { 105, 106, 107, 108, -1, 0 };

		int16_t g_hideLinks3[6] = { 9, 10, 11, 12, 0, -1 };

		int16_t g_showLinks3[6] = { 109, 110, 111, 112, -1, 0 };

		int16_t g_hideLinks4[18] = { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, -1 };

		int16_t g_showLinks4[18] = { 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, -1 };

		int16_t g_hideLinks5[14] = { 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, -1, 0 };

		int16_t g_showLinks5[14] = { 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, -1, 0 };

		int16_t g_hideLinks6[8] = { 43, 44, 45, 46, 47, 131, 132, -1 };

		int16_t g_showLinks6[8] = { 143, 144, 145, 146, 147, 231, 232, -1 };

		int16_t g_hideLinks7[6] = { 48, 49, 50, 51, -1, 0 };

		int16_t g_showLinks7[6] = { 148, 149, 150, 151, -1, 0 };

		int16_t g_hideLinks8[6] = { 52, 53, 54, 55, -1, 0 };

		int16_t g_showLinks8[6] = { 152, 153, 154, 155, -1, 0 };

		int16_t g_hideLinks9[4] = { 56, 57, 153, -1 };

		int16_t g_showLinks9[4] = { 156, 157, 253, -1 };

		int16_t g_hideLinks10[6] = { 58, 59, 60, 61, -1, 0 };

		int16_t g_showLinks10[6] = { 158, 159, 160, 161, -1, 0 };

		int16_t g_hideLinks11[4] = { 62, 63, 158, -1 };

		int16_t g_showLinks11[4] = { 162, 163, 258, -1 };

		int16_t g_hideLinks12[4] = { 64, 65, 258, -1 };

		int16_t g_showLinks12[4] = { 164, 165, 358, -1 };

		int16_t g_hideLinks13[6] = { 66, 67, 68, 69, 70, -1 };

		int16_t g_showLinks13[6] = { 166, 167, 168, 169, 170, -1 };

		int16_t g_hideLinks14[4] = { 71, 166, -1, 0 };

		int16_t g_showLinks14[4] = { 171, 266, -1, 0 };

		int16_t g_hideLinks15[6] = { 72, 73, 74, 266, -1, 0 };

		int16_t g_showLinks15[6] = { 172, 173, 174, 366, -1, 0 };

		// GLOBAL: TOY2 0x004F6DC4
		LevelLinkPair g_levelSelectLinks[15] = {
			{ g_hideLinks1, g_showLinks1 },
			{ g_hideLinks2, g_showLinks2 },
			{ g_hideLinks3, g_showLinks3 },
			{ g_hideLinks4, g_showLinks4 },
			{ g_hideLinks5, g_showLinks5 },
			{ g_hideLinks6, g_showLinks6 },
			{ g_hideLinks7, g_showLinks7 },
			{ g_hideLinks8, g_showLinks8 },
			{ g_hideLinks9, g_showLinks9 },
			{ g_hideLinks10, g_showLinks10 },
			{ g_hideLinks11, g_showLinks11 },
			{ g_hideLinks12, g_showLinks12 },
			{ g_hideLinks13, g_showLinks13 },
			{ g_hideLinks14, g_showLinks14 },
			{ g_hideLinks15, g_showLinks15 },
		};

		// FUNCTION: TOY2 0x00452170
		void ResetCursor() { g_levelSelectCursor = 0; }

		// STUB: TOY2 0x00438A50
		int32_t Tick() { return 0; }

	}
}
