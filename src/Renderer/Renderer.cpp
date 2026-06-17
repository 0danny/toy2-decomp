#include "Renderer/Renderer.h"
#include "DrawingDevice.h"
#include "Nu3D/Light.h"
#include "Nu3D/Font.h"
#include "Nu3D/BmpDataNode.h"
#include "NGNLoader/NGNLoader.h"

namespace Renderer
{
	// GLOBAL: TOY2 0x00884484
	int32_t g_isSoftwareRendering;

	// GLOBAL: TOY2 0x00884488
	int32_t g_rendererValid;

	// GLOBAL: TOY2 0x00E4D964
	LPDIRECT3D3 g_drawDeviceD3D;

	// GLOBAL: TOY2 0x00E4D968
	LPDIRECT3DDEVICE3 g_drawDeviceD3DDevice;

	// GLOBAL: TOY2 0x0088448C
	int32_t g_fogEnabled;

	// GLOBAL: TOY2 0x00E4D95C
	int32_t g_deviceBlendShadeCaps;

	// FUNCTION: TOY2 0x004B2CE0
	void DisableFog() { g_fogEnabled = 0; }

	// STUB: TOY2 0x004B9710
	void InitResources() {}

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

		if (outSurfaceDesc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAGOURAUDBLEND)
			g_deviceBlendShadeCaps |= 1;

		if (outSurfaceDesc.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_ONE)
			g_deviceBlendShadeCaps |= 2;

		if (outSurfaceDesc.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_INVSRCCOLOR)
			g_deviceBlendShadeCaps |= 4;

		if (outSurfaceDesc.dpcTriCaps.dwTextureBlendCaps & D3DPTEXTURECAPS_TRANSPARENCY)
			g_deviceBlendShadeCaps |= 8;
	}

	// FUNCTION: TOY2 0x004B3860
	void SetIsSoftwareRendering(int32_t value) { g_isSoftwareRendering = value; }

	// STUB: TOY2 0x00453CD0
	void SetVirtualRatioTo54() {}
}