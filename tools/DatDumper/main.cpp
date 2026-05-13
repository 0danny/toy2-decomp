#include <print>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <vector>

#include "Common/FileReader.hpp"
#include "Common/GameStructs.hpp"

namespace
{
	struct Type64
	{
		Vector3I unkVar0;
		Vector3I unkVar3;
		int32_t unkVar6;
		void* recordPtr;
		void* unkVar8;
	};

	int32_t g_type64Count = 0;
}

void dumpFile(const std::string& filePath)
{
	FileReader fileReader(filePath);

	uint64_t totalRecords = fileReader.read<uint32_t>();

	std::println("Total Records - {}", totalRecords);

	while (totalRecords && ! fileReader.atEOF())
	{
		auto recordCount = fileReader.read<int16_t>();
		auto recordType = fileReader.read<int16_t>();

		std::println("------------ Record {} ------------", totalRecords);

		uint64_t recordSize = 0;

		if (recordType == 63)
		{
			// type 63 special case
			recordSize = 16 * recordCount + 4;
		}
		else if (recordType >= 0)
		{
			// positive types
			recordSize = 12 * recordCount + 4;
		}
		else
		{
			// negative types
			recordSize = 4 * ((3 * recordCount + 1) / 2) + 16;
		}

		// subtract header size (4 bytes)
		recordSize -= sizeof(int16_t) + sizeof(int16_t);

		std::println("recordCount -> {}", recordCount);
		std::println("recordType -> {}", recordType);
		std::println("recordSize -> {}", recordSize);

		if (recordType == 64)
		{
			std::println("Found a type 64!");
			g_type64Count = 0;
		}

		fileReader.seek(recordSize);
		totalRecords--;
	}

	if (totalRecords == 0)
		std::println("Hit end of total records count");

	std::println("Offset {}, Size {}", fileReader.getOffset(), fileReader.getFileSize());
}

int main(int argc, char* argv[])
{
	std::println("Toy 2 DatDumper 0.1 - Danny");

	if (argc < 2)
	{
		std::println("Usage: {} <input_file>", argv[0]);
		return 1;
	}

	try
	{
		dumpFile(argv[1]);
	}
	catch (const std::exception& e)
	{
		std::println("[Exception]: {}", e.what());
	}

	return 0;
}
