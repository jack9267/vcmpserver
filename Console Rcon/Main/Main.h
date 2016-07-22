#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <stdio.h>
#include "SharedDefs.h"

typedef struct _GAME_SETTINGS {
	unsigned short usPort;								// Sets the port to use
	char szIP[ 64 + 1 ];				// Sets the IP to connect to
	char szPass[ 32 + 1 ];		// Sets the server password (if applicable)
	char szNick[ 32 + 1 ];		// Sets the nickname to use on the server
} GAME_SETTINGS;

#endif
