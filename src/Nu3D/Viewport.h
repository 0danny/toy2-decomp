#pragma once

#include "Common.h"

namespace Nu3D
{
	namespace Viewport
	{
		struct ViewportRect
		{
			float top;
			float bottom;
			float left;
			float right;
		};

		struct ViewportRectAlt
		{
			float left;
			float top;
			float right;
			float bottom;
		};

		struct FustrumInfo
		{
			int32_t unkInt1;
			int32_t unkInt2;
			int32_t unkInt3;
			int32_t unkInt4;
			int32_t unkInt5;
			int32_t unkInt6;
			int32_t unkInt7;
		};

		void Init();
		void Reset();
		void SetViewClipRect();
        void GetViewClipRect(ViewportRect* output);
	}

	STATIC_ASSERT(sizeof(Viewport::ViewportRect) == 0x10);
	STATIC_ASSERT(sizeof(Viewport::FustrumInfo) == 0x1C);
}