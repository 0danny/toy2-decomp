#include <windows.h>
#include <minhook.h>
#include <imgui.h>
#include <iostream>
#include <fstream>

#include "GameInterface.hpp"
#include "ImGuiWrapper.hpp"
#include "NoClip.hpp"
#include "FreeCam.hpp"

namespace
{
	static bool freezePos[64] = {};
	static Vector3I frozenPos[64] = {};
}

namespace Loader
{
	void OpenConsole()
	{
		AllocConsole();

		FILE* stream = nullptr;

		freopen_s(&stream, "CONOUT$", "w", stdout);
		freopen_s(&stream, "CONOUT$", "w", stderr);
		freopen_s(&stream, "CONIN$", "r", stdin);

		printf("[LOADER]: Dll loaded.\n");

		SetConsoleTitleA("ToyDebugger");
	}

	void InitMinhook()
	{
		if ( MH_Initialize() != MH_OK )
		{
			printf("[LOADER]: Failed to initialize MinHook.\n");
			return;
		}

		ImGuiWrapper::Init();
		FreeCam::Init();

		MH_EnableHook(MH_ALL_HOOKS);
	}

	void TeleportActors()
	{
		// Tp every single actor on the map to buzz's position in a 5 column grid
		const int32_t kSpacing = 18000;
		const int32_t kCols = 5;

		for ( int32_t idx = 0; idx < 40; idx++ )
		{
			Toy2Actor* curActor = &g_levelActors[idx];

			int32_t col = idx % kCols;
			int32_t row = idx / kCols;

			int32_t spawnX = g_buzzActor->pos.x + (col - 2) * kSpacing;
			int32_t spawnZ = g_buzzActor->pos.z + row * kSpacing;

			curActor->boundary.x = spawnX;
			curActor->boundary.y = g_buzzActor->pos.y;
			curActor->boundary.z = spawnZ;
			curActor->motionTargetPos.x = spawnX;
			curActor->motionTargetPos.y = g_buzzActor->pos.y;
			curActor->motionTargetPos.z = spawnZ;
			curActor->creatureRam->boundHalfX = 1;
			curActor->creatureRam->boundHalfZ = 1;
			curActor->actorFlags = 1;

			curActor->pos.x = spawnX;
			curActor->pos.y = g_buzzActor->pos.y;
			curActor->pos.z = spawnZ;
			curActor->visibilityDistance = 30000;
		}
	}

	void RenderGui()
	{
		NoClip::UpdateBuzzNoclip();

		ImGui::SetNextWindowSize(ImVec2(500.0f, 800.0f), ImGuiCond_FirstUseEver);

		if ( ImGui::Begin("ToyDebugger") )
		{
			ImGui::Text("ToyDebugger");
			ImGui::Separator();

			ImGui::Spacing();

			if ( ImGui::Button("Teleport Actors") )
				TeleportActors();

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::Text("Buzz");
			ImGui::Spacing();

			bool oldNoclip = NoClip::g_buzzNoclipEnabled;

			if ( ImGui::Checkbox("Noclip", &NoClip::g_buzzNoclipEnabled) )
				NoClip::SetBuzzNoclipEnabled(NoClip::g_buzzNoclipEnabled);

			ImGui::Checkbox("FreeCam", &FreeCam::g_enabled);

			// Actor Dropdowns
			ImGui::Text("Actors");
			ImGui::Spacing();

			static int32_t selectedActor[kMaxActors] = {};

			for ( int32_t idx = 0; idx < kMaxActors; idx++ )
			{
				ImGui::PushID(idx);

				std::string headerLabel = std::format("Actor {}", (idx + 1));

				Toy2Actor* curActor = &g_levelActors[idx];

				if ( ImGui::CollapsingHeader(headerLabel.c_str()) )
				{
					ImGui::Text("Actor Data");

					ImGui::InputInt("Pos X", &curActor->pos.x);
					ImGui::InputInt("Pos Y", &curActor->pos.y);
					ImGui::InputInt("Pos Z", &curActor->pos.z);

					if ( ImGui::Checkbox("Freeze Position", &freezePos[idx]) )
					{
						if ( freezePos[idx] )
							frozenPos[idx] = curActor->pos;
					}

					if ( freezePos[idx] )
						curActor->pos = frozenPos[idx];

					if ( ImGui::Button("Teleport To Player") )
						curActor->pos = g_buzzActor->pos;
				}

				ImGui::PopID();
			}
		}

		ImGui::End();
	}

	DWORD WINAPI InjectHooks(LPVOID lpParam)
	{
		OpenConsole();
		InitMinhook();

		ImGuiWrapper::SubscribeRenderCallback(std::bind(&RenderGui));

		// Gotta keep the loop alive
		while ( true )
			Sleep(16);
	}

	void Cleanup()
	{
		printf("[LOADER]: Cleaning up...\n");

		ImGuiWrapper::Cleanup();

		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls(hModule);

		static HANDLE g_thread = 0;
		static DWORD g_threadId = 0;

		g_thread = CreateThread(NULL, 0, Loader::InjectHooks, NULL, 0, &g_threadId);
	}
	else if ( ul_reason_for_call == DLL_PROCESS_DETACH )
	{
		Loader::Cleanup();
	}

	return TRUE;
}
