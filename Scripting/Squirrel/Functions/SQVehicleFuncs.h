/*
 * SQVehicleFuncs.h
 * Edited: 26/3/2009
 */


#ifndef _SQVEHICLEFUNCS_H
#define _SQVEHICLEFUNCS_H

#include "SquirrelMain.h"


class CSQVehicleFuncs
{
public:
	static _SQUIRRELDEF( CreateStaticVehicle );
	static _SQUIRRELDEF( FindVehicle );

	static _SQUIRRELDEF( GetCount );

	/*static _SQUIRRELDEF( GetID );
	static _SQUIRRELDEF( GetModel );

	static _SQUIRRELDEF( GetHealth );
	//static _SQUIRRELDEF( SetHealth );
	static _SQUIRRELDEF( Blow );
	static _SQUIRRELDEF( SetIndestructible );
	static _SQUIRRELDEF( GetColour1 );
	static _SQUIRRELDEF( GetColour2 );
	static _SQUIRRELDEF( GetDriver );

	static _SQUIRRELDEF( IsLocked );
	static _SQUIRRELDEF( SetLocked );
	static _SQUIRRELDEF( KillEngine );

	static _SQUIRRELDEF( GetPos );
	static _SQUIRRELDEF( SetPos );

	static _SQUIRRELDEF( Respawn );
	static _SQUIRRELDEF( SetSpawnInfo );
	static _SQUIRRELDEF( MoveTo );*/

	static _SQUIRRELDEF( GetNameFromModel );
	static _SQUIRRELDEF( GetModelFromName );
};

#endif