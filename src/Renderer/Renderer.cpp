#include "Renderer/Renderer.h"
#include "SoftwareRenderer.h"
#include "DrawingDevice.h"
#include "Nu3D/Light.h"
#include "Nu3D/Font.h"
#include "Nu3D/BmpDataNode.h"
#include "Nu3D/Material.h"
#include "NGNLoader/NGNLoader.h"
#include "Nu3D/Patch.h"
#include "Renderer/Sprite.h"
#include "Toy2/Toy2.h"
#include "Renderer/Glue.h"

namespace Renderer
{
	// GLOBAL: TOY2 0x00884484
	int32_t g_isSoftwareRendering;

	// GLOBAL: TOY2 0x0052F2D4
	uint32_t g_frameDelta;

	// GLOBAL: TOY2 0x00884488
	int32_t g_rendererValid;

	// GLOBAL: TOY2 0x00E4D964
	LPDIRECT3D3 g_drawDeviceD3D;

	// GLOBAL: TOY2 0x00E4D968
	LPDIRECT3DDEVICE3 g_drawDeviceD3DDevice;

	// GLOBAL: TOY2 0x0088448C
	int32_t g_fogEnabled;

	// GLOBAL: TOY2 0x00508514
	float g_fogStart = 1.0;

	// GLOBAL: TOY2 0x00508518
	float g_fogEnd = 100.0;

	// GLOBAL: TOY2 0x00884478
	DWORD g_fogColor;

	// GLOBAL: TOY2 0x00E4D95C
	int32_t g_deviceBlendShadeCaps;

	// GLOBAL: TOY2 0x00884AB8
	int32_t g_vertexColorModBlue;

	// GLOBAL: TOY2 0x00884ABC
	int32_t g_vertexColorModGreen;

	// GLOBAL: TOY2 0x0088C7C8
	int32_t g_vertexColorModRed;

	// GLOBAL: TOY2 0x0094FCD8
	int32_t g_additionalRenderFlags;

	// GLOBAL: TOY2 0x0094FCD4
	float g_lodFactor;

	// GLOBAL: TOY2 0x009F5FE0
	Nu3D::Material* g_whiteMaterial;

	// GLOBAL: TOY2 0x009F5FF8
	int32_t g_unk9F5FF8;

	// GLOBAL: TOY2 0x009F6000
	int32_t g_useVertexColorMod;

	// GLOBAL: TOY2 0x009F5FB0
	Nu3D::Patch::PatchVertices g_FVF_14C_Buffer_2;

	// GLOBAL: TOY2 0x009F5FC4
	Nu3D::Patch::PatchVertices g_FVF_14C_Buffer_1;

	// GLOBAL: TOY2 0x009F2F28
	Nu3D::Patch::PatchVertices g_FVF_152_Buffer;

	// GLOBAL: TOY2 0x00E4D920
	int32_t g_renderStateCache[8];

	// GLOBAL: TOY2 0x00E4D8F8
	int32_t g_maxSimultaneousTextures;

	// GLOBAL: TOY2 0x00508704
	int32_t g_maxSimultaneousTexturesMax = 1;

	// GLOBAL: TOY2 0x00508708
	int32_t g_srcBlendMode = 5;

	// GLOBAL: TOY2 0x0050870C
	int32_t g_destBlendMode = 2;

	// GLOBAL: TOY2 0x00508710
	int32_t g_alphaBlendSrc = 1;

	// GLOBAL: TOY2 0x00508714
	int32_t g_alphaBlendDest = 4;

	// GLOBAL: TOY2 0x00830C54
	int32_t g_lastFrameTimestamp;

	// GLOBAL: TOY2 0x00830C58
	int32_t g_frameStabilityCounter;

	// GLOBAL: TOY2 0x00500AA4
	int32_t g_maxSpeedMultiplier = 4;

	// GLOBAL: TOY2 0x00500AAC
	int32_t g_startupDelayFrames = 120;

	// GLOBAL: TOY2 0x00500AA8
	int32_t g_targetSpeedMultiplier = 1;

	// GLOBAL: TOY2 0x0050851C
	uint8_t g_gammaLUT[256] = {
		// clang-format off
		0u,   1u,   2u,   3u,   4u,   5u,   6u,   7u,   8u,   9u,  10u,  11u,  12u,  13u,  14u,  15u,
		16u,  17u,  18u,  19u,  20u,  21u,  22u,  23u,  24u,  25u,  26u,  27u,  28u,  29u,  30u,  31u,
		32u,  33u,  34u,  35u,  36u,  37u,  38u,  39u,  40u,  41u,  42u,  43u,  44u,  45u,  46u,  47u,
		48u,  49u,  50u,  51u,  52u,  53u,  54u,  55u,  56u,  57u,  58u,  59u,  60u,  61u,  62u,  63u,
		64u,  65u,  66u,  67u,  68u,  69u,  70u,  71u,  72u,  73u,  74u,  75u,  76u,  77u,  78u,  79u,
		80u,  81u,  82u,  83u,  84u,  85u,  86u,  87u,  88u,  89u,  90u,  91u,  92u,  93u,  94u,  95u,
		96u,  97u,  98u,  99u, 100u, 101u, 102u, 103u, 104u, 105u, 106u, 107u, 108u, 109u, 110u, 111u,
		112u, 113u, 114u, 115u, 116u, 117u, 118u, 119u, 120u, 121u, 122u, 123u, 124u, 125u, 126u, 127u,
		128u, 129u, 130u, 131u, 132u, 133u, 134u, 135u, 136u, 137u, 138u, 139u, 140u, 141u, 142u, 143u,
		144u, 145u, 146u, 147u, 148u, 149u, 150u, 151u, 152u, 153u, 154u, 155u, 156u, 157u, 158u, 159u,
		160u, 161u, 162u, 163u, 164u, 165u, 166u, 167u, 168u, 169u, 170u, 171u, 172u, 173u, 174u, 175u,
		176u, 177u, 178u, 179u, 180u, 181u, 182u, 183u, 184u, 185u, 186u, 187u, 188u, 189u, 190u, 191u,
		192u, 193u, 194u, 195u, 196u, 197u, 198u, 199u, 200u, 201u, 202u, 203u, 204u, 205u, 206u, 207u,
		208u, 209u, 210u, 211u, 212u, 213u, 214u, 215u, 216u, 217u, 218u, 219u, 220u, 221u, 222u, 223u,
		224u, 225u, 226u, 227u, 228u, 229u, 230u, 231u, 232u, 233u, 234u, 235u, 236u, 237u, 238u, 239u,
		240u, 241u, 242u, 243u, 244u, 245u, 246u, 247u, 248u, 249u, 250u, 251u, 252u, 253u, 254u, 255u,
		// clang-format on
	};

	// GLOBAL: TOY2 0x005087F4
	int32_t g_drawMaterialBuckets = 1;

	// GLOBAL: TOY2 0x005087F8
	int32_t g_drawTransparentBuckets = 1;

	// GLOBAL: TOY2 0x004F7414
	float g_virtualScreenWidth = 512.0;

	// GLOBAL: TOY2 0x004F7418
	float g_virtualScreenHeight = 256.0;

	// GLOBAL: TOY2 0x00A4CC90
	Nu3D::Material* g_boundMaterial;

	// GLOBAL: TOY2 0x00AAD778
	int32_t g_boundTextureIndices[8];

	// GLOBAL: TOY2 0x00500A54
	int32_t g_drawParallaxTexture = 1;

	// GLOBAL: TOY2 0x0072EF90
	float g_parallaxHorizOffset;

	// GLOBAL: TOY2 0x00830C50
	float g_parallaxCurHorizScroll;

	// GLOBAL: TOY2 0x00731CD0
	float g_parallaxTexHeightRatio;

	// GLOBAL: TOY2 0x00731CD4
	float g_parallaxTexWidthRatio;

	// GLOBAL: TOY2 0x00731D68
	RGBA g_parallaxTexFirstPixel;

	// GLOBAL: TOY2 0x00731CC8
	RGBA g_parallaxTexLastPixel;
}

namespace DrawingAPI
{
	// GLOBAL: TOY2 0x00508724
	Device_DrawIndexedPrimitive* DrawIndexedPrimitive;

	// GLOBAL: TOY2 0x00508720
	Device_DrawIndexedPrimitiveVB* DrawIndexedPrimitiveVB;

	// GLOBAL: TOY2 0x005084FC
	Device_ReleaseVertexBuffer* ReleaseVertexBuffer;

	// GLOBAL: TOY2 0x00508500
	Device_CreateVertexBuffer* CreateVertexBuffer;

	// GLOBAL: TOY2 0x00508504
	Device_LockVertexBuffer* LockVertexBuffer;

	// GLOBAL: TOY2 0x00508508
	Device_UnlockVertexBuffer* UnlockVertexBuffer;

	// GLOBAL: TOY2 0x0050850C
	Device_OptimizeVertexBuffer* OptimizeVertexBuffer;

	// GLOBAL: TOY2 0x00508510
	Device_ProcessVerticesOnBuffer* ProcessVerticesOnBuffer;

	// FUNCTION: TOY2 0x004B2BF0
	void SetVertexAPIs(int32_t isSoftwareRendering)
	{
		if (isSoftwareRendering)
		{
			ReleaseVertexBuffer = SoftwareDevice::ReleaseVertexBuffer;
			CreateVertexBuffer = SoftwareDevice::CreateVertexBuffer;
			LockVertexBuffer = SoftwareDevice::LockVertexBuffer;
			UnlockVertexBuffer = SoftwareDevice::UnlockVertexBuffer;
			OptimizeVertexBuffer = SoftwareDevice::OptimizeVertexBuffer;
			ProcessVerticesOnBuffer = SoftwareDevice::ProcessVerticesOnBuffer;
		}
		else
		{
			ReleaseVertexBuffer = HardwareDevice::ReleaseVertexBuffer;
			CreateVertexBuffer = HardwareDevice::CreateVertexBuffer;
			LockVertexBuffer = HardwareDevice::LockVertexBuffer;
			UnlockVertexBuffer = HardwareDevice::UnlockVertexBuffer;
			OptimizeVertexBuffer = HardwareDevice::OptimizeVertexBuffer;
			ProcessVerticesOnBuffer = HardwareDevice::ProcessVerticesOnBuffer;
		}
	}
}

namespace Renderer
{
	// STUB: TOY2 0x004B5CE0
	int32_t Set508718(int32_t value) { return 0; }

	// FUNCTION: TOY2 0x004B2CE0
	void DisableFog() { g_fogEnabled = 0; }

	// FUNCTION: TOY2 0x004B6320
	void SetRenderState(int32_t newStateFlags)
	{
		if (g_isSoftwareRendering)
		{
			g_renderStateCache[0] = newStateFlags;
		}
		else
		{
			uint32_t flagsToDisable = g_renderStateCache[0] & (newStateFlags ^ g_renderStateCache[0]);

			if (flagsToDisable)
			{
				if ((flagsToDisable & RENDER_ALPHA_DEFAULT) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);

				if ((flagsToDisable & RENDER_ALPHA_CUSTOM) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);

				if ((flagsToDisable & RENDER_ALPHA_ALT) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);

				if ((flagsToDisable & RENDER_ALPHA_TEX_MODULATE) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);

				if ((flagsToDisable & RENDER_ALPHA_TEX_MOD_CUSTOM) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);

				if ((flagsToDisable & RENDER_ALPHA_TEX_MOD_ALT) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);

				if ((flagsToDisable & RENDER_Z) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ZENABLE, 1);

				if ((flagsToDisable & RENDER_DISABLE_PERSPECTIVE) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 1);

				if ((flagsToDisable & RENDER_BILINEAR_FILTER) != 0)
				{
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMAG, 1);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMIN, 1);
				}

				if ((flagsToDisable & RENDER_ZWRITE) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1);

				if ((flagsToDisable & RENDER_COLOR_VERTEX) != 0)
					DrawingDevice::SetLightState(D3DLIGHTSTATE_COLORVERTEX, 0);

				if ((flagsToDisable & RENDER_ZBIAS_1) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ZBIAS, 0);

				if ((flagsToDisable & RENDER_ZBIAS_2) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ZBIAS, 0);
			}

			uint32_t flagsToEnable = newStateFlags & (newStateFlags ^ g_renderStateCache[0]);

			if (flagsToEnable)
			{
				if ((flagsToEnable & RENDER_Z) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ZENABLE, 0);

				if ((flagsToEnable & RENDER_ZWRITE) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 0);

				if ((flagsToEnable & RENDER_DISABLE_PERSPECTIVE) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 0);

				if ((flagsToEnable & RENDER_CULL_BACK) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_CULLMODE, 2);

				if ((flagsToEnable & RENDER_CULL_FRONT) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_CULLMODE, 3);

				if ((flagsToEnable & RENDER_CULL_NONE) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_CULLMODE, 1);

				if ((flagsToEnable & RENDER_ALPHA_DEFAULT) != 0)
				{
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, 4);
				}

				if ((flagsToEnable & RENDER_ALPHA_CUSTOM) != 0)
				{
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_SRCBLEND, g_srcBlendMode);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_DESTBLEND, g_destBlendMode);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, 4);
				}

				if ((flagsToEnable & RENDER_ALPHA_ALT) != 0)
				{
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_SRCBLEND, g_alphaBlendSrc);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_DESTBLEND, g_alphaBlendDest);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, 4);
				}

				if ((flagsToEnable & RENDER_ALPHA_TEX_MODULATE) != 0)
				{
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, 3);
				}

				if ((flagsToEnable & RENDER_ALPHA_TEX_MOD_CUSTOM) != 0)
				{
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_SRCBLEND, g_srcBlendMode);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_DESTBLEND, g_destBlendMode);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, 3);
				}

				if ((flagsToEnable & RENDER_ALPHA_TEX_MOD_ALT) != 0)
				{
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_SRCBLEND, g_alphaBlendSrc);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_DESTBLEND, g_alphaBlendDest);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, 3);
				}

				if ((flagsToEnable & RENDER_BILINEAR_FILTER) != 0)
				{
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMAG, 2);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREMIN, 2);
				}

				if ((flagsToEnable & RENDER_COLOR_VERTEX) != 0)
				{
					DrawingDevice::SetLightState(D3DLIGHTSTATE_COLORVERTEX, 1);
					DrawingDevice::SetRenderState(D3DRENDERSTATE_SHADEMODE, 2);
				}

				if ((flagsToEnable & RENDER_ZBIAS_1) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ZBIAS, 1);

				if ((flagsToEnable & RENDER_ZBIAS_2) != 0)
					DrawingDevice::SetRenderState(D3DRENDERSTATE_ZBIAS, 2);
			}

			g_renderStateCache[0] = newStateFlags;
		}
	}

	// FUNCTION: TOY2 0x004B6660
	void SetTextureStageState(int32_t newState, DWORD textureStage)
	{
		uint32_t stateFlagsToDisable = g_renderStateCache[textureStage + 1] & (newState ^ g_renderStateCache[textureStage + 1]);

		if (stateFlagsToDisable)
		{
			if ((stateFlagsToDisable & (RENDER_TEXTURE_WRAP_UV | RENDER_TEXTURE_CLAMP_U)) != 0)
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_ADDRESS, 1);

			if ((stateFlagsToDisable & (RENDER_COLOR_MODULATE | RENDER_COLOR_BLEND_FACTOR)) != 0)
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_COLOROP, 1);
		}

		uint32_t flagsToEnable = newState & (newState ^ g_renderStateCache[textureStage + 1]);

		if (flagsToEnable)
		{
			if ((flagsToEnable & RENDER_TEXTURE_WRAP_UV) != 0)
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_ADDRESS, 3);

			if ((flagsToEnable & RENDER_TEXTURE_CLAMP_U) != 0)
			{
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_ADDRESS, 2);
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_ADDRESS, 4);
			}

			if ((flagsToEnable & RENDER_COLOR_MODULATE) != 0)
			{
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_COLOROP, 4);
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_COLORARG1, 2);
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_COLORARG2, 1);
			}

			if ((flagsToEnable & RENDER_COLOR_BLEND_FACTOR) != 0)
			{
				DrawingDevice::SetRenderState(D3DRENDERSTATE_TEXTUREFACTOR, 0x80FFFFFF);
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_COLOROP, 14);
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_COLORARG1, 2);
				DrawingDevice::SetTextureStageState(textureStage, D3DTSS_COLORARG2, 1);
			}
		}

		g_renderStateCache[textureStage + 1] = newState;
	}

	// FUNCTION: TOY2 0x004B6850
	void InitRenderState(int32_t newStage)
	{
		SetRenderState(newStage | RENDER_COLOR_MODULATE);
		SetTextureStageState(newStage | RENDER_COLOR_MODULATE, 0);

		for (int32_t texture = 1; texture < g_maxSimultaneousTextures; ++texture)
			SetTextureStageState(0, texture);
	}

	// STUB: TOY2 0x004B62C0
	void ResetRenderPools() {}

	// FUNCTION: TOY2 0x004B9710
	void InitResources()
	{
		g_vertexColorModBlue = 4096;
		g_vertexColorModGreen = 4096;
		g_vertexColorModRed = 4096;
		g_additionalRenderFlags = 0;
		g_lodFactor = 1.0;

		RGBColor whiteColor;
		whiteColor.b = 1.0;
		whiteColor.g = 1.0;
		whiteColor.r = 1.0;

		g_whiteMaterial = Nu3D::Material::CreateFromColor(&whiteColor);

		g_unk9F5FF8 = 0;
		g_useVertexColorMod = 0;

		SoftwareRenderer::g_viewportRect = 0;

		Set508718(5);

		g_FVF_14C_Buffer_2.format = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1;
		g_FVF_14C_Buffer_2.vertexCount = 1000;
		g_FVF_14C_Buffer_2.vertexBuffer = 0;
		g_FVF_14C_Buffer_2.data.verticesTL = (Nu3D::VertexTL*)malloc(sizeof(Nu3D::VertexTL) * 1000);

		Nu3D::Patch::PatchVertices::CreateVertexBuffer(&g_FVF_14C_Buffer_2, 12);

		g_FVF_14C_Buffer_1.format = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1;
		g_FVF_14C_Buffer_1.vertexCount = 1000;
		g_FVF_14C_Buffer_1.vertexBuffer = 0;
		g_FVF_14C_Buffer_1.data.verticesTL = (Nu3D::VertexTL*)malloc(sizeof(Nu3D::VertexTL) * 1000);

		Nu3D::Patch::PatchVertices::CreateVertexBuffer(&g_FVF_14C_Buffer_1, 2);

		g_FVF_152_Buffer.format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
		g_FVF_152_Buffer.vertexCount = 1000;
		g_FVF_152_Buffer.vertexBuffer = 0;
		g_FVF_152_Buffer.data.vertices = (Nu3D::Vertex*)malloc(sizeof(Nu3D::Vertex) * 1000);

		Nu3D::Patch::PatchVertices::CreateVertexBuffer(&g_FVF_152_Buffer, 14);

		D3DDEVICEDESC outSurfaceDesc;
		memcpy(&outSurfaceDesc, DrawingDevice::CopySurfaceDesc(&outSurfaceDesc), sizeof(outSurfaceDesc));

		memset(g_renderStateCache, 0, sizeof(g_renderStateCache));

		g_maxSimultaneousTextures = outSurfaceDesc.wMaxSimultaneousTextures;

		if (outSurfaceDesc.wMaxSimultaneousTextures >= g_maxSimultaneousTexturesMax)
			g_maxSimultaneousTextures = g_maxSimultaneousTexturesMax;

		if ((outSurfaceDesc.dpcTriCaps.dwShadeCaps & 0x4000) != 0)
		{
			if ((outSurfaceDesc.dpcTriCaps.dwDestBlendCaps & 2) == 0)
				g_destBlendMode = 6;

			if ((outSurfaceDesc.dpcTriCaps.dwDestBlendCaps & 8) == 0)
			{
				g_alphaBlendSrc = 5;
				g_alphaBlendDest = 6;
			}
		}
		else
		{
			g_srcBlendMode = 2;
			g_destBlendMode = 2;

			if ((outSurfaceDesc.dpcTriCaps.dwDestBlendCaps & 8) == 0)
			{
				g_alphaBlendSrc = 5;
				g_alphaBlendDest = 6;
			}
		}

		InitRenderState(3078);
		InitRenderState(4752);

		DrawingAPI::SetVertexAPIs(g_isSoftwareRendering);

		if (g_isSoftwareRendering == 1)
		{
			DrawingAPI::DrawIndexedPrimitiveVB = SoftwareDevice::DrawIndexedPrimitiveVB;
			DrawingAPI::DrawIndexedPrimitive = SoftwareDevice::DrawIndexedPrimitive;
		}
		else
		{
			DrawingAPI::DrawIndexedPrimitiveVB = HardwareDevice::DrawIndexedPrimitiveVB;
			DrawingAPI::DrawIndexedPrimitive = HardwareDevice::DrawIndexedPrimitive;
		}

		ResetRenderPools();
		Renderer::Sprite::ResetQueue();

		SoftwareRenderer::InitialisePrimarySurface();

		DECOMP_PRINT(("Finished Renderer::InitResources"));
	}

	// STUB: TOY2 0x004B37F0
	void Cleanup() {}

	// FUNCTION: TOY2 0x004B3630
	void Init()
	{
		if (g_rendererValid)
			Cleanup();

		g_drawDeviceD3D = DrawingDevice::GetD3D();
		g_drawDeviceD3DDevice = DrawingDevice::GetD3DDevice();

		Nu3D::g_primListHead = 0;
		Nu3D::g_patchListHead = 0;

		Nu3D::SetIsSoftwareRendering(g_isSoftwareRendering);
		Nu3D::SetMinTexSize(g_isSoftwareRendering != 0 ? 256 : 0);

		Nu3D::Viewport::Init();
		Nu3D::Light::InitPool(16);
		NGNLoader::Init();
		Nu3D::Material::Init();

		InitResources();
		g_rendererValid = 1;
		Nu3D::Font::BuildFontTextures();
		DisableFog();

		g_deviceBlendShadeCaps = 0;

		D3DDEVICEDESC outSurfaceDesc;
		memcpy(&outSurfaceDesc, DrawingDevice::CopySurfaceDesc(&outSurfaceDesc), sizeof(outSurfaceDesc));

		if ((outSurfaceDesc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAGOURAUDBLEND) != 0)
			g_deviceBlendShadeCaps |= 1;

		if ((outSurfaceDesc.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_ONE) != 0)
			g_deviceBlendShadeCaps |= 2;

		if ((outSurfaceDesc.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_INVSRCCOLOR) != 0)
			g_deviceBlendShadeCaps |= 4;

		if ((outSurfaceDesc.dpcTriCaps.dwTextureBlendCaps & D3DPTEXTURECAPS_TRANSPARENCY) != 0)
			g_deviceBlendShadeCaps |= 8;
	}

	// FUNCTION: TOY2 0x004B3860
	void SetIsSoftwareRendering(int32_t value) { g_isSoftwareRendering = value; }

	// FUNCTION: TOY2 0x00453CD0
	void SetVirtualRatioTo54()
	{
		g_virtualScreenWidth = 320.0;
		g_virtualScreenHeight = 256.0;
	}

	// FUNCTION: TOY2 0x00490860 [MATCHED]
	void DoFrameDelay(int32_t isGameplayFrame)
	{
		// This method does the FPS delay for each frame, it is the source of all pain
		int32_t hrt = Nu3D::GetHighResolutionTime();
		int32_t elapsedMs = hrt - g_lastFrameTimestamp;

		if (Toy2::g_demoMode)
		{
			g_frameDelta = 2;

			if (elapsedMs < 33)
			{
				Nu3D::PrecisionSleep(33 - elapsedMs);
				g_lastFrameTimestamp = Nu3D::GetHighResolutionTime();
				return;
			}

			goto LBL_UPDATE_TIMESTAMP;
		}

		{
			int32_t calculatedMultiplier = 60 * elapsedMs / 1000;
			g_frameDelta = calculatedMultiplier;

			if (1000 * calculatedMultiplier / 60 != elapsedMs)
				g_frameDelta = ++calculatedMultiplier;

			int32_t minMultiplier;

			if (calculatedMultiplier < 1)
				minMultiplier = 1;
			else
				minMultiplier = calculatedMultiplier;

			if (g_maxSpeedMultiplier < minMultiplier)
			{
				calculatedMultiplier = g_maxSpeedMultiplier;
			}
			else
			{
				if (calculatedMultiplier >= 1)
					goto LBL_STARTUP_DELAY;

				calculatedMultiplier = 1;
			}

			g_frameDelta = calculatedMultiplier;

		LBL_STARTUP_DELAY:

			int32_t delayFrames = g_startupDelayFrames;
			if (g_startupDelayFrames > 0)
			{
				delayFrames = g_startupDelayFrames - calculatedMultiplier;
				g_startupDelayFrames -= calculatedMultiplier;
			}

			if (isGameplayFrame)
			{
				if (delayFrames > 0)
					goto LBL_WAIT_NEXT_FRAME;

				int32_t stabilityCounter = g_frameStabilityCounter;
				int32_t targetSpeedMultiplier = g_targetSpeedMultiplier;

				if (g_frameStabilityCounter > 0)
				{
					stabilityCounter = g_frameStabilityCounter - targetSpeedMultiplier;
					g_frameStabilityCounter -= targetSpeedMultiplier;
				}

				if (calculatedMultiplier < targetSpeedMultiplier)
				{
					if (stabilityCounter > 0)
					{
						calculatedMultiplier = targetSpeedMultiplier;
						targetSpeedMultiplier = calculatedMultiplier;
						g_frameDelta = calculatedMultiplier;
					}
				}
				else
				{
					g_frameStabilityCounter = 60;
				}

				g_targetSpeedMultiplier = calculatedMultiplier;
			}
			else
			{
				g_targetSpeedMultiplier = 1;
				g_frameStabilityCounter = 0;
				g_startupDelayFrames = 120;
			}

		LBL_WAIT_NEXT_FRAME:

			int32_t sleepTimeMs = 1000 * calculatedMultiplier / 60 - elapsedMs;

			if (sleepTimeMs > 0)
				Nu3D::PrecisionSleep(sleepTimeMs);
		}

	LBL_UPDATE_TIMESTAMP:

		g_lastFrameTimestamp = Nu3D::GetHighResolutionTime();
	}

	// FUNCTION: TOY2 0x004C2080
	int32_t ConvertRGBATo16Bit(RGBA color) { return 0; }

	// FUNCTION: TOY2 0x004B37B0
	RGBA ApplyGammaCorrection(RGBA color)
	{
		color.b = g_gammaLUT[color.b];
		color.g = g_gammaLUT[color.g];
		color.r = g_gammaLUT[color.r];

		return color;
	}

	// FUNCTION: TOY2 0x004B2C80
	void ClearScreen(RGBA clearColor, int32_t clearFlags)
	{
		RGBA color = ApplyGammaCorrection(clearColor);

		if (g_isSoftwareRendering)
		{
			uint16_t clearColor = Renderer::ConvertRGBATo16Bit(color);
			SoftwareRenderer::g_softwareClearColor = (clearColor << 16) | clearColor;
		}
		else
		{
			DrawingDevice::ClearScreen(clearFlags, color.value);
		}
	}

	// FUNCTION: TOY2 0x004B2D80
	void ApplyFogSettings()
	{
		if (g_fogEnabled)
		{
			DrawingDevice::SetRenderState(D3DRENDERSTATE_FOGENABLE, 1);
			DrawingDevice::SetRenderState(D3DRENDERSTATE_FOGCOLOR, g_fogColor & 0xFFFFFF);

			DrawingDevice::SetLightState(D3DLIGHTSTATE_FOGMODE, 3);
			DrawingDevice::SetLightState(D3DLIGHTSTATE_FOGSTART, g_fogStart);
			DrawingDevice::SetLightState(D3DLIGHTSTATE_FOGEND, g_fogEnd);
		}
		else
		{
			DrawingDevice::SetRenderState(D3DRENDERSTATE_FOGENABLE, 0);
		}
	}

	// FUNCTION: TOY2 0x004B2D50
	int32_t BeginScene()
	{
		if (! DrawingDevice::BeginScene())
		{
			Renderer::ApplyFogSettings();
			Nu3D::Viewport::Reset();
			Nu3D::Viewport::SetViewClipRect();

			if (g_isSoftwareRendering)
				SoftwareRenderer::UnkFunc32();

			return 1;
		}

		return 0;
	}

	// FUNCTION: TOY2 0x004B2DE0
	void EndScene(int32_t presentFrame)
	{
		DrawingDevice::EndScene();

		if (presentFrame)
		{
			if (g_isSoftwareRendering)
			{
				SoftwareRenderer::PresentFrame();
			}
			else if (DrawingDevice::PresentFrame() == DDERR_SURFACELOST)
			{
				DrawingDevice::CD3DFramework* device = DrawingDevice::g_drawingDevice;

				LPDIRECTDRAWSURFACE4 frontBuffer = DrawingDevice::g_drawingDevice->m_pddsFrontBuffer;

				if (frontBuffer && frontBuffer->IsLost())
					device->m_pddsFrontBuffer->Restore();

				LPDIRECTDRAWSURFACE4 backBuffer = device->m_pddsBackBuffer;

				if (backBuffer && backBuffer->IsLost())
					device->m_pddsBackBuffer->Restore();

				LPDIRECTDRAWSURFACE4 zBuffer = device->m_pddsZBuffer;

				if (zBuffer)
				{
					if (zBuffer->IsLost())
						device->m_pddsZBuffer->Restore();
				}
			}
		}
	}

	// FUNCTION: TOY2 0x0049B580
	void DrawMainMenuText(int16_t yPos, char* text, int32_t fadeAlpha)
	{
		char* charPtr = text;
		int32_t strLength = 0;

		if (*text)
			while (text[++strLength]) {};

		int16_t xPos = 160 - 6 * strLength;

		if (strLength > 0)
		{
			int32_t remaining = strLength;

			do
			{
				char currentChar = *charPtr++;

				if (currentChar != ' ')
				{
					uint8_t tileIndex;

					if (currentChar == '\'')
						tileIndex = 47;
					else
						tileIndex = currentChar - 97;

					if (fadeAlpha == 128)
						Sprite::DrawScaled(xPos, yPos, 50, tileIndex, 128u, 128u, 128u, 255, 2048, 2048);
					else
						Sprite::DrawScaled(xPos, yPos, 50, tileIndex, 255u, 255u, 255u, (fadeAlpha << 9) + 96, 2048, 2048);
				}

				xPos += 12;
				--remaining;

			} while (remaining);
		}
	}

	// STUB: TOY2 0x0044DD80
	void DrawTintOverlay() {}

	// STUB: TOY2 0x0048F3E0
	void ResetParallax() {}

	// FUNCTION: TOY2 0x004B3870
	int32_t GetIsSoftwareRendering() { return g_isSoftwareRendering; }

	// FUNCTION: TOY2 0x0048F410
	void RenderParallaxBackground(int32_t forceRender)
	{
		if (g_drawParallaxTexture)
		{
			if (Toy2::g_hasStaticBackdrop)
			{
				g_parallaxHorizOffset = 0.0;
				g_parallaxCurHorizScroll = 0.0;
				g_parallaxTexHeightRatio = 1.0;
				g_parallaxTexWidthRatio = 1.0;
			}
			else if (! Toy2::g_hasBackdrop)
			{
				return;
			}

			if (forceRender || GetIsSoftwareRendering() || (Glue::SetBackdrop(Toy2::g_nextBackdropId), ! Glue::BackdropBltFast()))
			{
				int32_t texIndex = NGNLoader::GetTextureDataIndex(Toy2::g_nextBackdropId);

				if (texIndex)
				{
					Vector2F uvMax;
					Vector2F uvMin;

					uvMin.y = 0.0;
					uvMin.x = 0.0;

					uvMax.y = 1.0;
					uvMax.x = 1.0;

					if (g_parallaxHorizOffset > 0.0)
					{
						float topFillHeight = 1.0 / g_virtualScreenHeight + g_parallaxHorizOffset;
						Sprite::Queue2DSprite(0.0, 0.0, 1.0, topFillHeight, &uvMin, &uvMax, 0, g_parallaxTexLastPixel, RENDER_PARALLAX_BG);
					}

					double verticalExtent = g_parallaxTexHeightRatio + g_parallaxHorizOffset;

					if (verticalExtent < 1.0)
					{
						float bottomFillHeight = 1.0 - g_parallaxHorizOffset - g_parallaxTexHeightRatio + 1.0 / g_virtualScreenHeight;
						float verticalEnd = verticalExtent;

						Sprite::Queue2DSprite(
							0.0, verticalEnd, 1.0, bottomFillHeight, &uvMin, &uvMax, 0, g_parallaxTexFirstPixel, RENDER_PARALLAX_BG);
					}

					double nextHorizontalPos;

					do
					{
						RGBA color;
						color.value = -1;

						Sprite::Queue2DSprite(g_parallaxCurHorizScroll,
							g_parallaxHorizOffset,
							g_parallaxTexWidthRatio,
							g_parallaxTexHeightRatio,
							&uvMin,
							&uvMax,
							texIndex,
							color,
							RENDER_PARALLAX_BG);

						nextHorizontalPos = g_parallaxTexWidthRatio + g_parallaxCurHorizScroll;

						g_parallaxCurHorizScroll = nextHorizontalPos;

					} while (nextHorizontalPos < 1.0);
				}
			}
		}
	}

	// FUNCTION: TOY2 0x004B8400
	void FlushMaterialBuckets() {}

	// FUNCTION: TOY2 0x004B6A90
	void FlushTransparentBuckets() {}

	// FUNCTION: TOY2 0x004B5CF0
	void FlushPrimitives() {}

	// FUNCTION: TOY2 0x004B6A50
	void FlushRenderQueues()
	{
		if (g_drawMaterialBuckets)
			FlushMaterialBuckets();

		if (g_drawTransparentBuckets)
			FlushTransparentBuckets();

		FlushPrimitives();
		ResetRenderPools();

		if (g_isSoftwareRendering)
		{
			SoftwareRenderer::UnkFunc33();
			SoftwareRenderer::UnkFunc32();
		}
	}

	// FUNCTION: TOY2 0x004B8BF0
	RGBA ModulateColorByAlpha(RGBA color, int32_t flags)
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;

		if (((flags & 0x4000) != 0 || flags == 0x20000000) && g_srcBlendMode == 2)
		{
			blue = (color.a * color.b) >> 8;
			red = (color.a * color.r) >> 8;
			green = (color.a * color.g) >> 8;

			color.b = blue;
			color.r = red;
			color.g = green;
		}
		else
		{
			blue = color.b;
			green = color.g;
			red = color.r;
		}

		if (g_alphaBlendDest != 6 || flags != 0x40000000)
			return color;

		color.a = green;

		if (blue == green && blue == red)
		{
			color.b = 0;
			color.g = 0;
			color.r = 0;
			return color;
		}

		color.b = 255 - blue;
		color.g = 255 - green;
		color.r = 255 - red;

		return color;
	}

	// FUNCTION: TOY2 0x004C2870
	void BindTexture(int32_t texIndex)
	{
		g_boundMaterial = 0;

		if (g_boundTextureIndices[0] != texIndex)
		{
			g_boundTextureIndices[0] = texIndex;
			DrawingDevice::BindTexWithStage(texIndex, 0);
		}

		for (int32_t idx = 1; idx < g_maxSimultaneousTextures; ++idx)
		{
			if (g_boundTextureIndices[0] != texIndex)
			{
				g_boundTextureIndices[0] = -1;
				DrawingDevice::BindTexWithStage(-1, idx);
			}
		}
	}
}

namespace DevDraw
{
	// GLOBAL: TOY2 0x00732FBC
	int32_t g_vertexCount;

	// STUB: TOY2 0x004907E0
	int16_t DrawSlots() { return 0; }
}