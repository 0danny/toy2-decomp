#include "Logger.h"

#include <cstdio>
#include <cstdarg>
#include <stdlib.h>

namespace Logger
{
	static char* g_errorHandlerPath;
	static int32_t g_errorHandlerLine;

	int32_t g_showMsgBoxOnThrow;
	int32_t g_logsEnabled;
	int32_t g_logFileExists = 1;

	// $FUNC 004A87C0 [IMPLEMENTED]
	ThrowErrorFunc GetErrorHandler(char* p_filePath, int32_t p_lineNumber)
	{
		g_errorHandlerPath = p_filePath;
		g_errorHandlerLine = p_lineNumber;

		return ThrowError;
	}

	// $FUNC 004A8710 [IMPLEMENTED]
	void ThrowError(char* p_format, ...)
	{
		char l_caption[256];
		char l_text[1024];

		va_list l_argList;
		va_start(l_argList, p_format);

		if ( *p_format )
		{
			sprintf(l_caption, "Soft Abort - %s Line %d", g_errorHandlerPath, g_errorHandlerLine);
			vsprintf(l_text, p_format, l_argList);

			FILE* l_file = fopen("toy2.err", "wb");

			if ( l_file )
			{
				fprintf(l_file, "%s\r\n%s\r\n", l_caption, l_text);
				fclose(l_file);
			}

			if ( ! g_showMsgBoxOnThrow )
				MessageBoxA(0, l_text, l_caption, 0);
		}

		exit(-1);
	}

	// $FUNC 004A66A0 [IMPLEMENTED] [MODIFIED]
	void Log(char* p_format, ...)
	{
		char l_buffer[1024];

		va_list l_argList;
		va_start(l_argList, p_format);

		memset(l_buffer, 0, sizeof(l_buffer));
		vsprintf(l_buffer, p_format, l_argList);

		printf("%s", l_buffer);

		if ( g_logsEnabled )
		{


			if ( g_logFileExists )
			{
				g_logFileExists = 0;
				remove("toy2.log");
			}

			FILE* l_file = fopen("toy2.log", "at");

			if ( l_file )
			{
				fprintf(l_file, l_buffer);
				fclose(l_file);
			}
		}
	}

	// $FUNC 004A6730 [IMPLEMENTED]
	void LogLn(char* p_format, ...)
	{
		char l_buffer[1024];

		va_list l_argList;
		va_start(l_argList, p_format);

		memset(l_buffer, 0, sizeof(l_buffer));
		vsprintf(l_buffer, p_format, l_argList);
		lstrcatA(l_buffer, "\r\n");

		Log(l_buffer);
	}

	// $FUNC 00431900 [IMPLEMENTED]
	void LogDDError(const char* p_message, HRESULT p_error)
	{
		char l_buffer[2048];

		memset(l_buffer, 0, sizeof(l_buffer));
		char* l_message = ErrorToMessage(p_error);
		sprintf(l_buffer, "ERROR - %s\nERROR - %s\n", p_message, l_message);

		LogLn(l_buffer);
	}

	// $FUNC 0040D490 [Unimplemented];
	char* ErrorToMessage(HRESULT p_error) { return "Unimplemented"; }
}
