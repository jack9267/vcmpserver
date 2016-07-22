#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "Main.h"
#include "ConsoleDefs.h"

class CConsole
{
public:
	static void							Print							( const char* szText, CONSOLE_COLOURS cCol, ... );
	static void							Print2							( const char* szText, CONSOLE_COLOURS cCol );

	static void							PrintC							( const char* szNote, CONSOLE_COLOURS cCol, const char* szText, ... );
	static void							PrintC2							( const char* szNote, CONSOLE_COLOURS cCol, const char* szTexts );

	static void							PrintTime						( const char* szText, CONSOLE_COLOURS cCol, unsigned int iTime, ... );
};

#endif
