#pragma once

#include <functional>

namespace ImGuiWrapper
{
	using RenderCallback = std::function<void()>;

	void Init();
	void Cleanup();
	void SubscribeRenderCallback(RenderCallback callback);
}
