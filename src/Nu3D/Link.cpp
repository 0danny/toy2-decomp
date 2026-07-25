#pragma once

#include "Nu3D/Link.h"
#include "Nu3D/Math.h"
#include "NGNLoader/NGNLoader.h"

namespace Nu3D
{
	namespace Link
	{
		// FUNCTION: TOY2 0x004C3130 [MATCHED]
		DynamicScaler** GetCellByPos(Vector3I* pos, NGNLoader::NGNImage* ngnImage)
		{
			int32_t x = pos->x;

			if (pos->x < 0)
				return 0;

			int32_t gridWidth = ngnImage->gridWidth;

			if (x >= gridWidth)
				return 0;

			int32_t y = pos->y;

			if (y < 0 || y >= ngnImage->gridHeight)
				return 0;
			else
				return &ngnImage->spacialGrid[pos->z][x] + gridWidth * y;
		}

		// FUNCTION: TOY2 0x004C30D0 [MATCHED]
		int32_t ComputeCellFromXZ(Vector3I* pos, float x, float z, int32_t layer, NGNLoader::NGNImage* ngnImage)
		{
			int32_t xTemp = ((x - ngnImage->worldMinX) / ngnImage->cellWidthInWorldUnits);
			int32_t yTemp = ((z - ngnImage->worldMinZ) / ngnImage->cellHeightInWorldUnits);

			if (xTemp < 0 || xTemp >= ngnImage->gridWidth || yTemp < 0 || yTemp >= ngnImage->gridHeight)
				return 0;

			pos->y = yTemp;
			pos->x = xTemp;
			pos->z = layer;

			return 1;
		}

		// FUNCTION: TOY2 0x004C31C0 [MATCHED]
		void InsertScalerAtComputedCell(DynamicScaler* scaler, int32_t type, NGNLoader::NGNImage* ngnImage)
		{
			Vector3F vector;
			Vector3I pos;

			Math::VertexAdd(&vector, &scaler->translation, &scaler->boundsCenterWorld);

			if (Link::ComputeCellFromXZ(&pos, vector.x, vector.z, type, ngnImage))
			{
				DynamicScaler** head = Link::GetCellByPos(&pos, ngnImage);
				DynamicScaler* headPtr = *head;

				scaler->next = *head;

				if (headPtr)
					headPtr->prev = scaler;

				scaler->prev = 0;
				scaler->cellHead = head;

				*head = scaler;
			}
			else
			{
				scaler->cellHead = 0;
			}
		}

		// FUNCTION: TOY2 0x004C3170 [MATCHED]
		void UnlinkScalerThenReinsert(DynamicScaler* scaler, NGNLoader::NGNImage* ngnImage)
		{
			DynamicScaler** cellHead = scaler->cellHead;

			if (cellHead)
			{
				DynamicScaler* prev = scaler->prev;

				if (prev)
					prev->next = scaler->next;
				else
					*cellHead = scaler->next;

				if (scaler->next)
					scaler->next->prev = scaler->prev;

				scaler->cellHead = 0;
			}

			InsertScalerAtComputedCell(scaler, scaler->gscaleType, ngnImage);
		}

		// FUNCTION: TOY2 0x004CCBE0 [MATCHED]
		void RebuildMatrixAndCommit(Linker* link)
		{
			D3DMATRIX* matrix = &link->dynamicScaler->transformMatrix;

			Math::BuildIdentityMatrix(matrix);
			Math::ScaleMatrixByVector(matrix, &link->currentScale);

			Math::RotateZFromLut(matrix, link->currentRot.z);
			Math::RotateYFromLut(matrix, link->currentRot.y);
			Math::PostRotateXFromLut(matrix, link->currentRot.x);

			Math::AddWorldSpaceTransform(matrix, &link->currentPos);

			Math::TransformVectorByMatrix(
				&link->dynamicScaler->boundsCenterWorld, &NGNLoader::g_ngnImage->primitives[link->dynamicScaler->shapeId]->boundsCenter, matrix);

			link->dynamicScaler->translation = link->currentPos;

			UnlinkScalerThenReinsert(link->dynamicScaler, NGNLoader::g_ngnImage);
		}

		// FUNCTION: TOY2 0x004CCB20 [MATCHED]
		void SetScaleFromFixedOffsets(int32_t linkId, int32_t x, int32_t y, int32_t z)
		{
			if (! NGNLoader::g_ngnImage)
				return;

			Linker* links = NGNLoader::g_ngnImage->links;

			if (! links)
				return;

			if (linkId < NGNLoader::g_ngnImage->maxLinkId)
			{
				Linker* link = &links[linkId];

				DynamicScaler* scaler = link->dynamicScaler;

				if (! scaler)
					return;

				if (x || y || z)
				{
					float xValue = x * scaler->scale.x;

					scaler->flags &= ~1;
					link->currentScale.x = xValue * 0.000244140625f;
					link->currentScale.y = y * scaler->scale.y * 0.000244140625f;
					link->currentScale.z = z * scaler->scale.z * 0.000244140625f;

					RebuildMatrixAndCommit(link);
				}
				else
				{
					scaler->flags |= 1u;
				}
			}
		}
	}
}