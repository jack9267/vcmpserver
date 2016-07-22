#ifndef _MAIN_H
#define _MAIN_H

#include <string.h>

#if defined( WIN32 ) || defined( _WIN64 )

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <process.h>

#else

#include <dlfcn.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef char* PCHAR;
typedef long long __int64;
typedef int HANDLE;

#endif

#include "Entity.h"
#include "GetTime.h"
#include "Console.h"
#include "SharedDefs.h"
#include "Scripting.h"
#include "ConfigLoader.h"

void ProcessConsoleInput( char* sz );
void FilterInvalidNickChars( char* szString );

typedef struct _SERVER_INFO
{
	char szServerReply[ 11 ];
	unsigned char ucPlayers;
	unsigned char ucMaxPlayers;
	unsigned char ucServerNameLen;
	unsigned char bLocked;
	char szServerName[ 128 ];
	char szMode[ 64 ];
	char szMap[ 32 ];
} SERVER_INFO;

#endif
