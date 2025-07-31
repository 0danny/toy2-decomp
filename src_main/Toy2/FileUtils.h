#pragma once

#include "Common.h"

namespace FileUtils
{
	extern char    g_pathRegValue[512];
	extern char    g_cdPathRegValue[512];
	extern int32_t g_registryKeysRead;

	void ValidateInstall();
}
