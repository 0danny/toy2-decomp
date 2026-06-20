#include "Toy2/Levels.h"

#include "SoftwareRenderer.h"
#include "FileUtils.h"
#include "Random.h"
#include "Logger.h"
#include "CharacterLoader.h"
#include "RawLoader.h"
#include "Collision.h"

#include "NGNLoader/NGNLoader.h"
#include "Toy2/Toy2.h"
#include "Toy2/Collectables.h"
#include "Toy2/Actor.h"
#include "AudioManager/AudioManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shadows.h"
#include "Renderer/Sprite.h"
#include "Nu3D/Particles.h"
#include "Nu3D/Camera.h"

namespace Toy2
{
	namespace Levels
	{
		// GLOBAL: TOY2 0x00559C70
		RecordData* g_recordData[96];

		// GLOBAL: TOY2 0x0055A114
		int32_t g_levelLoadConfig;

		// GLOBAL: TOY2 0x0055A12C
		int32_t g_levelLoadConfigCopy;

		// GLOBAL: TOY2 0x00729128
		void* g_cachedAllBuffer;

		// GLOBAL: TOY2 0x005D2BE8
		uint8_t g_levelDataHeapBase[1249280];

		// GLOBAL: TOY2 0x00703C00
		uint8_t* g_levelDataHeapBasePtr;

		// GLOBAL: TOY2 0x00559DF4
		uint8_t* g_levelLoadArena;

		// GLOBAL: TOY2 0x0054DE98
		void* g_levelDataBase;

		// STUB: TOY2 0x004CEA20
		void FlushRenderer() {}

		// STUB: TOY2 0x004CE8B0
		int32_t InitLevelDefaults() { return 0; }

		// STUB: TOY2 0x0043E6E0
		int32_t LoadDAT(int32_t levelId, int32_t fileSize) { return 0; }

		// FUNCTION: TOY2 0x004521A0
		void BuildLevelPath(int32_t level, char* output, const char* suffix)
		{
			if (level >= 10)
			{
				strcpy(output, "level");
				output[5] = level / 10 + 48;
				output[6] = level % 10 + 48;
			}
			else
			{
				strcpy(output, "level0");
				output[6] = level + 48;
			}

			strcpy(output + 7, "\\");
			strcat(output, suffix);
		}

		// FUNCTION: TOY2 0x00452FC0
		void InitLevelPlay(int32_t levelId)
		{
			SoftwareRenderer::SetLevelFileIndex(g_levelFileIndex);
			FlushRenderer();
			InitLevelDefaults();

			FileUtils::LoadFile("rand.dat", g_randDatBuffer);

			int32_t levelIdCpy = levelId;

			AudioManager::g_curTrackIndex = -1;
			g_isElevatorHopLevel = levelId != 10;

			Logger::Log("InitLevelPlay : START.\n");

			AudioManager::LoadSfxPackForLevel(levelId);

			memset(&g_recordData, 0, sizeof(g_recordData));
			memset(CharacterLoader::g_boneTransforms, 0, sizeof(CharacterLoader::g_boneTransforms));
			memset(Collision::g_collisionMeshInstances, 0, sizeof(Collision::g_collisionMeshInstances));
			memset(Collision::g_mathScratch, 0, sizeof(Collision::g_mathScratch));

			SoftwareRenderer::g_unk4F7400.x = -32768;
			SoftwareRenderer::g_unk4F7400.y = -32768;

			g_hasBackdrop = 0;

			CharacterLoader::g_unk54717C = 0;
			CharacterLoader::g_unk546D78 = 0;
			CharacterLoader::g_unk547CD0 = 0;

			SoftwareRenderer::g_unk500A1C = -1;

			Renderer::Shadows::g_shadowCount = 0;
			Renderer::Shadows::g_unusedShadowVar = 0;

			for (int32_t particleIdx = 0; particleIdx < 64; particleIdx++)
				Nu3D::Particles::g_particleInstances[particleIdx].lifetime = 0;

			Nu3D::Camera::InitViewMatrixGlobals();

			memset(CharacterLoader::g_alternateAllParse, 0, sizeof(CharacterLoader::g_alternateAllParse));
			memset(CharacterLoader::g_charFileDataCache, 0, sizeof(CharacterLoader::g_charFileDataCache));

			g_cachedAllBuffer = 0;

			if (levelId > 10)
			{
				g_levelLoadConfig |= 1;

				if ((g_levelLoadConfig & 192) == 0)
					g_levelLoadConfig |= 64;

				levelIdCpy = levelId - 10;
			}

			int32_t loadConfigCpy = g_levelLoadConfig;
			g_levelLoadConfigCopy = g_levelLoadConfig;

			uint8_t* dataBuffer = g_levelDataHeapBase;

			g_levelDataHeapBasePtr = g_levelDataHeapBase;
			g_levelLoadArena = g_levelDataHeapBase;

			char fileNameBuffer[128];
			fileNameBuffer[0] = '\0';

			if ((loadConfigCpy & 1) == 0)
			{
				int32_t rawVariant = ((loadConfigCpy >> 6) & 3) - 1;
				const char* rawFileName;

				if (rawVariant)
				{
					int32_t rawVariantStep = rawVariant - 1;

					if (rawVariantStep)
					{
						if (rawVariantStep == 1)
						{
							if (levelIdCpy >= 10)
							{
								strcpy(fileNameBuffer, "level");
								fileNameBuffer[5] = levelIdCpy / 10 + 48;
								fileNameBuffer[6] = levelIdCpy % 10 + 48;
							}
							else
							{
								strcpy(fileNameBuffer, "level0");
								fileNameBuffer[6] = levelIdCpy + 48;
							}

							strcpy(&fileNameBuffer[7], "\\");
							rawFileName = "level3.raw";
						}
						else
						{
							if (levelIdCpy >= 10)
							{
								strcpy(fileNameBuffer, "level");
								fileNameBuffer[5] = levelIdCpy / 10 + 48;
								fileNameBuffer[6] = levelIdCpy % 10 + 48;
							}
							else
							{
								strcpy(fileNameBuffer, "level0");
								fileNameBuffer[6] = levelIdCpy + 48;
							}

							strcpy(&fileNameBuffer[7], "\\");
							rawFileName = "level.raw";
						}
					}
					else
					{
						if (levelIdCpy >= 10)
						{
							strcpy(fileNameBuffer, "level");
							fileNameBuffer[5] = levelIdCpy / 10 + 48;
							fileNameBuffer[6] = levelIdCpy % 10 + 48;
						}
						else
						{
							strcpy(fileNameBuffer, "level0");
							fileNameBuffer[6] = levelIdCpy + 48;
						}

						strcpy(&fileNameBuffer[7], "\\");
						rawFileName = "level2.raw";
					}
				}
				else
				{
					if (levelIdCpy >= 10)
					{
						strcpy(fileNameBuffer, "level");
						fileNameBuffer[5] = levelIdCpy / 10 + 48;
						fileNameBuffer[6] = levelIdCpy % 10 + 48;
					}
					else
					{
						strcpy(fileNameBuffer, "level0");
						fileNameBuffer[6] = levelIdCpy + 48;
					}

					strcpy(&fileNameBuffer[7], "\\");
					rawFileName = "level1.raw";
				}

				strcat(fileNameBuffer, rawFileName);
				RawLoader::LoadPacketData(fileNameBuffer);
				loadConfigCpy = g_levelLoadConfig;
			}

			dataBuffer = g_levelLoadArena;

			char levelDigits[4];

			if ((loadConfigCpy & 2) == 0)
			{
				int32_t binVariant = ((loadConfigCpy >> 8) & 3) - 1;

				if (binVariant)
				{
					int32_t binVariantStep = binVariant - 1;

					if (binVariantStep)
					{
						if (binVariantStep == 1)
						{
							strcpy(fileNameBuffer, "..\\level");

							if (levelIdCpy + 10 >= 10)
							{
								levelDigits[2] = 0;
								levelDigits[0] = (levelIdCpy + 10) / 10 + 48;
								levelDigits[1] = (levelIdCpy + 10) % 10 + 48;
							}
							else
							{
								levelDigits[0] = '0';
								levelDigits[1] = levelIdCpy + 58;
								levelDigits[2] = 0;
							}
						}
						else
						{
							strcpy(fileNameBuffer, "..\\level");

							if (levelIdCpy >= 10)
							{
								levelDigits[2] = 0;
								levelDigits[0] = levelIdCpy / 10 + 48;
								levelDigits[1] = levelIdCpy % 10 + 48;
							}
							else
							{
								levelDigits[0] = '0';
								levelDigits[2] = 0;
								levelDigits[1] = levelIdCpy + 48;
							}
						}
					}
					else
					{
						strcpy(fileNameBuffer, "..\\level");

						if (levelIdCpy + 10 >= 10)
						{
							levelDigits[2] = 0;
							levelDigits[0] = (levelIdCpy + 10) / 10 + 48;
							levelDigits[1] = (levelIdCpy + 10) % 10 + 48;
						}
						else
						{
							levelDigits[0] = '0';
							levelDigits[1] = levelIdCpy + 58;
							levelDigits[2] = 0;
						}
					}
				}
				else
				{
					strcpy(fileNameBuffer, "..\\level");

					if (levelIdCpy + 10 >= 10)
					{
						levelDigits[0] = (levelIdCpy + 10) / 10 + 48;
						levelDigits[1] = (levelIdCpy + 10) % 10 + 48;
					}
					else
					{
						levelDigits[0] = 48;
						levelDigits[1] = levelIdCpy + 58;
					}

					levelDigits[2] = 0;
				}

				strcat(fileNameBuffer, levelDigits);
				strcat(fileNameBuffer, ".bin");
			}

			Renderer::Sprite::InitSheets();

			const char* datFileName;
			void* bufferPtr;
			int32_t charLoaderFlag = 0;

			memset(fileNameBuffer, 0, sizeof(fileNameBuffer));

			if ((g_levelLoadConfig & 4) == 0)
			{
				bufferPtr = dataBuffer;
				g_levelDataBase = dataBuffer;

				switch ((g_levelLoadConfig >> 8) & 3)
				{
					case 1:
						if (levelIdCpy >= 10)
						{
							strcpy(fileNameBuffer, "level");
							fileNameBuffer[5] = levelIdCpy / 10 + 48;
							fileNameBuffer[6] = levelIdCpy % 10 + 48;
						}
						else
						{
							strcpy(fileNameBuffer, "level0");
							fileNameBuffer[6] = levelIdCpy + 48;
						}

						strcpy(&fileNameBuffer[7], "\\");
						datFileName = "level1.dat";
						break;

					case 2:
						if (levelIdCpy >= 10)
						{
							strcpy(fileNameBuffer, "level");
							fileNameBuffer[5] = levelIdCpy / 10 + 48;
							fileNameBuffer[6] = levelIdCpy % 10 + 48;
						}
						else
						{
							strcpy(fileNameBuffer, "level0");
							fileNameBuffer[6] = levelIdCpy + 48;
						}

						strcpy(&fileNameBuffer[7], "\\");
						datFileName = "level2.dat";
						break;

					case 3:
						if (levelIdCpy >= 10)
						{
							strcpy(fileNameBuffer, "level");
							fileNameBuffer[5] = levelIdCpy / 10 + 48;
							fileNameBuffer[6] = levelIdCpy % 10 + 48;
						}
						else
						{
							strcpy(fileNameBuffer, "level0");
							fileNameBuffer[6] = levelIdCpy + 48;
						}

						strcpy(&fileNameBuffer[7], "\\");
						datFileName = "level3.dat";
						break;

					default:
						if (levelIdCpy >= 10)
						{
							strcpy(fileNameBuffer, "level");
							fileNameBuffer[5] = levelIdCpy / 10 + 48;
							fileNameBuffer[6] = levelIdCpy % 10 + 48;
						}
						else
						{
							strcpy(fileNameBuffer, "level0");
							fileNameBuffer[6] = levelIdCpy + 48;
						}

						strcpy(&fileNameBuffer[7], "\\");
						datFileName = "level.dat";
						break;
				}

				strcat(fileNameBuffer, datFileName);

				int32_t fileSize = FileUtils::LoadFile(fileNameBuffer, bufferPtr);
				dataBuffer = dataBuffer + fileSize;

				int32_t offset = LoadDAT(levelIdCpy, fileSize);
				dataBuffer = dataBuffer + offset;
			}

			fileNameBuffer[0] = 0;
			FileUtils::AppendRegPathToBuffer();

			int32_t variant;
			int32_t variantStep;
			const char* ngnBaseName;
			int32_t loadConfigCpy2 = g_levelLoadConfig;

			if ((g_levelLoadConfig & 4) == 0 || (g_levelLoadConfig & 1) == 0)
			{
				g_levelDataBase = dataBuffer;

				if (((g_levelLoadConfig >> 8) & 3) == 1)
				{
					if (levelIdCpy >= 10)
					{
						strcpy(fileNameBuffer, "level");
						fileNameBuffer[5] = levelIdCpy / 10 + 48;
						fileNameBuffer[6] = levelIdCpy % 10 + 48;
						loadConfigCpy2 = g_levelLoadConfig;
					}
					else
					{
						strcpy(fileNameBuffer, "level0");
						fileNameBuffer[6] = levelIdCpy + 48;
					}

					strcpy(&fileNameBuffer[7], "\\");
					ngnBaseName = "level1";
				}
				else
				{
					if (((g_levelLoadConfig >> 8) & 3) != 2)
					{
						if (((g_levelLoadConfig >> 8) & 3) == 3)
							BuildLevelPath(levelIdCpy, fileNameBuffer, "level3");
						else
							BuildLevelPath(levelIdCpy, fileNameBuffer, "level");

						loadConfigCpy2 = g_levelLoadConfig;

					LBL_BUILD_TEXTURE_SUFFIX:

						if ((loadConfigCpy2 & 4) == 0 && levelIdCpy && levelId != 16)
							goto LBL_LOAD_NGN;

						variant = ((loadConfigCpy2 >> 6) & 3) - 1;

						const char* tSuffix;

						if (variant)
						{
							variantStep = variant - 1;

							if (variantStep)
							{
								if (variantStep != 1)
								{
								LBL_LOAD_NGN:

									strcat(fileNameBuffer, ".ngn");
									strcat(FileUtils::g_fileNameBuffer, fileNameBuffer);

									NGNLoader::SetNewImage(FileUtils::g_fileNameBuffer);
									NGNLoader::DetectBackdropTextures();

									loadConfigCpy2 = g_levelLoadConfig;
									goto LBL_POST_NGN_LOAD;
								}

								tSuffix = "t3";
							}
							else
							{
								tSuffix = "t2";
							}
						}
						else
						{
							tSuffix = "t1";
						}

						strcat(fileNameBuffer, tSuffix);
						goto LBL_LOAD_NGN;
					}

					if (levelIdCpy >= 10)
					{
						strcpy(fileNameBuffer, "level");
						fileNameBuffer[5] = levelIdCpy / 10 + 48;
						fileNameBuffer[6] = levelIdCpy % 10 + 48;
						loadConfigCpy2 = g_levelLoadConfig;
					}
					else
					{
						strcpy(fileNameBuffer, "level0");
						fileNameBuffer[6] = levelIdCpy + 48;
					}

					strcpy(&fileNameBuffer[7], "\\");
					ngnBaseName = "level2";
				}

				strcat(fileNameBuffer, ngnBaseName);
				goto LBL_BUILD_TEXTURE_SUFFIX;
			}

		LBL_POST_NGN_LOAD:

			if ((loadConfigCpy2 & 8) == 0)
			{
				if (! g_cachedAllBuffer)
				{
					Collision::BuildCollisionWorld(levelIdCpy, &dataBuffer, (loadConfigCpy2 >> 8) & 3);
					loadConfigCpy2 = (loadConfigCpy2 & ~0xFF) | (g_levelLoadConfig & 0xFF);
				}

				if ((loadConfigCpy2 & 8) == 0)
				{
					Collectables::BuildPickupTable();
					loadConfigCpy2 = (loadConfigCpy2 & ~0xFF) | (g_levelLoadConfig & 0xFF);
				}
			}

			if ((loadConfigCpy2 & 16) == 0)
			{
				uint8_t creatureIdList[128];
				Actor::GetCreatureList(creatureIdList);
				CharacterLoader::Start(&charLoaderFlag, &dataBuffer, creatureIdList);
			}

			g_levelLoadArena = dataBuffer;
			g_levelLoadConfig = 0;

			Logger::Log("InitLevelPlay : END.\n");
		}
	}
}