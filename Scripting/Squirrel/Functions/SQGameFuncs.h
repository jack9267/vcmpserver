/*
 * SQGameFuncs.h
 * Edited: 11/3/2008
 */


#ifndef _SQGAMEFUNCS_H
#define _SQGAMEFUNCS_H

#include "SquirrelMain.h"

class CSQGameFuncs
{
public:
	static _SQUIRRELDEF( GetHour );
	static _SQUIRRELDEF( SetHour );
	static _SQUIRRELDEF( GetMinute );
	static _SQUIRRELDEF( SetMinute );
	static _SQUIRRELDEF( SetTime );
	static _SQUIRRELDEF( GetTimeRate );
	static _SQUIRRELDEF( SetTimeRate );
	
	static _SQUIRRELDEF( GetWeather );
	static _SQUIRRELDEF( SetWeather );
	static _SQUIRRELDEF( GetWeatherRate );
	static _SQUIRRELDEF( SetWeatherRate );
	static _SQUIRRELDEF( GetWeatherLock );
	static _SQUIRRELDEF( SetWeatherLock );

	static _SQUIRRELDEF( SetAmmuWeapon );
	static _SQUIRRELDEF( IsAmmuWeaponEnabled );
	static _SQUIRRELDEF( SetAmmuWeaponEnabled );

	static _SQUIRRELDEF( CreateMarker );
	static _SQUIRRELDEF( CreateMarkerForPlayer );

};

#endif