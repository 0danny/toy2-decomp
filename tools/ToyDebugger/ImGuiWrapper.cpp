#include "ImGuiWrapper.hpp"
#include "imgui_impl_d3d3.hpp"
#include "Common/GameStructs.hpp"

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <windows.h>
#include <minhook.h>
#include <iostream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	// typedef hooks
	typedef void(__stdcall* EndSceneHook)();
	typedef LRESULT(__stdcall* WndProcHook)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// original functions
	static EndSceneHook o_EndSceneHook;
	static WndProcHook o_WndProcHook;

	// addresses of functions
	static LPVOID EndSceneAddress = (LPVOID)0x004ABAD0;
	static LPVOID WndProcAddress = (LPVOID)0x004A6D40;

	// imgui
	static bool g_imguiInitialised = false;
	static bool g_imguiFailed = false;
	static ImGuiContext* g_imguiContext = nullptr;
	static DWORD g_lastFrameTick = 0;
	static bool g_showWindow = false;

	static CD3DFramework** g_drawingDevice = reinterpret_cast<CD3DFramework**>(0x00884008);
	static CD3DFramework* g_framework = nullptr;
	static ImGuiWrapper::RenderCallback g_renderCallback = nullptr;
}

namespace ImGuiWrapper
{
	static LRESULT __stdcall WndProcHookFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if ( msg == WM_KEYDOWN && wParam == VK_F12 )
		{
			g_showWindow = ! g_showWindow;
			return 0;
		}

		if ( g_imguiInitialised && g_imguiContext )
		{
			ImGui::SetCurrentContext(g_imguiContext);
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		}

		return o_WndProcHook(hWnd, msg, wParam, lParam);
	}

	static float GetFrameDeltaSeconds()
	{
		DWORD now = GetTickCount();

		if ( g_lastFrameTick == 0 )
		{
			g_lastFrameTick = now;
			return 1.0f / 60.0f;
		}

		DWORD elapsed = now - g_lastFrameTick;
		g_lastFrameTick = now;

		if ( elapsed == 0 )
			return 1.0f / 60.0f;

		float delta = static_cast<float>(elapsed) / 1000.0f;

		if ( delta <= 0.0f )
			delta = 1.0f / 60.0f;

		if ( delta > 0.25f )
			delta = 0.25f;

		return delta;
	}

	static int32_t GetRenderWidth(const CD3DFramework* framework)
	{
		if ( framework->dwRenderWidth > 0 )
			return framework->dwRenderWidth;

		return framework->rcViewportRect.right - framework->rcViewportRect.left;
	}

	static int32_t GetRenderHeight(const CD3DFramework* framework)
	{
		if ( framework->dwRenderHeight > 0 )
			return framework->dwRenderHeight;

		return framework->rcViewportRect.bottom - framework->rcViewportRect.top;
	}

	static bool InitImGui()
	{
		if ( g_imguiInitialised )
			return true;

		if ( g_imguiFailed )
			return false;

		if ( ! g_framework )
			g_framework = g_drawingDevice[0];

		const int32_t width = GetRenderWidth(g_framework);
		const int32_t height = GetRenderHeight(g_framework);

		if ( width <= 0 || height <= 0 )
			return false;

		IMGUI_CHECKVERSION();

		g_imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(g_imguiContext);

		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		ImGui::StyleColorsDark();

		HWND hwnd = reinterpret_cast<HWND>(g_framework->hWnd);

		if ( ! ImGui_ImplWin32_Init(hwnd) )
		{
			ImGui::DestroyContext(g_imguiContext);
			g_imguiContext = nullptr;
			g_imguiFailed = true;
			return false;
		}

		if ( ! ImGui_ImplD3D3_Init(g_framework->pDD, g_framework->pd3dDevice, width, height) )
		{
			ImGui_ImplWin32_Shutdown();

			ImGui::DestroyContext(g_imguiContext);
			g_imguiContext = nullptr;
			g_imguiFailed = true;
			return false;
		}

		g_imguiInitialised = true;

		printf("[IMGUI]: Initialised D3D3 backend. Size: %d x %d\n", width, height);

		return true;
	}

	static void RenderImGui()
	{
		if ( ! InitImGui() )
			return;

		ImGui::SetCurrentContext(g_imguiContext);

		const int32_t width = GetRenderWidth(g_framework);
		const int32_t height = GetRenderHeight(g_framework);
		const float deltaSeconds = GetFrameDeltaSeconds();

		ImGui_ImplWin32_NewFrame();
		ImGui_ImplD3D3_NewFrame(width, height, deltaSeconds);
		ImGui::NewFrame();

		if ( g_showWindow && g_renderCallback )
			g_renderCallback();

		ImGui::Render();

		ImGui_ImplD3D3_RenderDrawData(ImGui::GetDrawData());
	}

	static void __stdcall EndSceneHookFunction()
	{
		RenderImGui();
		ShowCursor(1);
		o_EndSceneHook();
	}

	void Init()
	{
		if ( MH_CreateHook(EndSceneAddress, (LPVOID)&EndSceneHookFunction, reinterpret_cast<LPVOID*>(&o_EndSceneHook)) != MH_OK )
			printf("[HOOK]: Could not hook EndSceneHookFunction function.\n");

		if ( MH_CreateHook(WndProcAddress, (LPVOID)&WndProcHookFunction, reinterpret_cast<LPVOID*>(&o_WndProcHook)) != MH_OK )
			printf("[HOOK]: Could not hook WndProcHookFunction function.\n");
	}

	void Cleanup()
	{
		if ( ! g_imguiInitialised )
			return;

		ImGui::SetCurrentContext(g_imguiContext);

		ImGui_ImplD3D3_Shutdown();
		ImGui_ImplWin32_Shutdown();

		ImGui::DestroyContext(g_imguiContext);

		g_imguiContext = nullptr;
		g_imguiInitialised = false;
	}

	void SubscribeRenderCallback(RenderCallback callback)
	{
		g_renderCallback = std::move(callback);
	}
}
