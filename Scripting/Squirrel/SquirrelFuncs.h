#ifndef _SQUIRRELFUNCS_H
#define _SQUIRRELFUNCS_H

#include "SquirrelMain.h"

class CSquirrelFuncs
{
public:
	static SQInteger				RegisterGlobalFunc					( HSQUIRRELVM v, SQFUNCTION f, const char *fname, unsigned char ucParams = 0, char* szParams = 0 );
	static void						RegisterFuncs						( HSQUIRRELVM v );

	static void						ClassBinds							( void );
};

#endif
