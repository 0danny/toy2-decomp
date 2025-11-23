#pragma once

#include "Common.h"

namespace Logger
{
	extern int32_t g_showMsgBoxOnThrow;

	typedef void (*ThrowErrorFunc)(char* p_format, ...);

	ThrowErrorFunc GetErrorHandler(char* p_filePath, int32_t p_lineNumber);
	void ThrowError(char* p_format, ...);
}
