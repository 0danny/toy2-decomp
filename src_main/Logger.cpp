#include "Logger.h"

#include <cstdio>
#include <cstdarg>
#include <stdlib.h>
#include <windows.h>

namespace Logger
{
	static char* g_errorHandlerPath;
	static int32_t g_errorHandlerLine;

	int32_t g_showMsgBoxOnThrow;

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
}
