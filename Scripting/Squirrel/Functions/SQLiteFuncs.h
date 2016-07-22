/*
 * SQLiteFuncs.h
 * Edited: 23/3/2009
 */

#ifndef _SQLITEFUNCS_H
#define _SQLITEFUNCS_H

#include "SquirrelMain.h"
#include "sqlite3.h"

class CSQLiteFuncs
{
public:
	static _SQUIRRELDEF( Connect );
	static _SQUIRRELDEF( Close );

	static _SQUIRRELDEF( Query );
	static _SQUIRRELDEF( GetNextRow );
	static _SQUIRRELDEF( GetRowColumns );
	static _SQUIRRELDEF( GetColumnResult );
};

#endif