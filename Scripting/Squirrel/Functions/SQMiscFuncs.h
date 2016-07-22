/*
 * SQMiscFuncs.h
 * Edited: 5/5/2009
 */


#ifndef _SQMIRCFUNCS_H
#define _SQMIRCFUNCS_H

#include "SquirrelMain.h"

class CSQMiscFuncs
{
public:
	static _SQUIRRELDEF( Print );
	
	// Server commands
	static _SQUIRRELDEF( Message );
	static _SQUIRRELDEF( MessagePlayer );
	static _SQUIRRELDEF( MessageAllExcept );
	static _SQUIRRELDEF( PrivMessage );
	static _SQUIRRELDEF( Announce );
	static _SQUIRRELDEF( AnnounceAll );
	static _SQUIRRELDEF( SendPlayerMessage );

	static _SQUIRRELDEF( GetTickCount );
	static _SQUIRRELDEF( GetTime );
	static _SQUIRRELDEF( GetFullTime );
	static _SQUIRRELDEF( IsNum );

	static _SQUIRRELDEF( KickPlayer );
	static _SQUIRRELDEF( BanPlayer );
	static _SQUIRRELDEF( BanIP );
	static _SQUIRRELDEF( UnbanIP );

	static _SQUIRRELDEF( GetServerName );
	static _SQUIRRELDEF( SetServerName );
	static _SQUIRRELDEF( GetGamemodeName );
	static _SQUIRRELDEF( SetGamemodeName );
	static _SQUIRRELDEF( GetMapName );
	static _SQUIRRELDEF( SetMapName );
	static _SQUIRRELDEF( GetPassword );
	static _SQUIRRELDEF( SetPassword );
	static _SQUIRRELDEF( GetMaxPlayers );
	static _SQUIRRELDEF( SetMaxPlayers );
	static _SQUIRRELDEF( GetLocked );
	static _SQUIRRELDEF( SetLocked );

	static _SQUIRRELDEF( GetPlayers );
	static _SQUIRRELDEF( GetDeathmatchScoreboard );
	static _SQUIRRELDEF( SetDeathmatchScoreboard );
	static _SQUIRRELDEF( GetDrivebyEnabled );
	static _SQUIRRELDEF( SetDrivebyEnabled );

	static _SQUIRRELDEF( GetWeaponName );
	static _SQUIRRELDEF( GetWeaponID );
	static _SQUIRRELDEF( GetDistrictName );
	static _SQUIRRELDEF( GetSkinName );
	static _SQUIRRELDEF( InPoly );

	static _SQUIRRELDEF( CallFunc );

	static _SQUIRRELDEF( Halt );
};

#endif