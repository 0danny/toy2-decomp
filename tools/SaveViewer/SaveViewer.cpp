#include "SaveViewer.hpp"
#include "Common/Utils.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <filesystem>

namespace SaveViewer
{
	struct [[gnu::packed]] SaveControlMapping
	{
		int32_t m_dInputCode;
		int32_t m_controlSettingId;
	};

	struct [[gnu::packed]] Save99Data
	{
		int32_t            m_strSize;
		int32_t            m_unused1;
		int32_t            m_unused2;
		SaveControlMapping m_mappings[38];
		uint8_t            m_unusedPad[80];
	};

	void readSave(const std::string& savePath)
	{
		const auto fileName = std::filesystem::path(savePath).filename().string();

		printf("[SaveViewer] Reading sav file -> %s\n", fileName.c_str());

		std::ifstream file(savePath, std::ios::binary | std::ios::ate);
		if ( ! file.is_open() )
		{
			printf("[SaveViewer] Could not open file %s\n", savePath.c_str());
			return;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		Save99Data saveData{};

		assert(sizeof(Save99Data) == size);

		if ( ! file.read(reinterpret_cast<char*>(&saveData), size) )
		{
			printf("[SaveViewer] Failed to read file.\n");
			return;
		}

		printf("------------------ SAVE DATA ------------------\n");
		printf("m_strSize: %d\n", saveData.m_strSize);
		printf("m_unused1: %d\n", saveData.m_unused1);
		printf("m_unused2: %d\n", saveData.m_unused2);

		int32_t index = 1;

		for ( SaveControlMapping mapping : saveData.m_mappings )
		{
			if ( mapping.m_dInputCode == -1 ) continue;

			auto gameControl = std::find_if(
			    Utils::g_gameControls, Utils::g_gameControls + Utils::g_gameControlsCount,
			    [&mapping](const Utils::GameControl& control) {
				    return control.m_id == mapping.m_dInputCode;
			    }
			);

			auto controlSetting = std::find_if(
			    Utils::g_controlSettings, Utils::g_controlSettings + Utils::g_controlSettingsCount,
			    [&mapping](const Utils::ControlSetting& control) {
				    return control.m_id == mapping.m_controlSettingId;
			    }
			);

			if ( controlSetting == Utils::g_controlSettings + Utils::g_controlSettingsCount )
			{
				printf("Control Setting Id %d, was not found for %s\n", mapping.m_controlSettingId, gameControl->m_name);
				continue;
			}

			printf("Mapping %d: [%s, %s]\n", index, gameControl->m_name, controlSetting->m_name);
			index++;
		}
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
