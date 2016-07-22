/*
	SQGameFuncs.cpp
	This file is part of VCMP Server.

    VCMP Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VCMP Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VCMP Server.  If not, see <http://www.gnu.org/licenses/>.

	--------------------------------------------------------------------

    VC:MP Server 
		A server for VC:MP - A Multiplayer Modification For GTA:VC
    Copyright 2009 VRocker, Knight2k9 and Juppi

	File Author: Juppi

	--------------------------------------------------------------------
*/

/*
 * SQGameFuncs.cpp
 * Edited: 30/3/2009
 */


#include "SQGameFuncs.h"
#include "Pools.h"
#include "../../Main/ConfigLoader.h"



_SQUIRRELDEF( CSQGameFuncs::GetHour )
{
	sq_pushinteger( v, (SQInteger)CConfigLoader::GetHour() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetHour )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iHour = NULL;
		sq_getinteger( v, 2, &iHour );

		CScripting::PushFunction( "onTimeChange" );
		CScripting::PushInteger( (unsigned char)iHour );
		CScripting::PushInteger( CConfigLoader::GetMinute() );
		CScripting::CallFunction();

		CNetSends::SetHour( (unsigned char)iHour );
		CConfigLoader::SetHour( (unsigned char)iHour );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::GetMinute )
{
	sq_pushinteger( v, (SQInteger)CConfigLoader::GetMinute() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetMinute )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iMinute = NULL;
		sq_getinteger( v, 2, &iMinute );

		CScripting::PushFunction( "onTimeChange" );
		CScripting::PushInteger( CConfigLoader::GetHour() );
		CScripting::PushInteger( (unsigned char)iMinute );
		CScripting::CallFunction();

		CNetSends::SetMinute( (unsigned char)iMinute );
		CConfigLoader::SetMinute( (unsigned char)iMinute );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetTime )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQInteger iHour = NULL;
		sq_getinteger( v, 2, &iHour );

		SQInteger iMinute = NULL;
		sq_getinteger( v, 3, &iMinute );

		CScripting::PushFunction( "onTimeChange" );
		CScripting::PushInteger( (unsigned char)iHour );
		CScripting::PushInteger( (unsigned char)iMinute );
		CScripting::CallFunction();

		CNetSends::SetHour( (unsigned char)iHour );
		CConfigLoader::SetHour( (unsigned char)iHour );

		CNetSends::SetMinute( (unsigned char)iMinute );
		CConfigLoader::SetMinute( (unsigned char)iMinute );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::GetTimeRate )
{
	sq_pushinteger( v, (SQInteger)CConfigLoader::GetTimeRate() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetTimeRate )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iTimeRate = NULL;
		sq_getinteger( v, 2, &iTimeRate );

		CConfigLoader::SetTimeRate( (unsigned char)iTimeRate );
		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::GetWeather )
{
	sq_pushinteger( v, (SQInteger)CConfigLoader::GetWeather() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetWeather )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iWeather = NULL;
		sq_getinteger( v, 2, &iWeather );

		CScripting::PushFunction( "onWeatherChange" );
		CScripting::PushInteger( CConfigLoader::GetWeather() );
		CScripting::PushInteger( (unsigned char)iWeather );
		CScripting::CallFunction();

		CNetSends::SetWeather( (unsigned char)iWeather );
		CConfigLoader::SetWeather( (unsigned char)iWeather );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::GetWeatherRate )
{
	sq_pushinteger( v, (SQInteger)CConfigLoader::GetWeatherRate() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetWeatherRate )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iWeatherRate = NULL;
		sq_getinteger( v, 2, &iWeatherRate );

		CConfigLoader::SetWeatherRate( (unsigned int)(iWeatherRate * 1000) );
		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::GetWeatherLock )
{
	sq_pushbool( v, (SQBool)( CConfigLoader::GetWeatherRate() ? false : true ) );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetWeatherLock )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQBool bLocked = NULL;
		sq_getbool( v, 2, &bLocked );

		CConfigLoader::SetWeatherRate( bLocked ? NULL : 600000 );
		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetAmmuWeapon )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 5 )
	{
		SQInteger iAmmuID = NULL;
		sq_getinteger( v, 2, &iAmmuID );

		SQInteger iWeaponSlot = NULL;
		sq_getinteger( v, 3, &iWeaponSlot );

		SQInteger iWeaponID = NULL;
		sq_getinteger( v, 4, &iWeaponID );

		SQInteger iWeaponCost = NULL;
		sq_getinteger( v, 5, &iWeaponCost );

		CAmmuPool* pAmmunation = CAmmuPoolManager::Find( (unsigned char)iAmmuID );
		if ( pAmmunation )
		{
			pAmmunation->SetWeapon( (unsigned char)iWeaponSlot, (unsigned char)iWeaponID );
			pAmmunation->SetWeaponCost( (unsigned char)iWeaponSlot, (unsigned int)iWeaponCost );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::IsAmmuWeaponEnabled )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQInteger iAmmuID = NULL;
		sq_getinteger( v, 2, &iAmmuID );

		SQInteger iWeaponSlot = NULL;
		sq_getinteger( v, 3, &iWeaponSlot );

		CAmmuPool* pAmmunation = CAmmuPoolManager::Find( (unsigned char)iAmmuID );
		if ( pAmmunation )
		{
			sq_pushbool( v, (SQBool)pAmmunation->GetWeaponEnabled( (BYTE)iWeaponSlot ) );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::SetAmmuWeaponEnabled )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		SQInteger iAmmuID = NULL;
		sq_getinteger( v, 2, &iAmmuID );

		SQInteger iWeaponSlot = NULL;
		sq_getinteger( v, 3, &iWeaponSlot );

		SQBool bEnabled = NULL;
		sq_getbool( v, 4, &bEnabled );

		CAmmuPool* pAmmunation = CAmmuPoolManager::Find( (unsigned char)iAmmuID );
		if ( pAmmunation )
		{
			pAmmunation->SetWeaponEnabled( (unsigned char)iWeaponSlot, (bool)bEnabled );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::CreateMarker )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQInteger iIconID = NULL;
		sq_getinteger( v, 2, &iIconID );

		_CHECK_INST_PARAM( vPos, 3, Vector, Vector );

		CNetSends::SetMarker( iIconID, vPos->x, vPos->y, vPos->z );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQGameFuncs::CreateMarkerForPlayer )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		_CHECK_INST_PARAM( pPlayer, 2, CPlayerPool, Player );

		SQInteger iIconID = NULL;
		sq_getinteger( v, 3, &iIconID );

		_CHECK_INST_PARAM( vPos, 4, Vector, Vector );

		if ( pPlayer )
		{
			CNetSends::SetMarkerForPlayer( pPlayer->GetID(), iIconID, vPos->x, vPos->y, vPos->z );
			
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}