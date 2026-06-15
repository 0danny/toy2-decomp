#pragma once

#include "Numerics.h"
#include "Nu3D/Primitive.h"
#include "Nu3D/Patch.h"
#include <directx7/d3d.h>

namespace Nu3D
{
	struct Creature
	{
		int32_t dataFlags;
		int32_t nodeCount;
		D3DMATRIX* matrixList1;
		D3DMATRIX* matrixList2;
		D3DMATRIX* matrixList3;
		char** nodeNames;
		Primitive** primitives;
		Patch* patch;
		int32_t** flagsList;
		int32_t** unkVar10;
		void** animData;
		int32_t animCount;
	};

    STATIC_ASSERT(sizeof(Creature) == 0x30);
}