#pragma once

#include "Common.h"
#include <directx6/ddraw.h>
#include <directx6/d3d.h>

namespace Renderer
{
	struct RenderEntry;
}

namespace Nu3D
{
	struct Material
	{
		Material* next;
		Material* prev;
		Material* alwaysZero;
		int32_t id;
		D3DMATERIAL d3dMaterial;
		LPD3DMATERIALHANDLE d3dMaterialHandle;
		LPDIRECT3DMATERIAL3 direct3DMat3;
		int32_t texDataIndex;
		float opacity;
		Renderer::RenderEntry* renderEntryHead;
		int32_t metadata;
		int32_t originalMetadata;
		float horzOffset;
		float vertOffset;

		static void Init();
		static Material* CreateFromColor(RGBColor *rgbColor);
	};


	STATIC_ASSERT(sizeof(Material) == 0x84);
}