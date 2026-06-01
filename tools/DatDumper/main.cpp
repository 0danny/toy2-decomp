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

	struct Type63
	{
		Vector3I vec;
		uint32_t spriteIndex;
	};

	int32_t g_type64Count = 0;
}

void dumpFile(const std::string& filePath)
{
	FileReader fileReader(filePath);

	uint64_t totalGroups = fileReader.read<uint32_t>();

	std::println("Total Groups - {}", totalGroups);

	while (totalGroups && ! fileReader.atEOF())
	{
		auto recordCount = fileReader.read<int16_t>();
		auto recordType = fileReader.read<int16_t>();

		bool negative = false;

		std::println("------------ Group {} ------------", totalGroups);

		std::println("recordCount -> {}", recordCount);
		std::println("recordType -> {}", recordType);

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
			negative = true;
		}

		// subtract header size (4 bytes)
		recordSize -= sizeof(int16_t) + sizeof(int16_t);

		std::println("recordSize ({}) -> {}", negative ? "negative" : "positive", recordSize);

		if (recordType == 63)
		{
			for (int32_t idx = 0; idx < recordCount; idx++)
			{
				Type63 data = fileReader.read<Type63>();
				
				printf("[%d] x %u, y %u, z %u, sprite index %d\n", (idx + 1), data.vec.x, data.vec.y, data.vec.z, data.spriteIndex);
			}
		}
		else
		{
			for (int32_t idx = 0; idx < recordCount; idx++)
			{
				Vector3I data = fileReader.read<Vector3I>();
				
				printf("[%d] x %u, y %u, z %u\n", (idx + 1), data.x, data.y, data.z);
			}
		}

		totalGroups--;
	}

	if (totalGroups == 0)
		std::println("Hit end of total groups count");

	std::println("Offset {}, Size {}", fileReader.getOffset(), fileReader.getFileSize());

	// next section
	// game perma skips over whatever this data is
	int32_t count = fileReader.read<int32_t>() << 7;
	int32_t size = 4 * (count / 4) + 4;

	std::println("Unused Count -> {}", count);
	std::println("Unused Size -> {}", size);

	
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
