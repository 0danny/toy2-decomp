#pragma once

#include "Common.h"
#include "Numerics.h"

namespace Toy2
{
	namespace Levels
	{
		struct RecordData
		{
			uint16_t recordCount;
			uint16_t recordType;
			Vector3I data[];
		};

		extern RecordData* g_recordData[96];
		extern uint8_t g_levelDataHeapBase[1249280];
		extern uint8_t* g_levelDataHeapBasePtr;
		extern int32_t g_levelLoadConfig;
		extern uint8_t* g_levelLoadArena;

		void InitLevelPlay(int32_t levelId);

		STATIC_ASSERT(sizeof(RecordData) == 0x4);
	}
}