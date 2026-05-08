#include "imgui_impl_d3d3.hpp"

#include <imgui.h>
#include <string.h>
#include <vector>

/*
	ImGui Direct3D3/DirectDraw4 minimal implementation
*/

namespace
{
	static IDirectDraw4* g_ddraw = NULL;
	static IDirect3DDevice3* g_device = NULL;

	static IDirectDrawSurface4* g_fontSurface = NULL;
	static IDirect3DTexture2* g_fontTexture = NULL;
	static IDirect3DViewport3* g_viewport = NULL;

	static int32_t g_displayWidth = 0;
	static int32_t g_displayHeight = 0;

	struct D3D3Vertex
	{
		float sx;
		float sy;
		float sz;
		float rhw;
		DWORD colour;
		DWORD specular;
		float tu;
		float tv;
	};

	static DWORD ImGuiColourToD3DColour(ImU32 colour)
	{
		// ImGui: 0xAABBGGRR
		// D3D:   0xAARRGGBB
		return (colour & 0xFF00FF00) | ((colour & 0x000000FF) << 16) | ((colour & 0x00FF0000) >> 16);
	}

	static ImTextureID ToImTextureID(IDirect3DTexture2* texture)
	{
		return (ImTextureID)texture;
	}

	static IDirect3DTexture2* FromImTextureID(ImTextureID textureId)
	{
		return (IDirect3DTexture2*)textureId;
	}

	static void SafeRelease(IDirectDrawSurface4*& surface)
	{
		if ( surface )
		{
			surface->Release();
			surface = NULL;
		}
	}

	static void SafeRelease(IDirect3DTexture2*& texture)
	{
		if ( texture )
		{
			texture->Release();
			texture = NULL;
		}
	}

	struct SavedD3D3State
	{
		DWORD zEnable;
		DWORD zWriteEnable;
		DWORD alphaBlendEnable;
		DWORD srcBlend;
		DWORD destBlend;
		DWORD alphaTestEnable;
		DWORD lighting;
		DWORD cullMode;
		DWORD shadeMode;
		DWORD fogEnable;
		DWORD specularEnable;

		DWORD colourOp;
		DWORD colourArg1;
		DWORD colourArg2;
		DWORD alphaOp;
		DWORD alphaArg1;
		DWORD alphaArg2;

		IDirect3DTexture2* texture0;
	};

	static void SaveState(SavedD3D3State& state)
	{
		memset(&state, 0, sizeof(state));

		g_device->GetRenderState(D3DRENDERSTATE_ZENABLE, &state.zEnable);
		g_device->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &state.zWriteEnable);
		g_device->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &state.alphaBlendEnable);
		g_device->GetRenderState(D3DRENDERSTATE_SRCBLEND, &state.srcBlend);
		g_device->GetRenderState(D3DRENDERSTATE_DESTBLEND, &state.destBlend);
		g_device->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &state.alphaTestEnable);
		g_device->GetRenderState(D3DRENDERSTATE_LIGHTING, &state.lighting);
		g_device->GetRenderState(D3DRENDERSTATE_CULLMODE, &state.cullMode);
		g_device->GetRenderState(D3DRENDERSTATE_SHADEMODE, &state.shadeMode);
		g_device->GetRenderState(D3DRENDERSTATE_FOGENABLE, &state.fogEnable);
		g_device->GetRenderState(D3DRENDERSTATE_SPECULARENABLE, &state.specularEnable);

		g_device->GetTextureStageState(0, D3DTSS_COLOROP, &state.colourOp);
		g_device->GetTextureStageState(0, D3DTSS_COLORARG1, &state.colourArg1);
		g_device->GetTextureStageState(0, D3DTSS_COLORARG2, &state.colourArg2);
		g_device->GetTextureStageState(0, D3DTSS_ALPHAOP, &state.alphaOp);
		g_device->GetTextureStageState(0, D3DTSS_ALPHAARG1, &state.alphaArg1);
		g_device->GetTextureStageState(0, D3DTSS_ALPHAARG2, &state.alphaArg2);

		g_device->GetTexture(0, &state.texture0);
	}

	static void RestoreState(SavedD3D3State& state)
	{
		g_device->SetRenderState(D3DRENDERSTATE_ZENABLE, state.zEnable);
		g_device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, state.zWriteEnable);
		g_device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, state.alphaBlendEnable);
		g_device->SetRenderState(D3DRENDERSTATE_SRCBLEND, state.srcBlend);
		g_device->SetRenderState(D3DRENDERSTATE_DESTBLEND, state.destBlend);
		g_device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, state.alphaTestEnable);
		g_device->SetRenderState(D3DRENDERSTATE_LIGHTING, state.lighting);
		g_device->SetRenderState(D3DRENDERSTATE_CULLMODE, state.cullMode);
		g_device->SetRenderState(D3DRENDERSTATE_SHADEMODE, state.shadeMode);
		g_device->SetRenderState(D3DRENDERSTATE_FOGENABLE, state.fogEnable);
		g_device->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, state.specularEnable);

		g_device->SetTextureStageState(0, D3DTSS_COLOROP, state.colourOp);
		g_device->SetTextureStageState(0, D3DTSS_COLORARG1, state.colourArg1);
		g_device->SetTextureStageState(0, D3DTSS_COLORARG2, state.colourArg2);
		g_device->SetTextureStageState(0, D3DTSS_ALPHAOP, state.alphaOp);
		g_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, state.alphaArg1);
		g_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, state.alphaArg2);

		g_device->SetTexture(0, state.texture0);

		if ( state.texture0 )
			state.texture0->Release();
	}

	static void SetupRenderState()
	{
		g_device->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
		g_device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
		g_device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		g_device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		g_device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		g_device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
		g_device->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
		g_device->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
		g_device->SetRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD);
		g_device->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
		g_device->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, FALSE);

		g_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		g_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		g_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		g_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		g_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		g_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		g_device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}

	static bool CreateFontTextureA8R8G8B8(const uint8_t* pixels, int32_t width, int32_t height)
	{
		DDSURFACEDESC2 desc;
		memset(&desc, 0, sizeof(desc));

		desc.dwSize = sizeof(desc);
		desc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
		desc.dwWidth = width;
		desc.dwHeight = height;
		desc.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;

		desc.ddpfPixelFormat.dwSize = sizeof(desc.ddpfPixelFormat);
		desc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
		desc.ddpfPixelFormat.dwRGBBitCount = 32;
		desc.ddpfPixelFormat.dwRBitMask = 0x00FF0000;
		desc.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
		desc.ddpfPixelFormat.dwBBitMask = 0x000000FF;
		desc.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;

		HRESULT hr = g_ddraw->CreateSurface(&desc, &g_fontSurface, NULL);
		if ( FAILED(hr) )
			return false;

		DDSURFACEDESC2 lockDesc;
		memset(&lockDesc, 0, sizeof(lockDesc));
		lockDesc.dwSize = sizeof(lockDesc);

		hr = g_fontSurface->Lock(NULL, &lockDesc, DDLOCK_WAIT | DDLOCK_WRITEONLY, NULL);
		if ( FAILED(hr) )
		{
			SafeRelease(g_fontSurface);
			return false;
		}

		for ( int32_t y = 0; y < height; y++ )
		{
			uint8_t* dstRow = (uint8_t*)lockDesc.lpSurface + y * lockDesc.lPitch;
			DWORD* dst = (DWORD*)dstRow;

			const uint8_t* src = pixels + y * width * 4;

			for ( int32_t x = 0; x < width; x++ )
			{
				const uint8_t r = src[x * 4 + 0];
				const uint8_t g = src[x * 4 + 1];
				const uint8_t b = src[x * 4 + 2];
				const uint8_t a = src[x * 4 + 3];

				dst[x] = ((DWORD)a << 24) | ((DWORD)r << 16) | ((DWORD)g << 8) | ((DWORD)b);
			}
		}

		g_fontSurface->Unlock(NULL);

		hr = g_fontSurface->QueryInterface(IID_IDirect3DTexture2, (void**)&g_fontTexture);
		if ( FAILED(hr) )
		{
			SafeRelease(g_fontSurface);
			return false;
		}

		return true;
	}

	static bool CreateFontTextureA4R4G4B4(const uint8_t* pixels, int32_t width, int32_t height)
	{
		DDSURFACEDESC2 desc;
		memset(&desc, 0, sizeof(desc));

		desc.dwSize = sizeof(desc);
		desc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
		desc.dwWidth = width;
		desc.dwHeight = height;
		desc.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;

		desc.ddpfPixelFormat.dwSize = sizeof(desc.ddpfPixelFormat);
		desc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
		desc.ddpfPixelFormat.dwRGBBitCount = 16;
		desc.ddpfPixelFormat.dwRBitMask = 0x0F00;
		desc.ddpfPixelFormat.dwGBitMask = 0x00F0;
		desc.ddpfPixelFormat.dwBBitMask = 0x000F;
		desc.ddpfPixelFormat.dwRGBAlphaBitMask = 0xF000;

		HRESULT hr = g_ddraw->CreateSurface(&desc, &g_fontSurface, NULL);
		if ( FAILED(hr) )
			return false;

		DDSURFACEDESC2 lockDesc;
		memset(&lockDesc, 0, sizeof(lockDesc));
		lockDesc.dwSize = sizeof(lockDesc);

		hr = g_fontSurface->Lock(NULL, &lockDesc, DDLOCK_WAIT | DDLOCK_WRITEONLY, NULL);
		if ( FAILED(hr) )
		{
			SafeRelease(g_fontSurface);
			return false;
		}

		for ( int32_t y = 0; y < height; y++ )
		{
			uint8_t* dstRow = (uint8_t*)lockDesc.lpSurface + y * lockDesc.lPitch;
			WORD* dst = (WORD*)dstRow;

			const uint8_t* src = pixels + y * width * 4;

			for ( int32_t x = 0; x < width; x++ )
			{
				const uint8_t r = src[x * 4 + 0];
				const uint8_t g = src[x * 4 + 1];
				const uint8_t b = src[x * 4 + 2];
				const uint8_t a = src[x * 4 + 3];

				dst[x] = (WORD)(((a >> 4) << 12) | ((r >> 4) << 8) | ((g >> 4) << 4) | (b >> 4));
			}
		}

		g_fontSurface->Unlock(NULL);

		hr = g_fontSurface->QueryInterface(IID_IDirect3DTexture2, (void**)&g_fontTexture);
		if ( FAILED(hr) )
		{
			SafeRelease(g_fontSurface);
			return false;
		}

		return true;
	}
}

bool ImGui_ImplD3D3_Init(IDirectDraw4* ddraw, IDirect3DDevice3* device, int32_t displayWidth, int32_t displayHeight)
{
	if ( ! ddraw || ! device )
		return false;

	g_ddraw = ddraw;
	g_device = device;
	g_displayWidth = displayWidth;
	g_displayHeight = displayHeight;

	g_ddraw->AddRef();
	g_device->AddRef();

	g_device->GetCurrentViewport(&g_viewport);
	g_viewport->AddRef();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererName = "imgui_impl_d3d3_minimal";

	return ImGui_ImplD3D3_CreateDeviceObjects();
}

void ImGui_ImplD3D3_Shutdown()
{
	ImGui_ImplD3D3_InvalidateDeviceObjects();

	if ( g_device )
	{
		g_device->Release();
		g_device = NULL;
	}

	if ( g_ddraw )
	{
		g_ddraw->Release();
		g_ddraw = NULL;
	}
}

void ImGui_ImplD3D3_NewFrame(int32_t displayWidth, int32_t displayHeight, float deltaSeconds)
{
	g_displayWidth = displayWidth;
	g_displayHeight = displayHeight;

	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2((float)displayWidth, (float)displayHeight);
	io.DeltaTime = deltaSeconds > 0.0f ? deltaSeconds : (1.0f / 60.0f);

	if ( ! g_fontTexture )
		ImGui_ImplD3D3_CreateDeviceObjects();
}

bool ImGui_ImplD3D3_CreateDeviceObjects()
{
	if ( ! g_ddraw || ! g_device )
		return false;

	if ( g_fontTexture )
		return true;

	ImGuiIO& io = ImGui::GetIO();

	uint8_t* pixels = NULL;
	int32_t width = 0;
	int32_t height = 0;

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	if ( ! pixels || width <= 0 || height <= 0 )
		return false;

	if ( ! CreateFontTextureA8R8G8B8(pixels, width, height) )
	{
		if ( ! CreateFontTextureA4R4G4B4(pixels, width, height) )
			return false;
	}

	io.Fonts->TexID = ToImTextureID(g_fontTexture);
	return true;
}

void ImGui_ImplD3D3_InvalidateDeviceObjects()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->TexID = (ImTextureID)0;

	SafeRelease(g_fontTexture);
	SafeRelease(g_fontSurface);
}

void ImGui_ImplD3D3_RenderDrawData(ImDrawData* drawData)
{
	if ( ! drawData || ! g_device || ! g_fontTexture )
		return;

	if ( drawData->TotalVtxCount <= 0 )
		return;

	IM_ASSERT(sizeof(ImDrawIdx) == sizeof(WORD));

	SavedD3D3State oldState;
	SaveState(oldState);
	SetupRenderState();

	D3DVIEWPORT2 originalViewport;
	memset(&originalViewport, 0, sizeof(originalViewport));
	originalViewport.dwSize = sizeof(originalViewport);
	g_viewport->GetViewport2(&originalViewport);

	const ImVec2 clipOff = drawData->DisplayPos;

	for ( int32_t listIndex = 0; listIndex < drawData->CmdListsCount; listIndex++ )
	{
		const ImDrawList* cmdList = drawData->CmdLists[listIndex];

		std::vector<D3D3Vertex> vertices;
		vertices.resize(cmdList->VtxBuffer.Size);

		for ( int32_t i = 0; i < cmdList->VtxBuffer.Size; i++ )
		{
			const ImDrawVert& src = cmdList->VtxBuffer[i];
			D3D3Vertex& dst = vertices[i];

			dst.sx = src.pos.x - clipOff.x - 0.5f;
			dst.sy = src.pos.y - clipOff.y - 0.5f;
			dst.sz = 0.0f;
			dst.rhw = 1.0f;
			dst.colour = ImGuiColourToD3DColour(src.col);
			dst.specular = 0;
			dst.tu = src.uv.x;
			dst.tv = src.uv.y;
		}

		for ( int32_t cmdIndex = 0; cmdIndex < cmdList->CmdBuffer.Size; cmdIndex++ )
		{
			const ImDrawCmd* cmd = &cmdList->CmdBuffer[cmdIndex];

			if ( cmd->UserCallback )
			{
				cmd->UserCallback(cmdList, cmd);
				continue;
			}

			if ( cmd->ElemCount == 0 )
				continue;

			ImVec4 clipRect = cmd->ClipRect;
			clipRect.x -= clipOff.x;
			clipRect.y -= clipOff.y;
			clipRect.z -= clipOff.x;
			clipRect.w -= clipOff.y;

			if ( clipRect.x >= g_displayWidth || clipRect.y >= g_displayHeight )
				continue;

			if ( clipRect.z <= 0.0f || clipRect.w <= 0.0f )
				continue;

			IDirect3DTexture2* texture = FromImTextureID(cmd->GetTexID());
			if ( ! texture )
				texture = g_fontTexture;

			float cx = max(clipRect.x, 0.0f);
			float cy = max(clipRect.y, 0.0f);
			float cz = min(clipRect.z, (float)g_displayWidth);
			float cw = min(clipRect.w, (float)g_displayHeight);

			if ( cx >= cz || cy >= cw )
				continue;

			D3DVIEWPORT2 vp;
			memset(&vp, 0, sizeof(vp));
			vp.dwSize = sizeof(vp);
			vp.dwX = (DWORD)cx;
			vp.dwY = (DWORD)cy;
			vp.dwWidth = (DWORD)(cz - cx);
			vp.dwHeight = (DWORD)(cw - cy);
			vp.dvClipX = -1.0f;
			vp.dvClipY = 1.0f;
			vp.dvClipWidth = 2.0f;
			vp.dvClipHeight = 2.0f;
			vp.dvMinZ = 0.0f;
			vp.dvMaxZ = 1.0f;

			g_viewport->SetViewport2(&vp);

			g_device->SetTexture(0, texture);

			const WORD* indices = (const WORD*)(cmdList->IdxBuffer.Data + cmd->IdxOffset);

			if ( cmd->VtxOffset != 0 )
			{
				printf("Overflowing VTX offset\n");
				continue;
			}

			g_device->DrawIndexedPrimitive(
			    D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, vertices.empty() ? NULL : &vertices[0], (DWORD)vertices.size(), (LPWORD)indices, cmd->ElemCount,
			    D3DDP_DONOTLIGHT
			);
		}
	}

	g_viewport->SetViewport2(&originalViewport);

	RestoreState(oldState);
}
