#pragma once

#include "Common.h"
#include <windows.h>

namespace Logger
{
	extern int32_t g_showMsgBoxOnThrow;
	extern int32_t g_logsEnabled;
	extern int32_t g_logFileExists;

	typedef void (*ThrowErrorFunc)(char* format, ...);

	ThrowErrorFunc GetErrorHandler(char* filePath, int32_t lineNumber);
	void ThrowError(char* format, ...);

	void Log(char* format, ...);
	void LogLn(char* format, ...);
	void LogDDError(const char* message, HRESULT error);

	char* ErrorToMessage(HRESULT error);
}
