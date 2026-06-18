#include "NGNLoader/NGNLoader.h"
#include "Nu3D/Portal.h"
#include "Toy2/Toy2.h"
#include "Nu3D/BmpDataNode.h"
#include "Logger.h"

#include <windows.h>

namespace NGNLoader
{
	// GLOBAL: TOY2 0x00B62410
	NGNImage* g_ngnImage;

	// GLOBAL: TOY2 0x00AAD7AC
	char* g_curFileName;

	// GLOBAL: TOY2 0x00AAD7A8;
	int32_t g_curPrimCount;

	// GLOBAL: TOY2 0x00508A58
	Vector3F g_vertexScaleVector = { 1.0, 1.0, 1.0 };

	// FUNCTION: TOY2 0x004CB300
	void GetScaleVector(Vector3F* output) { *output = g_vertexScaleVector; }

	// STUB: TOY2 0x004BB3C0
	int32_t GetOrAllocateTexture(NGNTextureParams* texParams) { return 0; }

	// FUNCTION: TOY2 0x004AC220
	Nu3D::BmpDataNode* LoadTextureContents(FILE* stream, const char* rawTexStr, int32_t flags) { return Nu3D::LoadTextureByStream(stream, rawTexStr, flags); }

	// FUNCTION: TOY2 0x004BC320
	Nu3D::Portal::PortalState* AllocAreaPortal(NGNImage* ngnImage)
	{
		int32_t entryCount = ngnImage->portalEntryCount;

		if (entryCount >= ngnImage->areaPortalCount)
			return 0;

		Nu3D::Portal::PortalState* pool = ngnImage->portalStatePool;

		if (! pool)
			return 0;

		Nu3D::Portal::PortalState* newAlloc = &pool[entryCount];
		ngnImage->portalEntryCount = entryCount + 1;

		return newAlloc;
	}

	// FUNCTION: TOY2 0x004BC2C0
	int32_t InsertPortal(NGNImage* ngnImage, int32_t sourceAreaIdx, int32_t targetAreaIdx, Nu3D::Portal::AreaPortal* portal)
	{
		if (Toy2::g_isElevatorHopLevel && targetAreaIdx == 15)
			targetAreaIdx = -1;

		Nu3D::Portal::PortalState* head = AllocAreaPortal(ngnImage);

		if (head)
		{
			head->targetAreaIdx = targetAreaIdx;
			head->portal = portal;
			head->sourceAreaIdx = sourceAreaIdx;

			head->next = ngnImage->portalHashTable->buckets[sourceAreaIdx].portalStateHead;
			ngnImage->portalHashTable->buckets[sourceAreaIdx].portalStateHead = head;

			return 1;
		}

		return 0;
	}

	// FUNCTION: TOY2 0x004BC230
	void AllocPools(NGNImage* ngnImage, int32_t portalCount, int32_t maxScalerEntries)
	{
		ngnImage->portalEntryCount = 0;
		ngnImage->scalerEntryCount = 0;

		ngnImage->scalerEntryPool = (Nu3D::Portal::ScalerEntry*)malloc(sizeof(Nu3D::Portal::ScalerEntry) * maxScalerEntries);
		ngnImage->maxScalerEntries = maxScalerEntries;

		ngnImage->scalerEntryCount = 0;
		ngnImage->portalStatePool = (Nu3D::Portal::PortalState*)malloc(sizeof(Nu3D::Portal::PortalState) * portalCount);
		ngnImage->portalEntryCount = 0;
		ngnImage->areaPortalCount = portalCount;

		Nu3D::Portal::PortalHashTable* rotLookup = (Nu3D::Portal::PortalHashTable*)malloc(sizeof(Nu3D::Portal::PortalHashTable));

		ngnImage->portalHashTable = rotLookup;
		ngnImage->bucketCount = 64;

		memset(rotLookup, 0, sizeof(Nu3D::Portal::PortalHashTable));
	}

	// FUNCTION: TOY2 0x004B3350
	Nu3D::Portal::AreaPortal* AllocPortalVertices(int32_t vertexCount)
	{
		Nu3D::Portal::AreaPortal* portal = (Nu3D::Portal::AreaPortal*)malloc(sizeof(Vector3F) * vertexCount + sizeof(Nu3D::Portal::AreaPortal));

		if (portal)
		{
			memset(portal, 0, sizeof(Vector3F) * vertexCount + sizeof(Nu3D::Portal::AreaPortal));

			if (vertexCount)
			{
				// Points to space after struct, is the vertex space.
				portal->vertexCount = vertexCount;
				portal->vertices = reinterpret_cast<Vector3F*>(&portal[1]);
			}
		}

		return portal;
	}

	// STUB: TOY2 0x004C4080
	void ParseTextures(FILE* stream, NGNImage* ngnImage)
	{
		NGNTextureParams texParams;
		char rawTexStr[256];

		memset(&texParams, 0, sizeof(texParams));
		texParams.rawTexStr = rawTexStr;

		int32_t textureCount;
		fread(&textureCount, sizeof(int32_t), 1, stream);

		for (int32_t textureIndex = textureCount; textureIndex > 0; --textureIndex)
		{
			uint32_t dataOffset;
			uint32_t rawTexStrLen;

			fread(&dataOffset, sizeof(uint32_t), 1, stream);
			fread(&rawTexStrLen, sizeof(uint32_t), 1, stream);
			fread(rawTexStr, sizeof(char), rawTexStrLen, stream);

			rawTexStr[rawTexStrLen] = '\0';

			strlwr(rawTexStr);
			int32_t textureId = atoi(&rawTexStr[3]);

			int32_t flags = 0;
			int32_t isBGR = 0;

			if (textureId != 14 && textureId != 36 && textureId != 37)
				flags = 8;

			if (textureId > 31)
				flags |= 32;

			if (strstr(rawTexStr, "bgr"))
			{
				memcpy(rawTexStr, "tex", 3);
				isBGR = 1;
			}

			int32_t beforeOffset = ftell(stream);

			LoadTextureContents(stream, rawTexStr, flags);

			int32_t afterOffset = ftell(stream);

			if (afterOffset - beforeOffset != dataOffset)
				Logger::GetErrorHandler("C:\\projects\\nu3d\\world.c", 573)("bitmap parsed incorrectly in file %s", g_curFileName);

			texParams.isTex14 = strcmpi("tex14", rawTexStr) == 0;

			int32_t index = GetOrAllocateTexture(&texParams);

			if (textureId < 64)
			{
				ngnImage->textureEntries[textureId].isBGR = isBGR;
				ngnImage->textureEntries[textureId].textureDataIndex = index;
			}
		}
	}

	// FUNCTION: TOY2 0x004C3EB0
	void ParseLinker(FILE* stream, NGNImage* ngnImage)
	{
		int32_t maxLinkId;
		int32_t linkCount;

		fread(&linkCount, sizeof(int32_t), 1, stream);
		fread(&maxLinkId, sizeof(int32_t), 1, stream);

		++maxLinkId;

		Nu3D::Link::Linker* linkerArray = (Nu3D::Link::Linker*)malloc(sizeof(Nu3D::Link::Linker) * maxLinkId);

		ngnImage->links = linkerArray;

		if (! linkerArray)
			return;

		int32_t count = 0;

		memset(linkerArray, 0, sizeof(Nu3D::Link::Linker) * maxLinkId);

		ngnImage->maxLinkId = maxLinkId;

		if (linkCount > 0)
		{
			do
			{
				uint16_t linkId;
				uint16_t shapeId;

				fread(&linkId, sizeof(uint16_t), 1, stream);
				fread(&shapeId, sizeof(uint16_t), 1, stream);

				if (linkId >= 0 && linkId < maxLinkId)
				{
					int32_t type;

					if (shapeId & 0x8000 == 0)
					{
						type = 0;
					}
					else
					{
						shapeId = -shapeId & 0x3FFF;
						type = 1;
					}

					if (shapeId < ngnImage->shapeCounts[type])
					{
						Nu3D::Link::Linker* link = &ngnImage->links[linkId];
						Nu3D::Link::DynamicScaler* dynamicScaler = &ngnImage->dynamicScalers[type][shapeId];

						link->dynamicScaler = dynamicScaler;

						memcpy(&link->transformMatrix, &dynamicScaler->transformMatrix, sizeof(link->transformMatrix));

						link->dynamicScaler->flags |= 2;

						link->currentPos = link->dynamicScaler->translation;
						link->targetPos = link->dynamicScaler->translation;
						link->currentRot = link->dynamicScaler->rotation;
						link->currentScale = link->dynamicScaler->scale;
					}
				}

				count += 1;

			} while (count < linkCount);
		}
	}

	// STUB: TOY2 0x004C4220
	void ParseCreatures(FILE* stream, NGNImage* ngnImage) {}

	// FUNCTION: TOY2 0x004C3CA0
	void Parse266(FILE* stream, NGNImage* ngnImage)
	{
		// data266 isn't actually used anywhere in the game
		Vector3F scale;
		GetScaleVector(&scale);

		memset(ngnImage->data266, 0, sizeof(ngnImage->data266));

		int32_t entryCount = 0;
		fread(&entryCount, sizeof(entryCount), 1, stream);

		for (int32_t i = 0; i < entryCount; ++i)
		{
			int16_t count = 0;
			int16_t index = 0;

			fread(&count, sizeof(count), 1, stream);
			fread(&index, sizeof(index), 1, stream);

			Type266Entry* entry = (Type266Entry*)malloc(sizeof(Type266Entry));

			entry->count = 0;
			entry->index = 0;
			entry->points = 0;

			entry->count = count;
			entry->index = index;
			entry->points = (Vector3F*)malloc(sizeof(Vector3F) * count);

			const bool emptyCount = entry->count == 0;
			const bool negativeCount = entry->count < 0;

			if (! emptyCount && ! negativeCount)
			{
				for (int32_t j = 0; j < count; ++j)
				{
					fread(&entry->points[j].x, sizeof(float), 1, stream);
					fread(&entry->points[j].y, sizeof(float), 1, stream);
					fread(&entry->points[j].z, sizeof(float), 1, stream);

					entry->points[j].x *= scale.x;
					entry->points[j].y *= scale.y;
					entry->points[j].z *= scale.z;
				}
			}

			ngnImage->data266[index] = entry;
		}
	}

	// FUNCTION: TOY2 0x004C3DF0
	void ParseAreaPortalIdx(FILE* stream, NGNImage* ngnImage)
	{
		int32_t portalId, targetAreaIdx, sourceAreaIdx, portalCount;

		fread(&portalCount, sizeof(int32_t), 1, stream);

		if (! portalCount)
			return;

		AllocPools(ngnImage, portalCount, 4000);

		for (int32_t index = 0; index < portalCount; ++index)
		{
			fread(&portalId, sizeof(int32_t), 1, stream);
			fread(&sourceAreaIdx, sizeof(int32_t), 1, stream);
			fread(&targetAreaIdx, sizeof(int32_t), 1, stream);

			Nu3D::Portal::AreaPortal::CalculateBoundingSphere(ngnImage->areaPortals[portalId]);
			InsertPortal(ngnImage, sourceAreaIdx, targetAreaIdx, ngnImage->areaPortals[portalId]);
		}
	}

	// FUNCTION: TOY2 0x004C3BE0
	void ParseAreaPortalPos(FILE* stream, NGNImage* ngnImage)
	{
		int32_t portalCount;
		fread(&portalCount, sizeof(int32_t), 1, stream);

		if (! portalCount)
			return;

		Nu3D::Portal::AreaPortal** portalAlloc = (Nu3D::Portal::AreaPortal**)malloc(sizeof(void*) * portalCount);
		ngnImage->areaPortals = portalAlloc;

		if (! portalAlloc)
			return;

		int32_t portalId = 0;

		if (portalCount > 0)
		{
			while (true)
			{
				int32_t vertexCount;
				fread(&vertexCount, sizeof(int32_t), 1, stream);

				ngnImage->areaPortals[portalId] = AllocPortalVertices(vertexCount);

				Nu3D::Portal::AreaPortal* portal = ngnImage->areaPortals[portalId];

				if (! portal)
					break;

				portal->portalId = portalId;

				fread(ngnImage->areaPortals[portalId++]->vertices, 12u, vertexCount, stream);

				if (portalId >= portalCount)
					return;
			}

			ngnImage->actualPortalCount = portalId;
		}
	}

	// STUB: TOY2 0x004C3740
	void ParseGscale(FILE* stream, NGNImage* ngnImage) {}

	// STUB: TOY2 0x004C35C0
	void ParseGeometry(FILE* stream, NGNImage* ngnImage) {}

	// STUB: TOY2 0x004B9630
	void BuildTex14(int32_t unused) {}

	// STUB: TOY2 0x004C36A0
	void BuildGrid(int32_t gridWidth, int32_t gridHeight, int32_t type, NGNImage* ngnImage) {}

	// STUB: TOY2 0x004C3240
	void BuildScalerEntries(NGNImage* ngnImage) {}

	// FUNCTION: TOY2 0x004C33F0
	NGNImage* BuildImage(char* fileName)
	{
		int32_t terminate = 0;
		NGNImage* ngnImage = 0;

		g_curFileName = fileName;

		FILE* fileHandle = fopen(fileName, "rb");

		if (fileHandle)
		{
			ngnImage = (NGNImage*)malloc(sizeof(NGNImage));

			if (ngnImage)
			{
				memset(ngnImage, 0, sizeof(NGNImage));

				g_curPrimCount = 0;

				int32_t chunkSize;
				int32_t chunkHeaderId;

				if (fread(&chunkHeaderId, sizeof(int32_t), 1, fileHandle))
				{
					do
					{
						fread(&chunkSize, sizeof(int32_t), 1, fileHandle);
						ftell(fileHandle);

						switch (chunkHeaderId)
						{
							case 256:
								ParseGeometry(fileHandle, ngnImage);
								break;

							case 257:
								ParseGscale(fileHandle, ngnImage);
								++ngnImage->gscaleType;
								break;

							case 258:
								ParseAreaPortalPos(fileHandle, ngnImage);
								break;

							case 259:
								ParseAreaPortalIdx(fileHandle, ngnImage);
								break;

							case 266:
								Parse266(fileHandle, ngnImage);
								break;

							case 260:
								ParseTextures(fileHandle, ngnImage);
								break;

							case 261:
								ParseLinker(fileHandle, ngnImage);
								break;

							case 262:
								ParseCreatures(fileHandle, ngnImage);
								break;

							case 0:
								terminate = 1;
								break;

							default:
								fseek(fileHandle, chunkSize, 1);
								break;
						}

						ftell(fileHandle);

					} while (! terminate && fread(&chunkHeaderId, sizeof(int32_t), 1, fileHandle));
				}
			}

			fclose(fileHandle);
		}

		BuildTex14(14);

		if (ngnImage)
		{
			for (int32_t typeIndex = 0; typeIndex < ngnImage->gscaleType; ++typeIndex)
				BuildGrid(20, 20, typeIndex, ngnImage);

			BuildScalerEntries(ngnImage);
		}

		return ngnImage;
	}

	// FUNCTION: TOY2 0x004CEAE0
	void SetNewImage(char* fileName)
	{
		g_ngnImage = BuildImage(fileName);

		Nu3D::Portal::ClearVisibleAreaFlags();
	}

	// STUB: TOY2 0x004BB720
	void Init() {}

	// STUB: TOY2 0x0044FF50
	void DetectBackdropTextures() {}
}