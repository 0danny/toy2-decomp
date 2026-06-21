#include "Nu3D/Viewport.h"
#include "DrawingDevice.h"
#include "Renderer/Renderer.h"
#include "SoftwareRenderer.h"
#include <directx6/ddraw.h>
#include <directx6/d3d.h>

#include <STDIO.H>

namespace Nu3D
{
	namespace Viewport
	{
		// GLOBAL: TOY2 0x00884640
		D3DVIEWPORT2 g_pendingViewport;

		// GLOBAL: TOY2 0x00884680
		D3DVIEWPORT2 g_currentViewport;

		// GLOBAL: TOY2 0x00884638
		int32_t g_renderWidth;

		// GLOBAL: TOY2 0x00884634
		int32_t g_renderHeight;

		// GLOBAL: TOY2 0x008846AC
		float g_clipOffsetX;

		// GLOBAL: TOY2 0x008846B0
		float g_clipOffsetY;

		// GLOBAL: TOY2 0x00884630
		int32_t g_clipNormMatrixDirty;

		// GLOBAL: TOY2 0x008846B4
		int32_t g_screenSpaceMatrixDirty;

		// GLOBAL: TOY2 0x00884670
		float g_currentViewportX;

		// GLOBAL: TOY2 0x00884674
		float g_currentViewportY;

		// GLOBAL: TOY2 0x008845A0
		float g_minViewportY;

		// GLOBAL: TOY2 0x008845A4
		float g_minViewportX;

		// GLOBAL: TOY2 0x00884678
		DWORD g_currentViewportWidth;

		// GLOBAL: TOY2 0x0088466C
		DWORD g_currentViewportHeight;

		// GLOBAL: TOY2 0x00E4D958
		int32_t g_viewportChangeCount;

		// GLOBAL: TOY2 0x008845A8
		int32_t g_drawDeviceWidth;

		// GLOBAL: TOY2 0x008845AC
		int32_t g_drawDeviceHeight;

		// GLOBAL: TOY2 0x009F6224
		FustrumInfo g_fustrumInfo;

		// GLOBAL: TOY2 0x00E4D8E8
		ViewportRectAlt g_viewClipRect;

		// FUNCTION: TOY2 0x004B55D0
		void Init()
		{
			DrawingDevice::BuildFreshViewport(&g_pendingViewport);

			memcpy(&g_currentViewport, &g_pendingViewport, sizeof(g_currentViewport));

			g_renderWidth = DrawingDevice::GetWidth();
			g_renderHeight = DrawingDevice::GetHeight();

			g_clipOffsetY = 0.0;
			g_clipOffsetX = 0.0;
			g_clipNormMatrixDirty = 1;
			g_screenSpaceMatrixDirty = 1;
		}

		// FUNCTION: TOY2 0x004B5630
		void Reset()
		{
			DrawingDevice::SetViewport(&g_currentViewport);

			memcpy(&g_pendingViewport, &g_currentViewport, sizeof(g_pendingViewport));

			g_currentViewportX = g_currentViewport.dwX;
			g_currentViewportY = g_currentViewport.dwY;

			g_minViewportY = 0.0;

			g_currentViewportWidth = g_currentViewport.dwWidth;

			g_minViewportX = 0.0;
			g_clipOffsetY = 0.0;
			g_clipOffsetX = 0.0;
			g_clipNormMatrixDirty = 1;

			g_currentViewportHeight = g_currentViewport.dwHeight;

			g_screenSpaceMatrixDirty = 1;
			g_viewportChangeCount = 0;

			g_drawDeviceWidth = g_renderWidth;
			g_drawDeviceHeight = g_renderHeight;
		}

		// FUNCTION: TOY2 0x004BAC40
		void GetViewClipRect(ViewportRect* output)
		{
			output->top = g_viewClipRect.top;
			output->bottom = g_viewClipRect.left;
			output->left = g_viewClipRect.bottom;
			output->right = g_viewClipRect.right;
		}

		// FUNCTION: TOY2 0x004B5590
		void GetViewportRect(ViewportRectAlt* output)
		{
			output->left = g_currentViewportX;
			output->top = g_currentViewportY;
			output->right = g_currentViewportWidth + g_currentViewportX - 1.0;
			output->bottom = g_currentViewportHeight + g_currentViewportY - 1.0;
		}

		// FUNCTION: TOY2 0x004BA3A0
		void SetViewClipRect()
		{
			ViewportRectAlt spriteRect;

			if (Renderer::g_isSoftwareRendering)
			{
				g_viewClipRect.top = SoftwareRenderer::g_topOffset;
				g_viewClipRect.left = SoftwareRenderer::g_leftOffset;
				g_viewClipRect.bottom = SoftwareRenderer::g_bottomOffset;
				g_viewClipRect.right = SoftwareRenderer::g_rightOffset;
			}
			else
			{
				GetViewportRect(&spriteRect);
				g_viewClipRect.top = spriteRect.left;
				g_viewClipRect.left = spriteRect.top;
				g_viewClipRect.bottom = spriteRect.right;
				g_viewClipRect.right = spriteRect.bottom;
			}
		}
	}
}