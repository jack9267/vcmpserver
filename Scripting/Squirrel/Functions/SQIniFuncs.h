#ifndef _SQINIFUNCS_H
#define _SQINIFUNCS_H

#include "SquirrelMain.h"
#include <rudeconfig.h>

using namespace rude;

class CSQIniFuncs
{
public:
	static _SQUIRRELDEF( ReadString );
	static _SQUIRRELDEF( ReadInteger );
	static _SQUIRRELDEF( ReadNumber );
	static _SQUIRRELDEF( ReadBool );

	static _SQUIRRELDEF( WriteString );
	static _SQUIRRELDEF( WriteInteger );
	static _SQUIRRELDEF( WriteNumber );
	static _SQUIRRELDEF( WriteBool );

	static _SQUIRRELDEF( RemoveValue );

	static _SQUIRRELDEF( CountSection );

	static _SQUIRRELDEF( DeleteSection );
	static _SQUIRRELDEF( Clear );
};

#endif
