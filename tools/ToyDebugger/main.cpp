#include <windows.h>
#include <minhook.h>
#include <imgui.h>
#include <iostream>
#include <fstream>

#include "ImGuiWrapper.hpp"
#include "NoClip.hpp"
#include "FreeCam.hpp"
#include "GameInterface.hpp"

namespace
{
	static int32_t g_selectedActor = 0;
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

	void DrawActorPanel()
	{
		auto curActor = &g_levelActors[g_selectedActor];

		ImGui::SeparatorText("Position & Orientation");
		{
			ImGui::PushItemWidth(120.0f);

			ImGui::InputInt("Pos X", &curActor->pos.x);
			ImGui::InputInt("Pos Y", &curActor->pos.y);
			ImGui::InputInt("Pos Z", &curActor->pos.z);

			ImGui::Spacing();

			int32_t pitch = curActor->pitchAngle, yaw = curActor->yawAngle, roll = curActor->rollAngle;

			if ( ImGui::InputInt("Pitch", &pitch) )
				curActor->pitchAngle = static_cast<int16_t>(pitch);

			if ( ImGui::InputInt("Yaw", &yaw) )
				curActor->yawAngle = static_cast<int16_t>(yaw);

			if ( ImGui::InputInt("Roll", &roll) )
				curActor->rollAngle = static_cast<int16_t>(roll);

			ImGui::PopItemWidth();
		}

		ImGui::Spacing();
		ImGui::SeparatorText("Animation & Identity");
		{
			ImGui::PushItemWidth(120.0f);

			int32_t primaryAnim = curActor->primaryAnimIdx, secondaryAnim = curActor->secondaryAnimIdx;
			int32_t creatureId = curActor->creatureId;

			if ( ImGui::InputInt("Primary Anim", &primaryAnim) )
				curActor->primaryAnimIdx = static_cast<int16_t>(primaryAnim);

			if ( ImGui::InputInt("Secondary Anim", &secondaryAnim) )
				curActor->secondaryAnimIdx = static_cast<int16_t>(secondaryAnim);

			if ( ImGui::InputInt("Creature ID##actor", &creatureId) )
				curActor->creatureId = static_cast<int16_t>(creatureId);

			ImGui::PopItemWidth();
		}

		ImGui::Spacing();
		ImGui::SeparatorText("Flags & State");
		{
			ImGui::PushItemWidth(120.0f);

			// Actor flags as individual checkboxes
			auto CheckFlag = [&](const char* label, ActorFlags flag) {
				bool set = (curActor->actorFlags & flag) != 0;
				if ( ImGui::Checkbox(label, &set) )
				{
					if ( set )
						curActor->actorFlags |= flag;
					else
						curActor->actorFlags &= ~flag;
				}
			};

			if ( ImGui::TreeNode("Actor Flags") )
			{
				ImGui::Columns(2, "flagcols", false);

				CheckFlag("Cutscene Move", ACTOR_CUTSCENE_MOVE);
				CheckFlag("Visible/Active", ACTOR_VISIBLE_ACTIVE);
				CheckFlag("Locked Move", ACTOR_LOCKED_MOVE);
				CheckFlag("Grounded", ACTOR_GROUNDED);
				CheckFlag("Floor Snap", ACTOR_FLOOR_SNAP);
				CheckFlag("Stunned", ACTOR_STUNNED);
				CheckFlag("OOB Special Move", ACTOR_OOB_SPECIAL_MOVE);
				CheckFlag("Death Damage", ACTOR_DEATH_DAMAGE);

				ImGui::NextColumn();

				CheckFlag("Airborne Special", ACTOR_AIRBORNE_SPECIAL);
				CheckFlag("Player Trigger", ACTOR_PLAYER_TRIGGER);
				CheckFlag("Vel Clamp Override", ACTOR_VEL_CLAMP_OVERRIDE);
				CheckFlag("No Despawn", ACTOR_NO_DESPAWN);
				CheckFlag("Unused 0x1000", ACTOR_UNUSED_1000);
				CheckFlag("Force Remove", ACTOR_FORCE_REMOVE);
				CheckFlag("Camera Hover", ACTOR_CAMERA_HOVER);
				CheckFlag("Unknown 0x8000", ACTOR_UNKNOWN_8000);

				ImGui::Columns(1);
				ImGui::TreePop();
			}

			ImGui::Spacing();

			int32_t phase = curActor->actorPhase;
			int32_t visDist = curActor->visibilityDistance;

			if ( ImGui::InputInt("Actor Phase", &phase) )
				curActor->actorPhase = static_cast<int16_t>(phase);

			if ( ImGui::InputInt("Visibility Distance", &visDist) )
				curActor->visibilityDistance = static_cast<int16_t>(visDist);

			ImGui::PopItemWidth();
		}

		ImGui::Spacing();
		ImGui::SeparatorText("Velocity");
		{
			ImGui::PushItemWidth(120.0f);

			ImGui::InputInt("Vel X", &curActor->velX);
			ImGui::InputInt("Gravity Vel", &curActor->gravityVel);
			ImGui::InputInt("Vel Forward", &curActor->velForward);

			ImGui::PopItemWidth();
		}

		ImGui::Spacing();
		ImGui::SeparatorText("Boundary");
		{
			ImGui::PushItemWidth(120.0f);

			ImGui::InputInt("Boundary X", &curActor->boundary.x);
			ImGui::InputInt("Boundary Y", &curActor->boundary.y);
			ImGui::InputInt("Boundary Z", &curActor->boundary.z);

			ImGui::Spacing();

			ImGui::InputInt("Motion Target X", &curActor->motionTargetPos.x);
			ImGui::InputInt("Motion Target Y", &curActor->motionTargetPos.y);
			ImGui::InputInt("Motion Target Z", &curActor->motionTargetPos.z);

			ImGui::InputInt("Target Yaw", &curActor->targetYaw);

			ImGui::PopItemWidth();
		}

		ImGui::Spacing();
		ImGui::SeparatorText("Combat");
		{
			ImGui::PushItemWidth(120.0f);

			int32_t dmgCooldown = curActor->damageCooldownTimer;
			if ( ImGui::InputInt("Damage Cooldown", &dmgCooldown) )
				curActor->damageCooldownTimer = static_cast<int16_t>(dmgCooldown);

			ImGui::InputInt("Last Valid Y Pos", &curActor->lastValidYPosition);

			ImGui::PopItemWidth();
		}

		ImGui::Spacing();
		ImGui::SeparatorText("Creature RAM");
		{
			if ( curActor->creatureRam )
			{
				auto cr = curActor->creatureRam;
				ImGui::PushItemWidth(120.0f);

				ImGui::InputInt3("Creature Pos", reinterpret_cast<int*>(&cr->pos));

				ImGui::Spacing();

				int32_t creatureId = cr->creatureId;
				int32_t movCtrl = cr->movCtrl;
				int32_t rotSpeed = cr->rotSpeed;
				int32_t initialFacingAngle = cr->initialFacingAngle;

				if ( ImGui::InputInt("Creature ID##crram", &creatureId) )
					cr->creatureId = static_cast<uint8_t>(creatureId);

				if ( ImGui::InputInt("Mov Ctrl", &movCtrl) )
					cr->movCtrl = static_cast<uint8_t>(movCtrl);

				if ( ImGui::InputInt("Rot Speed", &rotSpeed) )
					cr->rotSpeed = static_cast<uint8_t>(rotSpeed);

				if ( ImGui::InputInt("Initial Facing Angle", &initialFacingAngle) )
					cr->initialFacingAngle = static_cast<uint8_t>(initialFacingAngle);

				ImGui::Spacing();
				ImGui::TextDisabled("Entity Control");

				int32_t entActorPhase = cr->entCtrl.actorPhase;
				int32_t entUnkByte = cr->entCtrl.unkByte;
				int32_t entFlags = cr->entCtrl.actorFlags;

				if ( ImGui::InputInt("Ent Actor Phase", &entActorPhase) )
					cr->entCtrl.actorPhase = static_cast<uint8_t>(entActorPhase);

				if ( ImGui::InputInt("Ent Unk Byte", &entUnkByte) )
					cr->entCtrl.unkByte = static_cast<uint8_t>(entUnkByte);

				if ( ImGui::InputInt("Ent Actor Flags", &entFlags) )
					cr->entCtrl.actorFlags = static_cast<uint16_t>(entFlags);

				ImGui::Spacing();
				ImGui::TextDisabled("Bounds");

				int32_t boundHalfX = cr->boundHalfX;
				int32_t boundHalfZ = cr->boundHalfZ;
				int32_t boundAngle = cr->boundAngle;
				int32_t defenseMode = cr->defenseMode;

				if ( ImGui::InputInt("Bound Half X", &boundHalfX) )
					cr->boundHalfX = static_cast<int16_t>(boundHalfX);

				if ( ImGui::InputInt("Bound Half Z", &boundHalfZ) )
					cr->boundHalfZ = static_cast<int16_t>(boundHalfZ);

				if ( ImGui::InputInt("Bound Angle", &boundAngle) )
					cr->boundAngle = static_cast<int16_t>(boundAngle);

				if ( ImGui::InputInt("Defense Mode", &defenseMode) )
					cr->defenseMode = static_cast<int16_t>(defenseMode);

				ImGui::Spacing();
				ImGui::TextDisabled("Speed");

				int32_t latSpeedNoTarget = cr->latSpeedNoTarget;
				int32_t latSpeedTarget = cr->latSpeedTarget;
				int32_t speedNoTarget = cr->speedNoTarget;
				int32_t speedTarget = cr->speedTarget;

				if ( ImGui::InputInt("Lat Speed (no target)", &latSpeedNoTarget) )
					cr->latSpeedNoTarget = static_cast<uint8_t>(latSpeedNoTarget);

				if ( ImGui::InputInt("Lat Speed (target)", &latSpeedTarget) )
					cr->latSpeedTarget = static_cast<uint8_t>(latSpeedTarget);

				if ( ImGui::InputInt("Speed (no target)", &speedNoTarget) )
					cr->speedNoTarget = static_cast<uint8_t>(speedNoTarget);

				if ( ImGui::InputInt("Speed (target)", &speedTarget) )
					cr->speedTarget = static_cast<uint8_t>(speedTarget);

				ImGui::PopItemWidth();
			}
			else
			{
				ImGui::TextDisabled("(no creature RAM)");
			}
		}

		if ( ImGui::TreeNode("Unknown Fields") )
		{
			ImGui::PushItemWidth(120.0f);

			ImGui::InputInt("Unk Var 7", &curActor->unkVar7);
			ImGui::InputInt("Unk Var 8", &curActor->unkVar8);
			ImGui::InputInt("Unk Var 9", &curActor->unkVar9);
			ImGui::InputInt("Unk Var 12", &curActor->unkVar12);
			ImGui::InputInt("Unk Var 13", &curActor->unkVar13);
			ImGui::InputInt("Unk Var 14", &curActor->unkVar14);
			ImGui::InputInt("Unk Var 28", &curActor->unkVar28);
			ImGui::InputInt("Unk Var 29", &curActor->unkVar29);
			ImGui::InputInt("Unk Var 30", &curActor->unkVar30);
			ImGui::InputInt("Unk Var 34", &curActor->unkVar34);

			ImGui::Spacing();

			int32_t unkVec16x = curActor->unkVector.x, unkVec16y = curActor->unkVector.y, unkVec16z = curActor->unkVector.z;

			if ( ImGui::InputInt("Unk Vec X", &unkVec16x) )
				curActor->unkVector.x = static_cast<int16_t>(unkVec16x);

			if ( ImGui::InputInt("Unk Vec Y", &unkVec16y) )
				curActor->unkVector.y = static_cast<int16_t>(unkVec16y);

			if ( ImGui::InputInt("Unk Vec Z", &unkVec16z) )
				curActor->unkVector.z = static_cast<int16_t>(unkVec16z);

			ImGui::Spacing();

			int32_t w1 = curActor->unkWord1, w2 = curActor->unkWord2, w3 = curActor->unkWord3, w4 = curActor->unkWord4;
			int32_t w15 = curActor->unkWord15, w16 = curActor->unkWord16;
			int32_t s1 = curActor->unkShort1, s2 = curActor->unkShort2, s3 = curActor->unkShort3, s4 = curActor->unkShort4;
			int32_t v10 = curActor->unkVar10;

			if ( ImGui::InputInt("Unk Word 1", &w1) )
				curActor->unkWord1 = static_cast<int16_t>(w1);
			if ( ImGui::InputInt("Unk Word 2", &w2) )
				curActor->unkWord2 = static_cast<int16_t>(w2);
			if ( ImGui::InputInt("Unk Word 3", &w3) )
				curActor->unkWord3 = static_cast<int16_t>(w3);
			if ( ImGui::InputInt("Unk Word 4", &w4) )
				curActor->unkWord4 = static_cast<int16_t>(w4);
			if ( ImGui::InputInt("Unk Word 15", &w15) )
				curActor->unkWord15 = static_cast<int16_t>(w15);
			if ( ImGui::InputInt("Unk Word 16", &w16) )
				curActor->unkWord16 = static_cast<int16_t>(w16);
			if ( ImGui::InputInt("Unk Short 1", &s1) )
				curActor->unkShort1 = static_cast<int16_t>(s1);
			if ( ImGui::InputInt("Unk Short 2", &s2) )
				curActor->unkShort2 = static_cast<int16_t>(s2);
			if ( ImGui::InputInt("Unk Short 3", &s3) )
				curActor->unkShort3 = static_cast<int16_t>(s3);
			if ( ImGui::InputInt("Unk Short 4", &s4) )
				curActor->unkShort4 = static_cast<int16_t>(s4);
			if ( ImGui::InputInt("Unk Var 10", &v10) )
				curActor->unkVar10 = static_cast<int16_t>(v10);

			ImGui::PopItemWidth();
			ImGui::TreePop();
		}

		ImGui::Spacing();
		ImGui::Separator();

		if ( ImGui::Button("Teleport To Player") )
			curActor->pos = g_buzzActor->pos;
	}

	void RenderGui()
	{
		NoClip::UpdateBuzzNoclip();

		ImGui::SetNextWindowSize(ImVec2(500.0f, 800.0f), ImGuiCond_FirstUseEver);

		if ( ImGui::Begin("ToyDebugger") )
		{
			float fps = ImGui::GetIO().Framerate;
			ImGui::Text("ToyDebugger");
			ImGui::SameLine();
			ImGui::TextDisabled("(%.1f fps)", fps);
			ImGui::Separator();

			ImGui::Spacing();

			if ( ImGui::Button("Teleport Actors") )
				TeleportActors();

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::Text("Buzz");
			ImGui::Spacing();

			ImGui::Checkbox("Noclip", &NoClip::g_enabled);
			ImGui::Checkbox("FreeCam", &FreeCam::g_enabled);

			// Actor
			ImGui::Text("Actors");
			ImGui::Spacing();

			if ( ImGui::BeginCombo("", std::format("Actor {}", g_selectedActor + 1).c_str()) )
			{
				for ( int32_t idx = 0; idx < kMaxActors; idx++ )
				{
					bool isSelected = (g_selectedActor == idx);

					if ( ImGui::Selectable(std::format("Actor {}", idx + 1).c_str(), isSelected) )
						g_selectedActor = idx;

					if ( isSelected )
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			DrawActorPanel();
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
