#pragma once

#include "Common.h"
#include "Renderer/RenderType.h"
#include <directx6/ddraw.h>
#include <directx6/d3d.h>

namespace Nu3D
{
	struct Primitive;
	struct Material;
	struct InstanceData;
}

namespace DrawingAPI
{
	// Drawing Methods
	typedef HRESULT Device_DrawIndexedPrimitiveVB(
		D3DPRIMITIVETYPE primitiveType, LPDIRECT3DVERTEXBUFFER vertexBuffer, WORD* indices, DWORD indexCount, DWORD flags);
	typedef HRESULT Device_DrawIndexedPrimitive(D3DPRIMITIVETYPE d3dptPrimitiveType,
		DWORD dwVertexTypeDesc,
		LPVOID lpvVertices,
		DWORD dwVertexCount,
		LPWORD lpwIndices,
		DWORD dwIndexCount,
		DWORD dwFlags);

	// Vertex Methods
	typedef HRESULT Device_ReleaseVertexBuffer(LPDIRECT3DVERTEXBUFFER buffer);
	typedef HRESULT Device_CreateVertexBuffer(D3DVERTEXBUFFERDESC* desc, LPDIRECT3DVERTEXBUFFER* outBuffer, DWORD flags);
	typedef HRESULT Device_LockVertexBuffer(LPDIRECT3DVERTEXBUFFER vertexBuffer, DWORD dwFlags, LPVOID* lplpData, DWORD* lpStride);
	typedef HRESULT Device_UnlockVertexBuffer(LPDIRECT3DVERTEXBUFFER buffer);
	typedef HRESULT Device_OptimizeVertexBuffer(LPDIRECT3DVERTEXBUFFER buffer, LPDIRECT3DDEVICE3 device, DWORD flags);
	typedef HRESULT Device_ProcessVerticesOnBuffer(LPDIRECT3DVERTEXBUFFER destBuffer,
		DWORD dwVertexOp,
		DWORD dwDestIndex,
		DWORD dwCount,
		LPDIRECT3DVERTEXBUFFER srcBuffer,
		DWORD dwSrcIndex,
		DWORD dwFlags);

	extern Device_DrawIndexedPrimitive* DrawIndexedPrimitive;
	extern Device_DrawIndexedPrimitiveVB* DrawIndexedPrimitiveVB;

	extern Device_ReleaseVertexBuffer* ReleaseVertexBuffer;
	extern Device_CreateVertexBuffer* CreateVertexBuffer;
	extern Device_LockVertexBuffer* LockVertexBuffer;
	extern Device_UnlockVertexBuffer* UnlockVertexBuffer;
	extern Device_OptimizeVertexBuffer* OptimizeVertexBuffer;
	extern Device_ProcessVerticesOnBuffer* ProcessVerticesOnBuffer;
}

namespace Renderer
{
	struct RenderEntry
	{
		RenderEntry* next;
		Renderer::RenderType type;
		float distanceSquared;
		Nu3D::Primitive* primitive;
		Nu3D::InstanceData* instanceData;
		Nu3D::Material* material;
	};

	extern int32_t g_isSoftwareRendering;
	extern uint32_t g_frameDelta;
	extern float g_virtualScreenWidth;
	extern float g_virtualScreenHeight;

	void Cleanup();
	void Init();
	void SetIsSoftwareRendering(int32_t value);
	void SetVirtualRatioTo54();
	void DoFrameDelay(int32_t isGameplayFrame);

	// Draw Methods
	void ClearScreen(RGBA clearColor, int32_t clearFlags);
	int32_t BeginScene();
	void EndScene(int32_t presentFrame);

	// Render Methods
	void DrawMainMenuText(int16_t yPos, char* text, int32_t fadeAlpha);
	void DrawTintOverlay();

	void ResetParallax();
	void RenderParallaxBackground(int32_t forceRender);
	void FlushRenderQueues();

	STATIC_ASSERT(sizeof(RenderEntry) == 0x18);
}

namespace DevDraw
{
	extern int32_t g_vertexCount;

	int16_t DrawSlots();
}