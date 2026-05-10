#include "FreeCam.hpp"
#include "GameInterface.hpp"
#include <minhook.h>
#include <iostream>

namespace
{
	// typedef hooks
	typedef void(__stdcall* UpdateCameraHook)();

	// original functions
	static UpdateCameraHook o_UpdateCameraHook;

	// addresses of functions
	static LPVOID UpdateCameraAddress = (LPVOID)0x00405860;

	// Free cam state
	static int32_t s_posX = 0;
	static int32_t s_posY = 0;
	static int32_t s_posZ = 0;
	static int32_t s_yaw = 0;   // 12-bit, 0..4095
	static int32_t s_pitch = 0; // 12-bit, 0..4095

	// Mouse tracking
	static POINT s_lastMousePos = {};
	static bool s_mouseInit = false;

	// Config
	// Base move speed in game fixed-point units per frame.
	// Game world units are /32 = float coords, so 512 = 16.0 units/frame.
	static constexpr int32_t kBaseMoveSpeed = 1024;
	static constexpr int32_t kFastMoveSpeed = 2048;
	// Mouse sensitivity: 12-bit units per pixel
	static constexpr int32_t kMouseSensX = 3; // yaw
	static constexpr int32_t kMouseSensY = 2; // pitch

	// Trig helpers using game's 12-bit angle range (4096 = 360 degrees)
	// We replicate with standard sin/cos and map 0..4095 -> 0..2pi
	static constexpr float kTwoPi = 6.2831853f;
	static bool s_wasEnabled = false;
	static bool s_mouseLocked = false;
}

namespace FreeCam
{
	static void SetMouseLocked(bool locked)
	{
		if ( locked )
		{
			HWND hwnd = GetForegroundWindow();

			RECT rect;
			GetClientRect(hwnd, &rect);

			POINT tl = { rect.left, rect.top };
			POINT br = { rect.right, rect.bottom };
			ClientToScreen(hwnd, &tl);
			ClientToScreen(hwnd, &br);

			RECT screenRect = { tl.x, tl.y, br.x, br.y };
			ClipCursor(&screenRect);

			// Warp mouse to center so first delta is always 0
			int cx = (tl.x + br.x) / 2;
			int cy = (tl.y + br.y) / 2;
			SetCursorPos(cx, cy);
			s_lastMousePos = { cx, cy };
		}
		else
			ClipCursor(nullptr); // Release clip
	}

	inline float AngleToRad(int32_t angle12bit)
	{
		return (angle12bit & 0xFFF) * (kTwoPi / 4096.0f);
	}

	static void GetCameraAxes(int32_t yaw, float& fwdX, float& fwdZ, float& rightX, float& rightZ)
	{
		// Yaw 0 = facing +Z, increasing yaw rotates clockwise (matching RotateYFromLut).
		// ApplyTransformToCamera does:  g_cameraRotY = 16 * yaw
		// RotateYFromLut is a standard left-hand Y rotation.
		float r = AngleToRad(yaw);
		fwdX = sinf(r);
		fwdZ = cosf(r);
		rightX = cosf(r);
		rightZ = -sinf(r);
	}

	static void SnapFreeCamToGame()
	{
		s_posX = g_activeCameraTransform->pos.x;
		s_posY = g_activeCameraTransform->pos.y;
		s_posZ = g_activeCameraTransform->pos.z;
		s_yaw = g_activeCameraTransform->angles.yaw;
		s_pitch = g_activeCameraTransform->angles.pitch;
	}

	static void TickFreeCam()
	{
		if ( ! s_wasEnabled )
		{
			SnapFreeCamToGame();
			s_mouseInit = false;
			s_wasEnabled = true;
		}

		POINT curMouse = {};
		GetCursorPos(&curMouse);

		if ( ! s_mouseInit )
		{
			s_lastMousePos = curMouse;
			s_mouseInit = true;
		}

		int32_t dx = curMouse.x - s_lastMousePos.x;
		int32_t dy = curMouse.y - s_lastMousePos.y;
		s_lastMousePos = curMouse;

		s_yaw = (s_yaw + dx * kMouseSensX) & 0xFFF;

		s_pitch = (s_pitch + dy * kMouseSensY) & 0xFFF;
		{
			// Convert to signed
			int32_t sp = s_pitch;
			if ( sp >= 0x800 )
				sp -= 0x1000;

			const int32_t kPitchLimit = 512;

			if ( sp > kPitchLimit )
				sp = kPitchLimit;

			if ( sp < -kPitchLimit )
				sp = -kPitchLimit;

			s_pitch = sp & 0xFFF;
		}

		bool fast = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
		int32_t speed = fast ? kFastMoveSpeed : kBaseMoveSpeed;

		float fwdX, fwdZ, rightX, rightZ;
		GetCameraAxes(s_yaw, fwdX, fwdZ, rightX, rightZ);

		float moveX = 0.0f, moveY = 0.0f, moveZ = 0.0f;

		if ( GetAsyncKeyState('W') & 0x8000 )
		{
			moveX += fwdX;
			moveZ += fwdZ;
		}
		if ( GetAsyncKeyState('S') & 0x8000 )
		{
			moveX -= fwdX;
			moveZ -= fwdZ;
		}
		if ( GetAsyncKeyState('D') & 0x8000 )
		{
			moveX += rightX;
			moveZ += rightZ;
		}
		if ( GetAsyncKeyState('A') & 0x8000 )
		{
			moveX -= rightX;
			moveZ -= rightZ;
		}

		// Vertical: E/Q or Space/Ctrl
		if ( GetAsyncKeyState('E') & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8000 )
			moveY -= 1.0f;

		if ( GetAsyncKeyState('Q') & 0x8000 || GetAsyncKeyState(VK_CONTROL) & 0x8000 )
			moveY += 1.0f;

		s_posX += (int32_t)(moveX * speed);
		s_posY += (int32_t)(moveY * speed);
		s_posZ += (int32_t)(moveZ * speed);

		g_activeCameraTransform->pos.x = s_posX;
		g_activeCameraTransform->pos.y = s_posY;
		g_activeCameraTransform->pos.z = s_posZ;
		g_activeCameraTransform->angles.yaw = (uint16_t)(s_yaw & 0xFFF);
		g_activeCameraTransform->angles.pitch = (uint16_t)(s_pitch & 0xFFF);
		g_activeCameraTransform->roll = 0;
	}

	static void __stdcall UpdateCameraFunction()
	{
		// if we call the original function it will drive the original camera logic
		if ( ! g_enabled )
		{
			if ( s_mouseLocked )
			{
				SetMouseLocked(false);
				s_mouseLocked = false;
				g_buzzActor->actorFlags &= ~ACTOR_LOCKED_MOVE;
			}

			o_UpdateCameraHook();
			return;
		}

		if ( ! s_mouseLocked )
		{
			SetMouseLocked(true);
			s_mouseLocked = true;

			// lock buzz's movement aswell
			g_buzzActor->actorFlags |= ACTOR_LOCKED_MOVE;
		}

		// free cam implementation
		TickFreeCam();
	}

	void Init()
	{
		if ( MH_CreateHook(UpdateCameraAddress, (LPVOID)&UpdateCameraFunction, reinterpret_cast<LPVOID*>(&o_UpdateCameraHook)) != MH_OK )
			printf("[HOOK]: Could not hook UpdateCameraHook function.\n");
	}
}
