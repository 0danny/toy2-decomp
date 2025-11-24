#pragma once

#include "Common.h"
#include <windows.h>

namespace Logger
{
	extern int32_t g_showMsgBoxOnThrow;
	extern int32_t g_logsEnabled;
	extern int32_t g_logFileExists;

	typedef void (*ThrowErrorFunc)(char* p_format, ...);

	ThrowErrorFunc GetErrorHandler(char* p_filePath, int32_t p_lineNumber);
	void ThrowError(char* p_format, ...);

	void Log(char* p_format, ...);
	void LogLn(char* p_format, ...);
	void LogDDError(const char* p_message, HRESULT p_error);

	char* ErrorToMessage(HRESULT p_error);
}
