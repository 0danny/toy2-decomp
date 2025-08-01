#include "SaveViewer.hpp"
#include "Common/Utils.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace SaveViewer
{
	void readSave(const std::string& savePath)
	{
		const auto filePath      = std::filesystem::path(savePath);
		const auto fileName      = filePath.filename().string();
		const auto fileExtension = filePath.extension().string();

		if ( fileExtension != ".sav" )
		{
			printf("[SaveViewer] Not a valid .sav file.\n");
			return;
		}

		printf("[SaveViewer] Reading sav file -> %s\n", fileName.c_str());

		std::ifstream file(savePath, std::ios::binary | std::ios::ate);
		if ( ! file.is_open() )
		{
			printf("[SaveViewer] Could not open file %s\n", savePath.c_str());
			return;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		// Read first 4 bytes to determine whether this is a normal save file or a 99.sav
		int32_t strSize{ 0 };

		if ( ! file.read(reinterpret_cast<char*>(&strSize), sizeof(int32_t)) )
		{
			printf("[SaveViewer] Failed to read file.\n");
			return;
		}

		printf("[SaveViewer] String size is %d\n", strSize);

		SaveType type = (strSize == 0 ? SaveType::ControlData : SaveType::ProgressionData);

		if ( type == SaveType::ControlData )
			readControlData(file);
		else if ( type == SaveType::ProgressionData )
			readProgressionData(file, strSize);
	}

	void readProgressionData(std::ifstream& file, const int32_t strSize)
	{
		//read the save name string in.
		std::vector<char> saveName(strSize, 0);

		if ( ! file.read(saveName.data(), strSize) )
		{
			printf("[SaveViewer] Failed to read progression data save name.\n");
			return;
		}

		Save0Data progData{};

		if ( ! file.read(reinterpret_cast<char*>(&progData), sizeof(Save0Data)) )
		{
			printf("[SaveViewer] Failed to read progression data.\n");
			return;
		}

		printf("------------------ Progression Data ------------------\n");
		printf("Save Name: %s\n", saveName.data());
		printf("m_unkInt1: %u\n", progData.m_unkInt1);
		printf("m_unkInt2: %u\n", progData.m_unkInt2);
		printf("m_lives: %u\n", progData.m_lives);
		printf("m_levelIndex: %u\n", progData.m_levelIndex);
		printf("m_unk: %u\n", progData.m_unk);
		printf("m_unkByte2_2: %u\n", progData.m_unkByte2_2);
		printf("m_cameraType: %u\n", progData.m_cameraType);
		printf("m_musicVolume: %u\n", progData.m_musicVolume);
		printf("m_soundVolume: %u\n", progData.m_soundVolume);
		printf("m_unkByte4_2: %u\n", progData.m_unkByte4_2);
		printf("m_unkShort3: %u\n", progData.m_unkShort3);
		printf("m_unkShort4: %u\n", progData.m_unkShort4);
		printf("m_health: %u\n", progData.m_health);
		printf("m_unkByte5: %u\n", progData.m_unkByte5);

		printf("m_tokens: [ ");
		for ( uint8_t token : progData.m_tokens )
		{
			printf("%u ", token);
		}
		printf("]\n");

		//uint8_t  m_unkBytes[48];

		printf("m_unkByte8: %u\n", progData.m_unkByte8);
	}

	void readControlData(std::ifstream& file)
	{
		Save99Data controlData{};

		if ( ! file.read(reinterpret_cast<char*>(&controlData), sizeof(Save99Data)) )
		{
			printf("[SaveViewer] Failed to read control data.\n");
			return;
		}

		printf("------------------ Control Data ------------------\n");
		printf("m_unused1: %d\n", controlData.m_unused1);
		printf("m_unused2: %d\n", controlData.m_unused2);

		int32_t index          = 1;
		int32_t unusedControls = 0;

		for ( SaveControlMapping mapping : controlData.m_mappings )
		{
			if ( mapping.m_dInputCode == -1 )
			{
				unusedControls++;
				continue;
			}

			auto inputMapping = std::find_if(
			    Utils::g_inputMapping, Utils::g_inputMapping + Utils::g_inputMappingCount,
			    [&mapping](const Utils::InputMapping& item) {
				    return item.m_id == mapping.m_dInputCode;
			    }
			);

			auto gameControl = std::find_if(
			    Utils::g_gameControls, Utils::g_gameControls + Utils::g_gameControlsCount,
			    [&mapping](const Utils::GameControl& item) {
				    return item.m_id == mapping.m_gameControlId;
			    }
			);

			printf("Mapping %d: \"%s\" mapped to \"%s\"\n", index, inputMapping->m_name, gameControl->m_name);
			index++;
		}

		printf("%d Unused Mappings\n", unusedControls);
	}
}

int32_t main(int32_t argc, char* argv[])
{
	if ( argc == 1 )
	{
		printf("[SaveViewer] Please provide the filename of the save.\n");
		return 1;
	}

	SaveViewer::readSave(std::string(argv[1]));

	return 0;
}
