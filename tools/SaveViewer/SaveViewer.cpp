#include "SaveViewer.hpp"
#include "Common/GameStructs.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace SaveViewer
{
	void readSave(const std::string& savePath)
	{
		const auto filePath = std::filesystem::path(savePath);
		const auto fileName = filePath.filename().string();
		const auto fileExtension = filePath.extension().string();

		if (fileExtension != ".sav")
		{
			printf("[SaveViewer] Not a valid .sav file.\n");
			return;
		}

		printf("[SaveViewer] Reading sav file -> %s\n", fileName.c_str());

		std::ifstream file(savePath, std::ios::binary | std::ios::ate);
		if (! file.is_open())
		{
			printf("[SaveViewer] Could not open file %s\n", savePath.c_str());
			return;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		// Read first 4 bytes to determine whether this is a normal save file or a 99.sav
		int32_t strSize { 0 };

		if (! file.read(reinterpret_cast<char*>(&strSize), sizeof(int32_t)))
		{
			printf("[SaveViewer] Failed to read file.\n");
			return;
		}

		printf("[SaveViewer] String size is %d\n", strSize);

		SaveType type = (strSize == 0 ? SaveType::ControlData : SaveType::ProgressionData);

		if (type == SaveType::ControlData)
			readControlData(file);
		else if (type == SaveType::ProgressionData)
			readProgressionData(file, strSize);
	}

	void readProgressionData(std::ifstream& file, const int32_t strSize)
	{
		// read the save name string in.
		std::vector<char> saveName(strSize, 0);

		if (! file.read(saveName.data(), strSize))
		{
			printf("[SaveViewer] Failed to read progression data save name.\n");
			return;
		}

		Save0Data progData {};

		if (! file.read(reinterpret_cast<char*>(&progData), sizeof(Save0Data)))
		{
			printf("[SaveViewer] Failed to read progression data.\n");
			return;
		}

		printf("------------------ Progression Data ------------------\n");
		printf("Save Name: %s\n", saveName.data());
		printf("unkInt1: %u\n", progData.unkInt1);
		printf("unkInt2: %u\n", progData.unkInt2);
		printf("lives: %u\n", progData.lives);
		printf("lastLevel: %u\n", progData.lastLevel);
		printf("unlocks: %u\n", progData.unlocks);
		printf("unkByte2_2: %u\n", progData.unkByte2_2);
		printf("cameraType: %u\n", progData.cameraType);
		printf("musicVolume: %u\n", progData.musicVolume);
		printf("soundVolume: %u\n", progData.soundVolume);
		printf("unkByte4_2: %u\n", progData.unkByte4_2);
		printf("unkShort3: %u\n", progData.unkShort3);
		printf("unkShort4: %u\n", progData.unkShort4);
		printf("health: %u\n", progData.health);
		printf("unkByte5: %u\n", progData.unkByte5);

		printf("tokens: [ ");
		for (uint8_t token : progData.tokens)
		{
			printf("%u ", token);
		}
		printf("]\n");

		// uint8_t  unkBytes[48];

		printf("unkByte8: %u\n", progData.unkByte8);
	}

	void readControlData(std::ifstream& file)
	{
		Save99Data controlData {};

		if (! file.read(reinterpret_cast<char*>(&controlData), sizeof(Save99Data)))
		{
			printf("[SaveViewer] Failed to read control data.\n");
			return;
		}

		printf("------------------ Control Data ------------------\n");
		printf("unused1: %d\n", controlData.unused1);
		printf("unused2: %d\n", controlData.unused2);

		int32_t index = 1;
		int32_t unusedControls = 0;

		for (SaveControlMapping mapping : controlData.mappings)
		{
			if (mapping.dInputCode == -1)
			{
				unusedControls++;
				continue;
			}

			auto inputMapping = std::find_if(g_inputMapping, g_inputMapping + g_inputMappingCount, [&mapping](const InputMapping& item) {
				return item.id == mapping.dInputCode;
			});

			auto gameControl = std::find_if(g_gameControls, g_gameControls + g_gameControlsCount, [&mapping](const GameControl& item) {
				return item.id == mapping.gameControlId;
			});

			printf("Mapping %d: \"%s\" mapped to \"%s\"\n", index, inputMapping->name, gameControl->name);
			index++;
		}

		printf("%d Unused Mappings\n", unusedControls);
	}
}

int32_t main(int32_t argc, char* argv[])
{
	if (argc == 1)
	{
		printf("[SaveViewer] Please provide the filename of the save.\n");
		return 1;
	}

	SaveViewer::readSave(std::string(argv[1]));

	return 0;
}
