#ifndef MAIN_H
#define MAIN_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <process.h>

#include "Defs.h"
#include "Entity.h"

void CallSignal( char* szAlias, char* szParams, ... );
void CallAlias( char* szAlias, char* szParams, ... );
void CallFunc( char* szAlias, char* szParams = NULL, ... );

#endif