/*
	Console.cpp
	This file is part of VCMP Server.

    VCMP Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VCMP Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VCMP Server.  If not, see <http://www.gnu.org/licenses/>.

	--------------------------------------------------------------------

    VC:MP Server
		A server for VC:MP - A Multiplayer Modification For GTA:VC
    Copyright 2009 VRocker, Knight2k9 and Juppi

	File Author: VRocker

	--------------------------------------------------------------------
*/

#include "Console.h"
#include <iostream>

void CConsole::Print( const char* szText, CONSOLE_COLOURS cCol, ... )
{
	char sz[ MAX_CONSOLELINE_LEN ] = {0};

	va_list marker;
	va_start( marker, cCol );
	vsnprintf( sz, sizeof( sz ), szText, marker );
	va_end( marker );

	switch ( cCol )
	{
		case COLOUR_WHITE:
			ConsoleWhite
			break;

		case COLOUR_GREY:
			ConsoleGrey
			break;

		case COLOUR_RED:
			ConsoleRed
			break;

		case COLOUR_GREEN:
			ConsoleGreen
			break;

		case COLOUR_BLUE:
			ConsoleBlue
			break;

		case COLOUR_YELLOW:
			ConsoleYellow
			break;

		case COLOUR_CYAN:
			ConsoleCyan
			break;

		case COLOUR_MAGENTA:
			ConsoleMagenta
			break;

		case COLOUR_DARKGREEN:
			ConsoleDarkGreen
			break;

		default:
			ConsoleWhite
			break;
	};

	printf( sz );
}

void CConsole::Print2( const char* szText, CONSOLE_COLOURS cCol )
{
	switch ( cCol )
	{
		case COLOUR_WHITE:
			ConsoleWhite
			break;

		case COLOUR_GREY:
			ConsoleGrey
			break;

		case COLOUR_RED:
			ConsoleRed
			break;

		case COLOUR_GREEN:
			ConsoleGreen
			break;

		case COLOUR_BLUE:
			ConsoleBlue
			break;

		case COLOUR_YELLOW:
			ConsoleYellow
			break;

		case COLOUR_CYAN:
			ConsoleCyan
			break;

		case COLOUR_MAGENTA:
			ConsoleMagenta
			break;

		case COLOUR_DARKGREEN:
			ConsoleDarkGreen
			break;

		default:
			ConsoleWhite
			break;
	};

	std::cout << szText;
}

void CConsole::PrintC( const char* szNote, CONSOLE_COLOURS cCol, const char* szText, ... )
{
	char sz[ MAX_CONSOLELINE_LEN ] = {0};
	va_list marker;
	va_start ( marker, szText );
	vsnprintf( sz, sizeof( sz ), szText, marker );
	va_end ( marker );

	strcat( sz, "\n" );

	Print( "%s: ", cCol, szNote );
	Print( sz, COLOUR_WHITE );
}

void CConsole::PrintC2( const char* szNote, CONSOLE_COLOURS cCol, const char* szText )
{
	char sz[ MAX_CONSOLELINE_LEN ] = {0};
	strcpy( sz, szText );
	strcat( sz, "\n" );

	Print( "%s: ", cCol, szNote );
	Print2( sz, COLOUR_WHITE );
}

void CConsole::PrintTime( const char* szText, CONSOLE_COLOURS cCol, unsigned int iTime, ... )
{
	char sz[ MAX_CONSOLELINE_LEN ] = {0};
	va_list marker;
	va_start ( marker, iTime );
	vsnprintf( sz, sizeof( sz ), szText, marker );
	va_end ( marker );

	int i = 0;
	for ( i = 0; i < (int)strlen(sz); i++ )
	{
		Print( "%c", cCol, sz[i]);
#if defined( WIN32 ) || defined( WIN64 )
		Sleep( iTime );
#else
		usleep( ( iTime * 1000 ) );
#endif
	}

	Print( "\n", COLOUR_WHITE );
}
