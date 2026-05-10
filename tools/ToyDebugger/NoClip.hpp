#pragma once

#include "GameInterface.hpp"
#include <windows.h>
#include <imgui.h>

/*
    dog shit noclip implementation, needs lots of work
*/

namespace NoClip
{
	inline bool g_buzzNoclipEnabled = false;
	inline Vector3I g_buzzNoclipPos = {};

	// Toy Story 2 positions look like fixed-point/world-unit integers.
	// Tune these live from ImGui.
	inline int32_t g_buzzNoclipSpeed = 180000;
	inline int32_t g_buzzNoclipFastSpeed = 500000;

	constexpr float kPi = 3.14159265358979323846f;

	inline bool IsKeyDown(int vk)
	{
		return (GetAsyncKeyState(vk) & 0x8000) != 0;
	}

	// The game seems to use 0x1000 / 4096 units for a full rotation.
	inline float GameAngleToRadians(int32_t angle)
	{
		return static_cast<float>(angle & 0xFFF) * ((2.0f * kPi) / 4096.0f);
	}

	inline void SetBuzzNoclipEnabled(bool enabled)
	{
		g_buzzNoclipEnabled = enabled;

		if ( g_buzzNoclipEnabled )
			g_buzzNoclipPos = g_buzzActor->pos;
	}

	inline void WriteBuzzNoclipPos()
	{
		g_buzzActor->pos = g_buzzNoclipPos;
		//g_buzzActor->motionTargetPos = g_buzzNoclipPos;
	}

	inline void UpdateBuzzNoclip()
	{
		if ( ! g_buzzNoclipEnabled )
			return;

		ImGuiIO& io = ImGui::GetIO();

		if ( io.WantTextInput )
		{
			WriteBuzzNoclipPos();
			return;
		}

		float inputX = 0.0f; // strafe
		float inputY = 0.0f; // up/down
		float inputZ = 0.0f; // forward/back

		if ( IsKeyDown('W') )
			inputZ += 1.0f;

		if ( IsKeyDown('S') )
			inputZ -= 1.0f;

		// if ( IsKeyDown('A') )
		// 	inputX -= 1.0f;

		// if ( IsKeyDown('D') )
		// 	inputX += 1.0f;

		if ( IsKeyDown(VK_SPACE) )
			inputY += 1.0f;

		if ( IsKeyDown(VK_CONTROL) )
			inputY -= 1.0f;

		const float lenSq = inputX * inputX + inputY * inputY + inputZ * inputZ;

		if ( lenSq > 0.0f )
		{
			const float invLen = 1.0f / std::sqrt(lenSq);

			inputX *= invLen;
			inputY *= invLen;
			inputZ *= invLen;

			const int32_t speed = IsKeyDown(VK_SHIFT) ? g_buzzNoclipFastSpeed : g_buzzNoclipSpeed;
			const float step = static_cast<float>(speed) * io.DeltaTime;

			const float yaw = GameAngleToRadians(g_buzzActor->facingAngle);

			const float sinYaw = std::sin(yaw);
			const float cosYaw = std::cos(yaw);

			// Forward vector based on Buzz's facing angle.
			const float forwardX = sinYaw;
			const float forwardZ = cosYaw;

			// Right vector, 90 degrees from forward.
			const float rightX = cosYaw;
			const float rightZ = -sinYaw;

			const float worldX = rightX * inputX + forwardX * inputZ;
			const float worldZ = rightZ * inputX + forwardZ * inputZ;

			g_buzzNoclipPos.x += static_cast<int32_t>(worldX * step);
			g_buzzNoclipPos.y += static_cast<int32_t>(inputY * step);
			g_buzzNoclipPos.z += static_cast<int32_t>(worldZ * step);
		}

		WriteBuzzNoclipPos();
	}
}
