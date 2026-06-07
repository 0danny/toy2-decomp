#pragma once

#include "Common.h"
#include "Numerics.h"
#include "Nu3D/Viewport.h"
#include "Nu3D/Sprite.h"
#include "Nu3D/Primitive.h"
#include "Nu3D/Material.h"
#include "Nu3D/Link.h"
#include "Nu3D/Portal.h"
#include "Nu3D/Creature.h"
#include "Renderer/Renderer.h"

#include <directx7/d3d.h>

namespace NGNLoader
{
	struct TextureEntry
	{
		int32_t isBGR;
		int32_t textureDataIndex;
		int32_t unused1;
		int32_t unused2;
	};

	struct Type266Entry
	{
		int16_t count;
		int16_t index;
		Vector3F* points;
	};

	struct NGNImage
	{
		Nu3D::Primitive** primitives;
		int32_t primCount;
		Nu3D::Link::DynamicScaler* dynamicScalers[2];
		int32_t shapeCounts[2];
		int32_t gscaleType;
		Nu3D::Link::DynamicScaler** spacialGrid[2];
		float worldMinX;
		float worldMaxX;
		float worldMinZ;
		float worldMaxZ;
		int32_t gridWidth;
		int32_t gridHeight;
		float cellWidthInWorldUnits;
		float cellHeightInWorldUnits;
		Nu3D::Portal::AreaPortal** areaPortals;
		int32_t actualPortalCount;
		Nu3D::Link::Linker* links;
		int32_t maxLinkId;
		Type266Entry* data266[128];
		Nu3D::Portal::PortalHashTable* portalHashTable;
		Nu3D::Portal::ScalerEntry* scalerEntryPool;
		int32_t scalerEntryCount;
		int32_t maxScalerEntries;
		Nu3D::Portal::PortalState* portalStatePool;
		int32_t portalEntryCount;
		int32_t areaPortalCount;
		int32_t bucketCount;
		Nu3D::Creature** creatureData;
		int32_t creatureCount;
		TextureEntry textureEntries[64];
	};

	STATIC_ASSERT(sizeof(NGNImage) == 0x67C);
	STATIC_ASSERT(sizeof(NGNTextureEntry) == 0x10);
}