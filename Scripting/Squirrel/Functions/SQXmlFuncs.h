/*
 * SQXmlFuncs.h
 * Edited: 20/4/2009
 */

#ifndef _SQXMLFUNCS_H
#define _SQXMLFUNCS_H

#include "SquirrelMain.h"
#include "tinyxml.h"



class CSQXmlFuncs
{
public:
	static _SQUIRRELDEF( CreateDoc );
	static _SQUIRRELDEF( LoadDoc );
	static _SQUIRRELDEF( SaveDoc );

	static _SQUIRRELDEF( CreateElement );
	static _SQUIRRELDEF( RemoveElement );
	static _SQUIRRELDEF( GetElement );
	static _SQUIRRELDEF( GetParentElement );
	static _SQUIRRELDEF( GetNextSiblingElement );
	static _SQUIRRELDEF( GetPrevSiblingElement );

	static _SQUIRRELDEF( GetElementAttribute );
	static _SQUIRRELDEF( SetElementAttribute );

	static _SQUIRRELDEF( GetElementName );

	static _SQUIRRELDEF( GetElementContent );
	static _SQUIRRELDEF( SetElementContent );
};


#endif