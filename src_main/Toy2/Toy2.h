#pragma once

#include "Common.h"

namespace Toy2
{
	void OneInit();
	void CheckForQuit();

	int32_t ReadCfg();
	int32_t ShowModeSelect();

	int32_t Run(int32_t p_argCount, char** p_argList);
}
