#include "Logger.h"

#include <cstdio>
#include <cstdarg>

namespace Logger
{
	// $GLOBAL 00B6269C
	const char* g_errorHandlerPath;

	// $GLOBAL 00B626A0
	int32_t g_errorHandlerLine;

	// $GLOBAL 00883348
	int32_t g_showMsgBoxOnThrow;

	// $GLOBAL 00882F38
	int32_t g_logsEnabled;

	// $GLOBAL 00504E54
	int32_t g_logFileExists = 1;
}

namespace Logger
{
	// $FUNC 004A87C0 [IMPLEMENTED]
	ThrowErrorFunc GetErrorHandler(char* filePath, int32_t lineNumber)
	{
		g_errorHandlerPath = filePath;
		g_errorHandlerLine = lineNumber;

		return ThrowError;
	}

	// $FUNC 004A8710 [IMPLEMENTED]
	void ThrowError(char* format, ...)
	{
		char caption[256];
		char text[1024];

		va_list argList;
		va_start(argList, format);

		if (*format)
		{
			sprintf(caption, "Soft Abort - %s Line %d", g_errorHandlerPath, g_errorHandlerLine);
			vsprintf(text, format, argList);

			FILE* file = fopen("toy2.err", "wb");

			if (file)
			{
				fprintf(file, "%s\r\n%s\r\n", caption, text);
				fclose(file);
			}

			if (! g_showMsgBoxOnThrow)
				MessageBoxA(0, text, caption, 0);
		}

		exit(-1);
	}

	// $FUNC 004A66A0 [IMPLEMENTED] [MODIFIED]
	void Log(char* format, ...)
	{
		char buffer[1024];

		va_list argList;
		va_start(argList, format);

		memset(buffer, 0, sizeof(buffer));
		vsprintf(buffer, format, argList);

		printf("%s", buffer); // Addition

		if (g_logsEnabled)
		{
			if (g_logFileExists)
			{
				g_logFileExists = 0;
				remove("toy2.log");
			}

			FILE* file = fopen("toy2.log", "at");

			if (file)
			{
				fprintf(file, buffer);
				fclose(file);
			}
		}
	}

	// $FUNC 004A6730 [IMPLEMENTED]
	void LogLn(char* format, ...)
	{
		char buffer[1024];

		va_list argList;
		va_start(argList, format);

		memset(buffer, 0, sizeof(buffer));
		vsprintf(buffer, format, argList);
		lstrcatA(buffer, "\r\n");

		Log(buffer);
	}

	// $FUNC 00431900 [IMPLEMENTED]
	void LogDDError(const char* message, HRESULT error)
	{
		char buffer[2048];

		memset(buffer, 0, sizeof(buffer));
		char* errorToMsg = ErrorToMessage(error);
		sprintf(buffer, "ERROR - %s\nERROR - %s\n", message, errorToMsg);

		LogLn(buffer);
	}

	// $FUNC 0040D490 [UNFINISHED];
	char* ErrorToMessage(HRESULT error) { return "Unimplemented"; }
}
