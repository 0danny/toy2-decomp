#include "NGNLoader.h"
#include "Nu3D/Portal.h"

#include <windows.h>
#include <iostream>

namespace NGNLoader
{
	// $GLOBAL 00B62410
	NGNImage* g_ngnImage;

	// $GLOBAL 00AAD7AC
	char* g_curFileName;

	// $GLOBAL 00AAD7A8;
	int32_t g_curPrimCount;

	// $GLOBAL 00508A58
	Vector3F g_vertexScaleVector = { 1.0, 1.0, 1.0 };

	// $FUNC 004CB300 [IMPLEMENTED]
	void GetScaleVector(Vector3F* output) { *output = g_vertexScaleVector; }

	// $FUNC 004B3350 [IMPLEMENTED]
	Nu3D::Portal::AreaPortal* AllocPortalVertices(int32_t vertexCount)
	{
		int32_t size = sizeof(Vector3F) * vertexCount * sizeof(Nu3D::Portal::AreaPortal);

		Nu3D::Portal::AreaPortal* alloc = (Nu3D::Portal::AreaPortal*)malloc(size);

		if (alloc)
		{
			memset(alloc, 0, size);

			if (vertexCount)
			{
				alloc->vertexCount = vertexCount;

				// Points to space after struct, is the vertex space.
				alloc->vertices = reinterpret_cast<Vector3F*>(&alloc[1]);
			}
		}

		return alloc;
	}

	// $FUNC 004C4080 [UNFINISHED]
	void ParseTextures(FILE* stream, NGNImage* ngnImage) {}

	// $FUNC 004C3EB0 [IMPLEMENTED]
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

	// $FUNC 004C4220 [UNFINISHED]
	void ParseCreatures(FILE* stream, NGNImage* ngnImage) {}

	// $FUNC 004C3CA0 [IMPLEMENTED]
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

	// $FUNC 004C3DF0 [UNFINISHED]
	void ParseAreaPortalRot(FILE* stream, NGNImage* ngnImage) {}

	// $FUNC 004C3BE0 [IMPLEMENTED]
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

				ngnImage->areaPortals[portalId] = NGNLoader::AllocPortalVertices(vertexCount);

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

	// $FUNC 004C3740 [UNFINISHED]
	void ParseGscale(FILE* stream, NGNImage* ngnImage) {}

	// $FUNC 004C35C0 [UNFINISHED]
	void ParseGeometry(FILE* stream, NGNImage* ngnImage) {}

	// $FUNC 004B9630 [UNFINISHED]
	void BuildTex14() {}

	// $FUNC 004C36A0 [UNFINISHED]
	void BuildGrid(int32_t gridWidth, int32_t gridHeight, int32_t type, NGNImage* ngnImage) {}

	// $FUNC 004C3240 [UNFINISHED]
	void BuildScalerEntries(NGNImage* p_ngnImage) {}

	// $FUNC 004C33F0 [IMPLEMENTED]
	NGNImage* BuildImage(char* fileName)
	{
		int32_t terminate = 0;
		NGNImage* ngnImage = 0;

		g_curFileName = fileName;
		FILE* fileHandle = fopen(fileName, "rb");

		if (fileHandle)
		{
			ngnImage = (NGNImage*)(malloc(sizeof(NGNImage)));

			if (ngnImage)
			{
				memset(ngnImage, 0, sizeof(NGNImage));

				g_curPrimCount = 0;
				int32_t chunkHeaderId;

				if (fread(&chunkHeaderId, sizeof(int32_t), 1, fileHandle)) // Get Chunk Header ID
				{
					do
					{
						int32_t chunkSize;

						fread(&chunkSize, sizeof(int32_t), 1, fileHandle); // Get Chunk Size
						ftell(fileHandle);

						if (chunkHeaderId > 259)
						{
							switch (chunkHeaderId)
							{
								case 260:
									ParseTextures(fileHandle, ngnImage);
									break;
								case 261:
									ParseLinker(fileHandle, ngnImage);
									break;
								case 262:
									ParseCreatures(fileHandle, ngnImage);
									break;
								case 266:
									Parse266(fileHandle, ngnImage);
									break;
								default:
									fseek(fileHandle, chunkSize, 1);
							}
						}
						else if (chunkHeaderId == 259)
						{
							ParseAreaPortalRot(fileHandle, ngnImage);
						}
						else if (chunkHeaderId > 257)
						{
							ParseAreaPortalPos(fileHandle, ngnImage);
						}
						else if (chunkHeaderId == 257)
						{
							ParseGscale(fileHandle, ngnImage);
							++ngnImage->gscaleType;
						}
						else if (chunkHeaderId)
						{
							if (chunkHeaderId == 256)
								ParseGeometry(fileHandle, ngnImage);
							else
								fseek(fileHandle, chunkSize, 1);
						}
						else
						{
							terminate = 1;
						}

						ftell(fileHandle);

					} while (! terminate && fread(&chunkHeaderId, sizeof(int32_t), 1, fileHandle));
				}
			}

			fclose(fileHandle);
		}

		BuildTex14();

		if (ngnImage)
		{
			for (int32_t typeIndex = 0; typeIndex < ngnImage->gscaleType; ++typeIndex)
				BuildGrid(20, 20, typeIndex, ngnImage);

			BuildScalerEntries(ngnImage);
		}

		return ngnImage;
	}

	// $FUNC 004CEAE0 [IMPLEMENTED]
	void SetNewImage(char* fileName)
	{
		g_ngnImage = BuildImage(fileName);

		Nu3D::Portal::ClearVisibleAreaFlags();
	}
}