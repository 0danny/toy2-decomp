#pragma once

#include "Common.h"

namespace FileUtils
{
	int32_t GetFileSize(const char* fileName);
    
	void AppendCDPath(char* path);
	void AppendRegPathToBuffer();
	void GetPathValue(char* pathOut);

	size_t LoadFile(const char* fileName, void* buffer);
	void ValidateInstall();
}