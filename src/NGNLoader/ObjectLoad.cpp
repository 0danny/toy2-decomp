#include "NGNLoader/ObjectLoad.h"
#include "NGNLoader/NGNLoader.h"
#include "Renderer/Renderer.h"
#include "Logger.h"

namespace NGNLoader
{
	namespace ObjectLoad
	{
		// GLOBAL: TOY2 0x00B0574C
		Nu3D::Primitive* g_curPrimObject;

		// GLOBAL: TOY2 0x00AFBB08
		int32_t g_curVertexCount;

		// GLOBAL: TOY2 0x00B1A470
		int32_t g_curMaterialCount;

		// GLOBAL: TOY2 0x00AAD8B0
		int32_t g_unused1;

		// GLOBAL: TOY2 0x00508A64
		int32_t g_isHardwareRendering = 1;

		// GLOBAL: TOY2 0x00AAD7B0
		char g_curShapeName[256];

		// GLOBAL: TOY2 0x00B1A474
		uint8_t g_curShapeNameLen;

		// GLOBAL: TOY2 0x00B1A420
		char* g_nameTableEntries[20];

		// GLOBAL: TOY2 0x00B057A0
		char g_nameTableBuffer[5120];

		// GLOBAL: TOY2 0x00AE8234
		uint32_t g_textureTable[42];

		// GLOBAL: TOY2 0x00B05750
		Nu3D::Material* g_curMaterialList[20];

		// GLOBAL: TOY2 0x00AE8288
		Nu3D::ShapeVertex g_shapeVertices[2000];

		// GLOBAL: TOY2 0x00B626A8
		int32_t g_curVertexFlags;

		// FUNCTION: TOY2 0x004CC430
		int32_t ExtractShapeName(FILE* stream)
		{
			fread(&g_curShapeNameLen, 1, 1, stream);
			fread(g_curShapeName, 1, g_curShapeNameLen, stream);
			g_curShapeName[g_curShapeNameLen] = '\0';

			return 1;
		}

		// FUNCTION: TOY2 0x004CB320
		int32_t ExtractShapeTextures(FILE* stream)
		{
			int16_t nameTableCount;
			int16_t texCount;
			uint8_t nameTableCountOffset;
			uint8_t nameTableLength;

			fread(&nameTableCount, sizeof(int16_t), 1, stream);
			fread(&texCount, sizeof(int16_t), 1, stream);
			fread(&nameTableCountOffset, sizeof(uint8_t), 1, stream);
			fread(&nameTableLength, sizeof(uint8_t), 1, stream);

			if ((nameTableCountOffset + 1) * nameTableCount > 5120)
				Logger::GetErrorHandler("C:\\projects\\nu3d\\objload.c", 228)("texture nametable size exceeds maximum allowed");

			if (texCount >= 20)
				Logger::GetErrorHandler("C:\\projects\\nu3d\\objload.c", 231)("number of textures %hd exceeds maximum allowed per shape %d", texCount, 20);

			int32_t i = 0;
			int32_t offset = 0;

			if (nameTableCount)
			{
				char** buffer = g_nameTableEntries;
				do
				{
					*buffer = &g_nameTableBuffer[offset];

					uint8_t texDataLength;
					fread(&texDataLength, sizeof(uint8_t), 1, stream);

					if (texDataLength)
						fread(*buffer, 1, texDataLength, stream);

					(*buffer)[texDataLength] = '\0';
					++buffer;

					++i;
					offset += nameTableCountOffset + 1;
				} while (i < nameTableCount);
			}

			i = 0;

			if (texCount)
			{
				uint32_t* nextIndex = g_textureTable;

				do
				{
					int16_t nameIdx;
					fread(&nameIdx, sizeof(int16_t), 1, stream);

					NGNTextureParams texParams;
					texParams.rawTexStr = g_nameTableEntries[nameIdx];
					fread(&texParams.isTex14, sizeof(NGNTextureParams) - sizeof(char*), 1, stream);

					if (nameTableLength != 26)
						fseek(stream, (uint8_t)(nameTableLength - 26), SEEK_CUR);

					*nextIndex = GetOrAllocateTexture(&texParams);

					++i;
					++nextIndex;
				} while (i < texCount);
			}

			return 1;
		}

		// FUNCTION: TOY2 0x004CB4E0
		int32_t ExtractShapeMaterials(FILE* stream)
		{
			int16_t textureIndex[16];
			Nu3D::MaterialFile materialFile;

			uint32_t materialCount;
			fread(&materialCount, sizeof(uint32_t), 1, stream);

			if (materialCount >= 20)
				Logger::GetErrorHandler("C:\\projects\\nu3d\\objload.c", 288)("Given material count of %d exceeds maximum allowed (%d)", materialCount, 20);

			uint32_t count = 0;

			if (materialCount)
			{
				Nu3D::Material** curMaterial = g_curMaterialList;

				do
				{
					int32_t oneColor;
					RGBColor3B color;
					uint32_t materialId;
					uint32_t blockSize;

					fread(&materialId, sizeof(uint32_t), 1, stream);
					fread(&blockSize, sizeof(uint32_t), 1, stream);

					if ((materialId & 1) != 0)
					{
						fread(&color, sizeof(uint8_t), 3, stream);

						materialFile.diffuseColor.r = color.r * Renderer::g_gammaCorrection * (1.0f / 255.0f);
						oneColor = color.b;
						blockSize = (blockSize - 3);
						materialFile.diffuseColor.g = color.g * Renderer::g_gammaCorrection * (1.0f / 255.0f);
						materialFile.diffuseColor.b = color.b * Renderer::g_gammaCorrection * (1.0f / 255.0f);
					}
					else
					{
						materialFile.diffuseColor.b = 1.0;
						materialFile.diffuseColor.g = 1.0;
						materialFile.diffuseColor.r = 1.0;
					}

					if ((materialId & 2) != 0)
					{
						fread(&color, sizeof(uint8_t), 3, stream);

						blockSize = (blockSize - 3);
						materialFile.ambientColor.r = color.r * Renderer::g_gammaCorrection * (1.0f / 255.0f);
						oneColor = color.b;
						materialFile.ambientColor.g = color.g * Renderer::g_gammaCorrection * (1.0f / 255.0f);
						materialFile.ambientColor.b = color.b * Renderer::g_gammaCorrection * (1.0f / 255.0f);
					}
					else
					{
						materialFile.ambientColor.b = 1.0;
						materialFile.ambientColor.g = 1.0;
						materialFile.ambientColor.r = 1.0;
					}

					if ((materialId & 4) != 0)
					{
						fread(&color, sizeof(uint8_t), 3, stream);

						blockSize = (blockSize - 3);
						materialFile.specularColor.r = color.r * Renderer::g_gammaCorrection * (1.0f / 255.0f);
						oneColor = color.b;
						materialFile.specularColor.g = color.g * Renderer::g_gammaCorrection * (1.0f / 255.0f);
						materialFile.specularColor.b = color.b * Renderer::g_gammaCorrection * (1.0f / 255.0f);
					}
					else
					{
						materialFile.specularColor.b = 2.0;
						materialFile.specularColor.g = 2.0;
						materialFile.specularColor.r = 2.0;
					}

					if ((materialId & 8) != 0)
					{
						fread(&color, sizeof(uint8_t), 1, stream);
						oneColor = color.r;
						blockSize = (blockSize - 1);
						materialFile.emissiveIntensity = color.r * (1.0f / 255.0f);
					}
					else
					{
						materialFile.emissiveIntensity = 0.0;
					}

					if ((materialId & 16) != 0)
					{
						fread(&color, sizeof(uint8_t), 1, stream);
						oneColor = color.r;
						blockSize = (blockSize - 1);
						materialFile.power = color.r * (1.0f / 255.0f);
					}
					else
					{
						materialFile.power = 1.0;
					}

					if ((materialId & 32) != 0)
					{
						fread(&color, sizeof(uint8_t), 1, stream);
						oneColor = color.r;
						blockSize = (blockSize - 1);
						materialFile.opacity = color.r * (1.0f / 255.0f);
					}
					else
					{
						materialFile.opacity = 1.0;
					}

					if ((materialId & 64) != 0)
					{
						fread(&materialFile.metadata, sizeof(int32_t), 1, stream);
						blockSize = (blockSize - 4);
					}
					else
					{
						materialFile.metadata = 0;
					}

					uint32_t size = (materialId >> 7) & 0xF;

					if (size && (fread(textureIndex, sizeof(int16_t), size, stream), blockSize = (blockSize - 2 * size), textureIndex[0] >= 0))
					{
						materialFile.texDataIndex = g_textureTable[textureIndex[0]];
					}
					else
					{
						materialFile.texDataIndex = 0;
					}

					*curMaterial = Nu3D::Material::CreateFromFile(&materialFile);

					if (blockSize)
						fseek(stream, blockSize, SEEK_CUR);

					++count;
					++curMaterial;

				} while (count < materialCount);

				g_curMaterialCount = count;

				return 1;
			}
			else
			{
				g_curMaterialCount = 0;
				return 1;
			}
		}

		// FUNCTION: TOY2 0x004CB970
		int32_t ExtractShapeVertices(FILE* stream)
		{
			uint32_t vertexFlags;
			int32_t vertexDataLength;
			uint32_t vertexCount;

			fread(&vertexFlags, sizeof(uint32_t), 1, stream);
			fread(&vertexDataLength, sizeof(int32_t), 1, stream);
			fread(&vertexCount, sizeof(uint32_t), 1, stream);

			if (vertexCount >= 2000)
				Logger::GetErrorHandler("C:\\projects\\nu3d\\objload.c", 428)("Given vertex count of %d exceeds maximum allowed (%d)", vertexCount, 2000);

			g_curVertexCount = vertexCount;
			g_curVertexFlags = (uint8_t)vertexFlags;

			int32_t vertexSize = vertexDataLength;

			if (vertexFlags & 1)
				vertexSize -= 12; // position

			if (vertexFlags & 2)
				vertexSize -= 12; // normals

			if (vertexFlags & 4)
				vertexSize -= 4; // diffuse color

			if (vertexFlags & 8)
				vertexSize -= 4; // specular color

			int32_t sizeCheck = vertexSize - 8 * (vertexFlags >> 4); // texcoord sets, 8 bytes each

			if (sizeCheck < 0)
				Logger::GetErrorHandler("C:\\projects\\nu3d\\objload.c", 449)("Given vertex size is not big enough to contain specified data");

			uint32_t curVertex = 0;

			if (vertexCount)
			{
				Nu3D::ShapeVertex* shapeVertex = g_shapeVertices;

				do
				{
					shapeVertex->primVerticesSize = 65535;
					shapeVertex->primIndex = 65535;

					if (vertexFlags & 1)
					{
						fread(shapeVertex, sizeof(Vector3F), 1, stream);

						shapeVertex->position.x = NGNLoader::g_vertexScaleVector.x * shapeVertex->position.x;
						shapeVertex->position.y = NGNLoader::g_vertexScaleVector.y * shapeVertex->position.y;
						shapeVertex->position.z = NGNLoader::g_vertexScaleVector.z * shapeVertex->position.z;
					}
					else
					{
						Logger::GetErrorHandler("C:\\projects\\nu3d\\objload.c", 462)("Vertex must contain a position");
					}

					if (vertexFlags & 2)
					{
						fread(&shapeVertex->normals, sizeof(Vector3F), 1, stream);
					}
					else
					{
						shapeVertex->normals.x = 0.0f;
						shapeVertex->normals.y = 1.0f;
						shapeVertex->normals.z = 0.0f;
					}

					ARGB color;

					if (vertexFlags & 4)
					{
						fread(&color, sizeof(uint8_t), sizeof(ARGB), stream);
					}
					else
					{
						color.a = 255;
						color.b = 255;
						color.g = 255;
						color.r = 255;
					}

					shapeVertex->diffuse.a = color.a;

					int32_t scaled = color.r * Renderer::g_gammaCorrection;

					if (scaled > 255)
						scaled = 255;

					shapeVertex->diffuse.b = (uint8_t)scaled;

					scaled = color.g * Renderer::g_gammaCorrection;

					if (scaled > 255)
						scaled = 255;

					shapeVertex->diffuse.g = (uint8_t)scaled;

					scaled = color.b * Renderer::g_gammaCorrection;

					if (scaled > 255)
						scaled = 255;

					shapeVertex->diffuse.r = (uint8_t)scaled;

					if (vertexFlags & 8)
					{
						fread(&color, sizeof(uint8_t), sizeof(ARGB), stream);
					}
					else
					{
						color.b = 255;
						color.g = 255;
						color.r = 255;
						color.a = 255;
					}

					if (vertexFlags & 240)
					{
						fread(&shapeVertex->coords, 4, 2, stream);
					}
					else
					{
						shapeVertex->coords.y = 0.0f;
						shapeVertex->coords.x = 0.0f;
					}

					if (sizeCheck)
						fseek(stream, sizeCheck, SEEK_CUR);

					++shapeVertex;
					++curVertex;

				} while (curVertex < vertexCount);
			}

			return 1;
		}

		// FUNCTION: TOY2 0x004CBC90
		int32_t ExtractShapePrimitives(FILE* stream) { return 0; }

		// FUNCTION: TOY2 0x004CB270
		void PrepareGlobals()
		{
			g_curVertexCount = 0;
			g_curMaterialCount = 0;

			g_unused1 = 0;
			g_curPrimObject = 0;

			g_isHardwareRendering = Renderer::g_isSoftwareRendering == 0;
		}

		// FUNCTION: TOY2 0x004CC970
		Nu3D::Primitive* ExtractShapeData(FILE* stream)
		{
			PrepareGlobals();

			int32_t stop = 0;
			int32_t chunkId;

			if (fread(&chunkId, sizeof(int32_t), 1, stream))
			{
				do
				{
					int32_t size;

					fread(&size, sizeof(int32_t), 1, stream);
					ftell(stream);

					DECOMP_PRINT(("Found chunk id -> %d\n", chunkId));

					switch (chunkId)
					{
						case 0:
							stop = 1;
							break;

						case 64:
							ExtractShapeName(stream);
							break;

						case 65:
							ExtractShapeTextures(stream);
							break;

						case 66:
							ExtractShapeMaterials(stream);
							break;

						case 67:
							ExtractShapeVertices(stream);
							break;

						case 68:
							ExtractShapePrimitives(stream);
							break;

						default:
							fseek(stream, size, SEEK_CUR);
							break;
					}

					ftell(stream);

				} while (! stop && fread(&chunkId, sizeof(int32_t), 1, stream));
			}

			Nu3D::Primitive* result = g_curPrimObject;

			if (g_curPrimObject)
			{
				Nu3D::Primitive::ComputeBounds(g_curPrimObject);
				return g_curPrimObject;
			}

			return result;
		}

		// FUNCTION: TOY2 0x004CB940
		Nu3D::Material* GetCurrentMatByIndex(uint32_t index)
		{
			if (index >= g_curMaterialCount || index < 0)
				return 0;
			else
				return g_curMaterialList[index];
		}
	}
}