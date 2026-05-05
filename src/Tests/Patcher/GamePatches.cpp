#include "GamePatches.h"

#include <cstdarg>
#include <cstdio>

namespace
{
	Toy2BuzzActor* g_buzzActor = reinterpret_cast<Toy2BuzzActor*>(0x0052F300);
	Toy2Actor* g_levelActors = reinterpret_cast<Toy2Actor*>(0x0052C840);

	const int32_t kMaxActors = 64;
	const int32_t kLevel1ActorCount = 36;
}

namespace GamePatches
{
	void InjectHooks()
	{
		printf("[GAME_PATCHES]: Injecting hooks...\n");

		if ( MH_CreateHook(Nu3DLogAddress, (LPVOID)&Nu3DLogHook, NULL) != MH_OK )
		{
			printf("[CONSTRUCTOR HOOK]: Could not hook Nu3DLogHook constructor.\n");
		}

		PollInput();
	}

	bool WasKeyPressed(int32_t key)
	{
		return GetAsyncKeyState(key) & 0x0001;
	}

	void PollInput()
	{
		while ( true )
		{
			if ( WasKeyPressed('O') )
			{
				// Tp every single actor on the map to buzz's position in a 5 column grid
				const int32_t kSpacing = 18000;
				const int32_t kCols = 5;

				for ( int32_t idx = 0; idx < kLevel1ActorCount; idx++ )
				{
					Toy2Actor* curActor = &g_levelActors[idx];

					int32_t col = idx % kCols;
					int32_t row = idx / kCols;

					int32_t spawnX = g_buzzActor->base.pos.x + (col - 2) * kSpacing;
					int32_t spawnZ = g_buzzActor->base.pos.z + row * kSpacing;

					curActor->base.boundary.x = spawnX;
					curActor->base.boundary.y = g_buzzActor->base.pos.y;
					curActor->base.boundary.z = spawnZ;
					curActor->base.motionTargetPos.x = spawnX;
					curActor->base.motionTargetPos.y = g_buzzActor->base.pos.y;
					curActor->base.motionTargetPos.z = spawnZ;
					curActor->creatureRam->boundHalfX = 1;
					curActor->creatureRam->boundHalfZ = 1;
					curActor->base.actorFlags = 1;

					curActor->base.pos.x = spawnX;
					curActor->base.pos.y = g_buzzActor->base.pos.y;
					curActor->base.pos.z = spawnZ;
					curActor->base.visibilityDistance = 30000;
				}
			}

			Sleep(16); //ms
		}
	}

	void __cdecl Nu3DLogHook(char* p_format, ...)
	{
		va_list args;
		va_start(args, p_format);
		vprintf(p_format, args);
		va_end(args);
	}
}
