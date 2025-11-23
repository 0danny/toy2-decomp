#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace SaveViewer
{
	struct [[gnu::packed]] SaveControlMapping
	{
		int32_t m_dInputCode;
		int32_t m_gameControlId;
	};

	struct [[gnu::packed]] Save99Data
	{
		int32_t            m_unused1;
		int32_t            m_unused2;
		SaveControlMapping m_mappings[38];
		uint8_t            m_unusedPad[80];
	};

	struct [[gnu::packed]] Save0Data
	{
		uint32_t m_unkInt1;
		uint32_t m_unkInt2;
		uint8_t  m_zeros[304];
		uint8_t  m_lives;
		uint8_t  m_lastLevel;
		uint8_t  m_unlocks;
		uint8_t  m_unkByte2_2;
		uint8_t  m_cameraType;
		uint8_t  m_musicVolume;
		uint8_t  m_soundVolume;
		uint8_t  m_unkByte4_2;
		uint16_t m_unkShort3;
		uint16_t m_unkShort4;
		uint16_t m_health;
		uint8_t  m_unkByte5;
		uint8_t  m_tokens[16];
		uint8_t  m_unkBytes[48];
		uint8_t  m_unkByte8;
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
