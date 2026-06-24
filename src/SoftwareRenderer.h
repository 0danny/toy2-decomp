#pragma once

#include "Common.h"
#include "Numerics.h"
#include "Nu3D/Viewport.h"
#include <directx6/ddraw.h>
#include <directx6/d3d.h>

namespace SoftwareRenderer
{
	extern PointI g_unk4F7400;
	extern int32_t g_unk500A1C;
	extern int32_t g_unk830C60;
	extern int32_t g_unkE4D950;
	extern int32_t g_unk9F6008;
	extern Nu3D::Viewport::ViewportRect* g_viewportRect;
	extern int32_t g_softwareClearColor;
	extern int32_t g_leftOffset;
	extern int32_t g_rightOffset;
	extern int32_t g_topOffset;
	extern int32_t g_bottomOffset;
	extern int32_t g_bitsPerPixel;
	
	void SwapRenderBuffer();
	void SetLevelFileIndex(int32_t index);
	void InitialisePrimarySurface();
	void InitialisePrimarySurface_T();
	void Destroy();
	void ZoomOut();
	void ZoomIn();
	void PresentFrame();

	void UnkFunc67(int32_t param1, int32_t param2);
	void UnkFunc2();
	int16_t UnkFunc3();
	void UnkFunc31();
	void UnkFunc32();
	void UnkFunc33();
	void UnkFunc7();
}

namespace SoftwareDevice
{
	// Drawing Methods
	HRESULT DrawIndexedPrimitiveVB(D3DPRIMITIVETYPE primitiveType, LPDIRECT3DVERTEXBUFFER vertexBuffer, WORD* indices, DWORD indexCount, DWORD flags);
	HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE d3dptPrimitiveType,
		DWORD dwVertexTypeDesc,
		LPVOID lpvVertices,
		DWORD dwVertexCount,
		LPWORD lpwIndices,
		DWORD dwIndexCount,
		DWORD dwFlags);

	// Vertex Methods
	HRESULT ReleaseVertexBuffer(LPDIRECT3DVERTEXBUFFER buffer);
	HRESULT CreateVertexBuffer(D3DVERTEXBUFFERDESC* desc, LPDIRECT3DVERTEXBUFFER* outBuffer, DWORD flags);
	HRESULT LockVertexBuffer(LPDIRECT3DVERTEXBUFFER vertexBuffer, DWORD dwFlags, LPVOID* lplpData, DWORD* lpStride);
	HRESULT UnlockVertexBuffer(LPDIRECT3DVERTEXBUFFER buffer);
	HRESULT OptimizeVertexBuffer(LPDIRECT3DVERTEXBUFFER buffer, LPDIRECT3DDEVICE3 device, DWORD flags);
	HRESULT ProcessVerticesOnBuffer(LPDIRECT3DVERTEXBUFFER destBuffer,
		DWORD dwVertexOp,
		DWORD dwDestIndex,
		DWORD dwCount,
		LPDIRECT3DVERTEXBUFFER srcBuffer,
		DWORD dwSrcIndex,
		DWORD dwFlags);
}