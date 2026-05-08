#pragma once

#include <directx7/d3d.h>
#include <directx7/d3dtypes.h>
#include <directx7/d3dcaps.h>
#include <directx7/ddraw.h>

#include <cstdint>

struct ImDrawData;

bool ImGui_ImplD3D3_Init(IDirectDraw4* ddraw, IDirect3DDevice3* device, int32_t displayWidth, int32_t displayHeight);
void ImGui_ImplD3D3_Shutdown();
void ImGui_ImplD3D3_NewFrame(int32_t displayWidth, int32_t displayHeight, float deltaSeconds);
void ImGui_ImplD3D3_RenderDrawData(ImDrawData* drawData);
bool ImGui_ImplD3D3_CreateDeviceObjects();
void ImGui_ImplD3D3_InvalidateDeviceObjects();
