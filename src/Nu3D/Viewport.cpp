#include "Nu3D/Viewport.h"
#include "DrawingDevice.h"
#include <directx7/d3d.h>

namespace Nu3D
{
	namespace Viewport
	{
		// $GLOBAL 00884640
		D3DVIEWPORT2 g_pendingViewport;

		// $GLOBAL 00884680
		D3DVIEWPORT2 g_currentViewport;

		// $GLOBAL 00884638
		int32_t g_renderWidth;

		// $GLOBAL 00884634
		int32_t g_renderHeight;

		// $GLOBAL 008846AC
		float g_clipOffsetX;

		// $GLOBAL 008846B0
		float g_clipOffsetY;

		// $GLOBAL 00884630
		int32_t g_clipNormMatrixDirty;

		// $GLOBAL 008846B4
		int32_t g_screenSpaceMatrixDirty;

		// $GLOBAL 00884670
		float g_currentViewportX;

		// $GLOBAL 00884674
		float g_currentViewportY;

		// $GLOBAL 008845A0
		float g_minViewportY;

		// $GLOBAL 008845A4
		float g_minViewportX;

		// $GLOBAL 00884678
		DWORD g_currentViewportWidth;

		// $GLOBAL 0088466C
		DWORD g_currentViewportHeight;

		// $GLOBAL 00E4D958
		int32_t g_viewportChangeCount;

		// $GLOBAL 008845A8
		int32_t g_drawDeviceWidth;

		// $GLOBAL 008845AC
		int32_t g_drawDeviceHeight;

		// $FUNC 004B55D0 [IMPLEMENTED]
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

		// $FUNC 004B5630 [IMPLEMENTED]
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
	}
}