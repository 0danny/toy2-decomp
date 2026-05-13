#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace SaveViewer
{
	struct [[gnu::packed]] SaveControlMapping
	{
		int32_t dInputCode;
		int32_t gameControlId;
	};

	struct [[gnu::packed]] Save99Data
	{
		int32_t unused1;
		int32_t unused2;
		SaveControlMapping mappings[38];
		uint8_t unusedPad[80];
	};

	struct [[gnu::packed]] Save0Data
	{
		uint32_t unkInt1;
		uint32_t unkInt2;
		uint8_t zeros[304];
		uint8_t lives;
		uint8_t lastLevel;
		uint8_t unlocks;
		uint8_t unkByte2_2;
		uint8_t cameraType;
		uint8_t musicVolume;
		uint8_t soundVolume;
		uint8_t unkByte4_2;
		uint16_t unkShort3;
		uint16_t unkShort4;
		uint16_t health;
		uint8_t unkByte5;
		uint8_t tokens[16];
		uint8_t unkBytes[48];
		uint8_t unkByte8;
	};

	enum class SaveType
	{
		ControlData,
		ProgressionData,
	};

	void readSave(const std::string& savePath);
	void readControlData(std::ifstream& file);
	void readProgressionData(std::ifstream& file, const int32_t strSize);
}
