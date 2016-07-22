/*
 * SQPickupFuncs.h
 * Edited: 30/3/2009
 */


#ifndef _SQPICKUPFUNCS_H
#define _SQPICKUPFUNCS_H

#include "SquirrelMain.h"


class CSQPickupFuncs
{
public:
	static _SQUIRRELDEF( Create );
	//static _SQUIRRELDEF( Remove );

	static _SQUIRRELDEF( FindPickup );

	static _SQUIRRELDEF( GetCount );

	/*static _SQUIRRELDEF( GetID );
	static _SQUIRRELDEF( GetModel );
	static _SQUIRRELDEF( SetModel );
	static _SQUIRRELDEF( IsSpawned );

	static _SQUIRRELDEF( SetOnceOnly );
	static _SQUIRRELDEF( SetRespawnTime );*/
	static _SQUIRRELDEF( GetGlobalRespawnTime );
	static _SQUIRRELDEF( SetGlobalRespawnTime );

	/*static _SQUIRRELDEF( GetPos );
	static _SQUIRRELDEF( SetPos );
	static _SQUIRRELDEF( Respawn );
	static _SQUIRRELDEF( SpawnForPlayer );
	static _SQUIRRELDEF( HideForPlayer );*/
};

#endif