/*
	SQPlayerFuncs.cpp
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

	Edited: 16/4/2009

	--------------------------------------------------------------------
*/


#include "SQPlayerFuncs.h"
#include "Pools.h" // We need to include pools to get access to players, vehicles etc.

/*#ifndef PI
#define PI (3.14159265358979323846)
#endif*/

const float PI = 3.14159265358979323846f;

/*
_SQUIRRELDEF( CSQPlayerFuncs::GetID )
{
	SQInteger iArgs = sq_gettop( v ); // Get how many arguments have been passed

	if ( iArgs == 2 ) // Arguments + root table
	{
		SQUserPointer pPointer = NULL; // We pass players as a pointer for easier operations

		sq_getuserpointer( v, 2, &pPointer ); // This grabs the user pointer. We use 2 because the root table is at 1

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer; // He recast the pointer we grabbed from squirrel to a CPlayerPool type
		if ( pPlayer ) // Is the pointer valid?
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetID() ); // Push the player id back to squirrel
			return 1; // Return here to stop it going further
		}
	}

	// If the args were invalid or something failed, this will execute and return an INVALID_PLAYER_ID (255)
	sq_pushinteger( v, (SQInteger)INVALID_PLAYER_ID );
	return 1;
}*/

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::FindPlayer )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		if ( sq_gettype( v, 2 ) == OT_INTEGER )
		{
			SQInteger iPlayerID = NULL;
			sq_getinteger( v, 2, &iPlayerID );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( (unsigned int)iPlayerID );
			if ( pPlayer )
			{
				CSquirrel* pScript = CSquirrelManager::Find( v );
				if ( pScript )
				{
					push_Player( pPlayer );
					return 1;
				}
			}
		}
		else if ( sq_gettype( v, 2 ) == OT_STRING )
		{
			const SQChar* szPlayer = 0;

			sq_getstring( v, 2, &szPlayer );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( (char*)szPlayer );
			if ( pPlayer )
			{
				CSquirrel* pScript = CSquirrelManager::Find( v );
				if ( pScript )
				{
					push_Player( pPlayer );
					return 1;
				}
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------
/*
_SQUIRRELDEF( CSQPlayerFuncs::GetNick )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			char szNick[ MAX_NAME_LEN ] = { NULL };
			strncpy( szNick, pPlayer->GetNick(), MAX_NAME_LEN );
			sq_pushstring( v, (SQChar*)szNick, -1 );
			return 1;
		}
	}

	sq_pushstring( v, (SQChar*)"Unknown", 7 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetIP )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );
		
		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			char szIP[ MAX_IP_LEN ] = { NULL };
			strncpy( szIP, pPlayer->GetIP(), MAX_IP_LEN );
			sq_pushstring( v, (SQChar*)szIP, -1 );
			return 1;
		}
	}
	sq_pushstring( v, (SQChar*)"0.0.0.0", 7 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetPing )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetPing() );	
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)0 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetHealth )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetHealth() );
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)0 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetHealth )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iHealth = NULL;
		sq_getinteger( v, 3, &iHealth );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{	
			pPlayer->SetHealth( (unsigned char)iHealth );
			CNetSends::SetPlayerHealth( (unsigned char)pPlayer->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetArmour )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetArmour() );
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)0 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetArmour )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iArmour = NULL;
		sq_getinteger( v, 3, &iArmour );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetArmour( (unsigned char)iArmour );
			CNetSends::SetPlayerArmour( (unsigned char)pPlayer->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetScore )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetScore() );
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)0 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetScore )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );
		SQInteger iScore = NULL;
		sq_getinteger( v, 3, &iScore );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetScore( (int)iScore );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IncScore )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->IncScore();
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::DecScore )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->DecScore();
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetClass )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetClassID() );
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)-1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetTeam )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetTeam() );
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)-1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetSkin )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CSpawnPool* pSpawn = CSpawnPoolManager::Find( pPlayer->GetClassID() );
			if ( pSpawn )
			{
				sq_pushinteger( v, (SQInteger)pSpawn->GetSkin() );
				return 1;
			}
		}
	}
	sq_pushinteger( v, (SQInteger)-1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetAnim )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iAnim = NULL;
		sq_getinteger( v, 3, &iAnim );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CNetSends::DoAnim( (unsigned char)pPlayer->GetID(), (unsigned char)iAnim );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetWeapon )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetWeapon() );
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)0 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetWeapon )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 4 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iWeapon = NULL;
		sq_getinteger( v, 3, &iWeapon );

		SQInteger iAmmo = NULL;
		sq_getinteger( v, 4, &iAmmo );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CNetSends::SetPlayerWeapon( (unsigned char)pPlayer->GetID(), (unsigned char)iWeapon, (unsigned int)iAmmo );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetControls )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetKeys() );
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetCash )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushinteger( v, (SQInteger)pPlayer->GetCash() );
			return 1;
		}
	}
	sq_pushinteger( v, (SQInteger)0 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetCash )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iCash = NULL;
		sq_getinteger( v, 3, &iCash );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetCash( (int)iCash );
			CNetSends::SetPlayerCash( (unsigned char)pPlayer->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GiveCash )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iCash = NULL;
		sq_getinteger( v, 3, &iCash );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetCash( pPlayer->GetCash() + (int)iCash );
			CNetSends::SetPlayerCash( (unsigned char)pPlayer->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::TakeCash )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iCash = NULL;
		sq_getinteger( v, 3, &iCash );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetCash( pPlayer->GetCash() - (int)iCash );
			CNetSends::SetPlayerCash( (unsigned char)pPlayer->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetVehicle )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushuserpointer( v, (SQUserPointer)CVehiclePoolManager::Find( pPlayer->GetVehicle() ) );
			return 1;
		}
	}
	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::PutInVehicle )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPlayerPointer = NULL;
		sq_getuserpointer( v, 2, &pPlayerPointer );

		SQUserPointer pVehiclePointer = NULL;
		sq_getuserpointer( v, 3, &pVehiclePointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPlayerPointer;
		CVehiclePool* pVehicle = (CVehiclePool*)pVehiclePointer;
		if ( ( pPlayer ) && ( pVehicle ) )
		{
			CNetSends::PutPlayerInVehicle( (unsigned char)pPlayer->GetID(), (unsigned char)pVehicle->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IsPassenger )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushbool( v, (SQBool)( pPlayer->GetVehiclePassenger()!=18 ? true : false ) );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IsAdmin )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushbool( v, (SQBool)pPlayer->IsAdmin() );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IsSpawned )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushbool( v, (SQBool)pPlayer->GetSpawned() );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetSpectating )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			if ( pPlayer->GetSpectating() != INVALID_PLAYER_ID )
			{
				sq_pushuserpointer( v, CPlayerPoolManager::Find( pPlayer->GetSpectating() ) );
				return 1;
			}
		}
	}
	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IsFrozen )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushbool( v, (SQBool)pPlayer->GetFrozen() );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::ToggleControls )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );
		SQBool bEnabled = NULL;
		sq_getbool( v, 3, &bEnabled );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetFrozen( (bool)bEnabled );
			CNetSends::SetPlayerFrozen( pPlayer->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IsMuted )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushbool( v, (SQBool)pPlayer->GetMuted() );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::Mute )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQBool bMuted = FALSE;
		sq_getbool( v, 3, &bMuted );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetMuted( (bool)bMuted );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IsIgnoredBy )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL, pOtherPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );
		sq_getuserpointer( v, 3, &pOtherPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CPlayerPool* pOtherPlayer = (CPlayerPool*)pOtherPointer;
			if ( pOtherPlayer )
			{
				sq_pushbool( v, (SQBool)pPlayer->GetIgnored( pOtherPlayer->GetID() ) );
				return 1;
			}
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::Ignore )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		if ( sq_gettype( v, 3 ) == OT_USERPOINTER )
		{
			SQUserPointer pPointer = NULL, pIgnoree = NULL;
			sq_getuserpointer( v, 2, &pPointer );
			sq_getuserpointer( v, 3, &pIgnoree );

			CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
			if ( pPlayer )
			{
				CPlayerPool* pIgnore = (CPlayerPool*)pIgnoree;
				if ( pIgnore )
				{
					pPlayer->SetIgnored( pIgnore->GetID(), true );

					sq_pushbool( v, (SQBool)true );
					return 1;
				}
			}
		}
		else if ( sq_gettype( v, 3 ) == OT_INTEGER )
		{
			SQUserPointer pPointer = NULL;
			SQInteger iIgnoree = 0;
			sq_getuserpointer( v, 2, &pPointer );
			sq_getinteger( v, 3, &iIgnoree );

			CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
			if ( pPlayer )
			{
				CPlayerPool* pIgnore = CPlayerPoolManager::Find( (unsigned char)iIgnoree );
				if ( pIgnore )
				{
					pPlayer->SetIgnored( pIgnore->GetID(), true );

					sq_pushbool( v, (SQBool)true );
					return 1;
				}
			}
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::Unignore )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		if ( sq_gettype( v, 3 ) == OT_USERPOINTER )
		{
			SQUserPointer pPointer = NULL, pIgnoree = NULL;
			sq_getuserpointer( v, 2, &pPointer );
			sq_getuserpointer( v, 3, &pIgnoree );

			CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
			if ( pPlayer )
			{
				CPlayerPool* pIgnore = (CPlayerPool*)pIgnoree;
				if ( pIgnore )
				{
					pPlayer->SetIgnored( pIgnore->GetID(), false );

					sq_pushbool( v, (SQBool)true );
					return 1;
				}
			}
		}
		else if ( sq_gettype( v, 3 ) == OT_INTEGER )
		{
			SQUserPointer pPointer = NULL;
			SQInteger iIgnoree = 0;
			sq_getuserpointer( v, 2, &pPointer );
			sq_getinteger( v, 3, &iIgnoree );

			CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
			if ( pPlayer )
			{
				CPlayerPool* pIgnore = CPlayerPoolManager::Find( (unsigned char)iIgnoree );
				if ( pIgnore )
				{
					pPlayer->SetIgnored( pIgnore->GetID(), false );

					sq_pushbool( v, (SQBool)true );
					return 1;
				}
			}
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IsSyncBlocked )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			sq_pushbool( v, (SQBool)pPlayer->GetSyncBlocked() );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetSyncBlocked )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQBool bSyncBlocked = FALSE;
		sq_getbool( v, 3, &bSyncBlocked );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetSyncBlocked( (bool)bSyncBlocked );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::IsSyncBlockedTo )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL, pOtherPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );
		sq_getuserpointer( v, 3, &pOtherPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CPlayerPool* pOtherPlayer = (CPlayerPool*)pOtherPointer;
			if ( pOtherPlayer )
			{
				sq_pushbool( v, (SQBool)pPlayer->GetSyncBlockedTo( pOtherPlayer->GetID() ) );
				return 1;
			}
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetSyncBlockedTo )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL, pPointerTo = NULL;
		sq_getuserpointer( v, 2, &pPointer );
		sq_getuserpointer( v, 3, &pPointerTo );

		SQBool bBlocked = false;
		sq_getbool( v, 4, &bBlocked );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CPlayerPool* pPlayerTo = (CPlayerPool*)pPointerTo;
			if ( pPlayerTo )
			{
				pPlayer->SetSyncBlockedTo( pPlayerTo->GetID(), bBlocked );
				sq_pushbool( v, (SQBool)true );
				return 1;
			}
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::GetPos )
{
	SQInteger iArgs = sq_gettop( v ); // Get how many arguments have been passed
	Vector *vPos = new Vector(); // Create a new Vector class

	if ( iArgs == 2 ) // Arguments + root table
	{
		SQUserPointer pPointer = NULL; // We pass players as a pointer for easier operations

		sq_getuserpointer( v, 2, &pPointer ); // This grabs the user pointer. We use 2 because the root table is at 1

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer; // He recast the pointer we grabbed from squirrel to a CPlayerPool type
		if ( pPlayer ) // Is the pointer valid?
		{
			// Load the position into our vector
			vPos->x = pPlayer->GetX(); 
			vPos->y = pPlayer->GetY();
			vPos->z = pPlayer->GetZ();
		}
	}

	// Push the vector to squirrel. It will either be 0.0 0.0 0.0 or the players position
	Push( v, vPos );

	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetPos )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		Vector* vPos = Get( TypeWrapper<Vector*>(), v, 3 );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetX( vPos->x );
			pPlayer->SetY( vPos->y );
			pPlayer->SetZ( vPos->z );
			CNetSends::SetPlayerPos( pPlayer->GetID(), 0 );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	if ( iArgs == 5 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQFloat fX = NULL;
		sq_getfloat( v, 3, &fX );

		SQFloat fY = NULL;
		sq_getfloat( v, 4, &fY );

		SQFloat fZ = NULL;
		sq_getfloat( v, 5, &fZ );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			pPlayer->SetX( (float)fX ); 
			pPlayer->SetY( (float)fY ); 
			pPlayer->SetZ( (float)fZ );
			CNetSends::SetPlayerPos( pPlayer->GetID(), 0 );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetInterior )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iInterior = NULL;
		sq_getinteger( v, 3, &iInterior );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CNetSends::SetPlayerPos( pPlayer->GetID(), (unsigned char)iInterior );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

#ifndef PI
#define PI (3.14159265358979323846)
#endif

_SQUIRRELDEF( CSQPlayerFuncs::GetAngle )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			float fAngle = pPlayer->GetRotation();
			sq_pushfloat( v, (SQFloat)(fAngle>0 ? fAngle*180/PI : (2*PI+fAngle)*180/PI) );
			return 1;
		}
	}
	sq_pushfloat( v, (SQFloat)0.0f );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetMarker )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iColour = NULL;
		sq_getinteger( v, 3, &iColour );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CNetSends::SetPlayerMarker( pPlayer->GetID(), iColour );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::RemoveMarker )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		if ( pPlayer )
		{
			CNetSends::RemovePlayerMarker( pPlayer->GetID() );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::SetLocalMarker )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 4 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iColour = NULL;
		sq_getinteger( v, 3, &iColour );

		SQUserPointer pPointerTo = NULL;
		sq_getuserpointer( v, 4, &pPointerTo );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		CPlayerPool* pPlayerTo = (CPlayerPool*)pPointerTo;

		if ( ( pPlayer ) && ( pPlayerTo ) )
		{
			CNetSends::SetPlayerLocalMarker( pPlayer->GetID(), iColour, pPlayerTo->GetID() );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPlayerFuncs::RemoveLocalMarker )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQUserPointer pPointerTo = NULL;
		sq_getuserpointer( v, 3, &pPointerTo );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;
		CPlayerPool* pPlayerTo = (CPlayerPool*)pPointerTo;

		if ( ( pPlayer ) && ( pPlayerTo ) )
		{
			CNetSends::RemovePlayerLocalMarker( pPlayer->GetID(), pPlayerTo->GetID() );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}
*/

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, _get )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	const SQChar *s = sa.GetString( 2 );

	if ( self )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

		if ( !strcmp( (char*)s, "ID" ) )
		{
			sa.Return( pPlayer->GetID() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Name" ) )
		{
			sa.Return( pPlayer->GetNick() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "IP" ) )
		{
			sa.Return( pPlayer->GetIP() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Ping" ) )
		{
			sa.Return( pPlayer->GetPing() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Health" ) )
		{
			sa.Return( pPlayer->GetHealth() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Armour" ) )
		{
			sa.Return( pPlayer->GetArmour() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Score" ) )
		{
			sa.Return( pPlayer->GetScore() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Class" ) )
		{
			sa.Return( pPlayer->GetClassID() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Team" ) )
		{
			sa.Return( pPlayer->GetTeam() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Skin" ) )
		{
			CSpawnPool* pSpawn = CSpawnPoolManager::Find( pPlayer->GetClassID() );
			if ( pSpawn )
			{
				sa.Return( (int)pSpawn->GetSkin() );
				return 1;
			}
		}

		else if ( !strcmp( (char*)s, "Cash" ) )
		{
			sa.Return( pPlayer->GetCash() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Weapon" ) )
		{
			sa.Return( pPlayer->GetWeapon() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Keys" ) )
		{
			sa.Return( pPlayer->GetKeys() );
			return 1;
		}
		
		else if ( !strcmp( (char*)s, "Vehicle" ) )
		{
			CVehiclePool* pVehicle = CVehiclePoolManager::Find( pPlayer->GetVehicle() );
			if ( pVehicle )
			{
				CSquirrel* pScript = CSquirrelManager::Find( v );
				if ( pScript )
				{
					push_Vehicle( pVehicle );
					return 1;
				}
			}

			sq_pushnull( v );
			return 1;
		}
		
		else if ( !strcmp( (char*)s, "IsPassenger" ) )
		{
			if ( ( pPlayer->GetVehicle() ) && ( pPlayer->GetVehiclePassenger() != 18 ) )
				sa.Return( true );
			else
				sa.Return( false );
		}

		else if ( !strcmp( (char*)s, "IsAdmin" ) )
		{
			sa.Return( pPlayer->IsAdmin() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "IsSpawned" ) )
		{
			sa.Return( pPlayer->GetSpawned() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "IsFrozen" ) )
		{
			sa.Return( !pPlayer->GetFrozen() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "IsMuted" ) )
		{
			sa.Return( pPlayer->GetMuted() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "IsSyncBlocked" ) )
		{
			sa.Return( pPlayer->GetSyncBlocked() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Pos" ) )
		{
			Vector* vPos = new Vector();
			vPos->x = pPlayer->GetX();
			vPos->y = pPlayer->GetY();
			vPos->z = pPlayer->GetZ();

			push_Vector( vPos );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Angle" ) )
		{
			float fAngle = pPlayer->GetRotation();

			sa.Return( (FLOAT)(fAngle>0 ? fAngle*180/PI : (2*PI+fAngle)*180/PI) );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Seat" ) )
		{
			sa.Return( !pPlayer->GetVehicleSeat() );
			return 1;
		}
	}

	sa.Return();
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, _set )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	const SQChar *s = sa.GetString( 2 );

	// We do the find stuff below because it seems the entire class gets loaded into a virtual class
	// So no changes get applied directly
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	if ( !strcmp( (char*)s, "Pos" ) )
	{
		_CHECK_INST_PARAM( vTmp, 3, Vector, Vector );

		if ( pPlayer )
		{
			pPlayer->SetX( vTmp->x );
			pPlayer->SetY( vTmp->y );
			pPlayer->SetZ( vTmp->z );

			CNetSends::SetPlayerPos( pPlayer->GetID(), 0 );

			sq_pushbool( v, true );

			return 1;
		}
	}
	else if ( !strcmp( (char*)s, "Score" ) )
	{
		int iScore = sa.GetInt( 3 );

		CScripting::PushFunction( "onPlayerScoreChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( pPlayer->GetScore() );
		CScripting::PushInteger( (int)iScore );
		CScripting::CallFunction();

		pPlayer->SetScore( (int)iScore );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Cash" ) )
	{
		int iCash = sa.GetInt( 3 );

		CScripting::PushFunction( "onPlayerCashChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( pPlayer->GetCash() );
		CScripting::PushInteger( (int)iCash );
		CScripting::CallFunction();

		pPlayer->SetCash( (int)iCash );

		CNetSends::SetPlayerCash( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Health" ) )
	{
		int iHealth = sa.GetInt( 3 );

		CScripting::PushFunction( "onPlayerHealthChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( pPlayer->GetHealth() );
		CScripting::PushInteger( (int)iHealth );
		CScripting::CallFunction();

		pPlayer->SetHealth( (int)iHealth );

		CNetSends::SetPlayerHealth( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Armour" ) )
	{
		int iArmour = sa.GetInt( 3 );

		CScripting::PushFunction( "onPlayerArmourChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( pPlayer->GetArmour() );
		CScripting::PushInteger( (int)iArmour );
		CScripting::CallFunction();

		pPlayer->SetArmour( (int)iArmour );

		CNetSends::SetPlayerArmour( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "IsFrozen" ) )
	{
		bool bFrozen = sa.GetBool( 3 );

		pPlayer->SetFrozen( !(bool)bFrozen );
		CNetSends::SetPlayerFrozen( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "IsMuted" ) )
	{
		bool bMuted = sa.GetBool( 3 );

		pPlayer->SetMuted( (bool)bMuted );

		sa.Return( true );
		return 1;
	}
	else if ( !strcmp( (char*)s, "IsSyncBlocked" ) )
	{
		bool bBlocked = sa.GetBool( 3 );

		pPlayer->SetSyncBlocked( (bool)bBlocked );

		sa.Return( true );
		return 1;
	}
	else if ( !strcmp( (char*)s, "Vehicle" ) )
	{
		_CHECK_INST_PARAM( pVehicle, 3, CVehiclePool, Vehicle );
		if ( pVehicle )
		{
			pPlayer->SetVehicle( pVehicle->GetID() );
			CNetSends::PutPlayerInVehicle( (unsigned char)pPlayer->GetID(), (unsigned char)pVehicle->GetID() );

			sa.Return( true );
			return 1;
		}
	}

	sa.Return( false );
	return 1;
}
/*
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, _add )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	const SQChar *s = sa.GetString( 2 );

	// We do the find stuff below because it seems the entire class gets loaded into a virtual class
	// So no changes get applied directly
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	if ( !strcmp( (char*)s, "Score" ) )
	{
		int i = sa.GetInt( 3 );
		int iScore = pPlayer->GetScore();

		CScripting::PushFunction( "onPlayerScoreChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iScore );
		CScripting::PushInteger( (int)( iScore + i ) );
		CScripting::CallFunction();

		pPlayer->SetScore( iScore + i );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Cash" ) )
	{
		int i = sa.GetInt( 3 );
		int iCash = pPlayer->GetCash();

		CScripting::PushFunction( "onPlayerCashChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iCash );
		CScripting::PushInteger( (int)( iCash + i ) );
		CScripting::CallFunction();

		pPlayer->SetCash( iCash + i );

		CNetSends::SetPlayerCash( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Health" ) )
	{
		int i = sa.GetInt( 3 );
		int iHealth = pPlayer->GetHealth();

		CScripting::PushFunction( "onPlayerHealthChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iHealth );
		CScripting::PushInteger( (int)( iHealth + i ) );
		CScripting::CallFunction();

		pPlayer->SetHealth( iHealth + i );

		CNetSends::SetPlayerHealth( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Armour" ) )
	{
		int i = sa.GetInt( 3 );
		int iArmour = pPlayer->GetArmour();

		CScripting::PushFunction( "onPlayerArmourChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iArmour );
		CScripting::PushInteger( (int)( iArmour + i ) );
		CScripting::CallFunction();

		pPlayer->SetArmour( iArmour + i );

		CNetSends::SetPlayerArmour( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}

	sa.Return( false );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, _sub )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	const SQChar *s = sa.GetString( 2 );

	// We do the find stuff below because it seems the entire class gets loaded into a virtual class
	// So no changes get applied directly
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	if ( !strcmp( (char*)s, "Score" ) )
	{
		int i = sa.GetInt( 3 );
		int iScore = pPlayer->GetScore();

		CScripting::PushFunction( "onPlayerScoreChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iScore );
		CScripting::PushInteger( (int)( iScore - i ) );
		CScripting::CallFunction();

		pPlayer->SetScore( iScore - i );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Cash" ) )
	{
		int i = sa.GetInt( 3 );
		int iCash = pPlayer->GetCash();

		CScripting::PushFunction( "onPlayerCashChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iCash );
		CScripting::PushInteger( (int)( iCash - i ) );
		CScripting::CallFunction();

		pPlayer->SetCash( iCash - i );

		CNetSends::SetPlayerCash( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Health" ) )
	{
		int i = sa.GetInt( 3 );
		int iHealth = pPlayer->GetHealth();

		CScripting::PushFunction( "onPlayerHealthChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iHealth );
		CScripting::PushInteger( (int)( iHealth - i ) );
		CScripting::CallFunction();

		pPlayer->SetHealth( iHealth - i );

		CNetSends::SetPlayerHealth( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Armour" ) )
	{
		int i = sa.GetInt( 3 );
		int iArmour = pPlayer->GetArmour();

		CScripting::PushFunction( "onPlayerArmourChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iArmour );
		CScripting::PushInteger( (int)( iArmour - i ) );
		CScripting::CallFunction();

		pPlayer->SetArmour( iArmour - i );

		CNetSends::SetPlayerArmour( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}

	sa.Return( false );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, _mul )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	const SQChar *s = sa.GetString( 2 );

	// We do the find stuff below because it seems the entire class gets loaded into a virtual class
	// So no changes get applied directly
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	if ( !strcmp( (char*)s, "Score" ) )
	{
		int i = sa.GetInt( 3 );
		int iScore = pPlayer->GetScore();

		CScripting::PushFunction( "onPlayerScoreChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iScore );
		CScripting::PushInteger( (int)( iScore * i ) );
		CScripting::CallFunction();

		pPlayer->SetScore( iScore * i );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Cash" ) )
	{
		int i = sa.GetInt( 3 );
		int iCash = pPlayer->GetCash();

		CScripting::PushFunction( "onPlayerCashChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iCash );
		CScripting::PushInteger( (int)( iCash * i ) );
		CScripting::CallFunction();

		pPlayer->SetCash( iCash * i );

		CNetSends::SetPlayerCash( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Health" ) )
	{
		int i = sa.GetInt( 3 );
		int iHealth = pPlayer->GetHealth();

		CScripting::PushFunction( "onPlayerHealthChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iHealth );
		CScripting::PushInteger( (int)( iHealth * i ) );
		CScripting::CallFunction();

		pPlayer->SetHealth( iHealth * i );

		CNetSends::SetPlayerHealth( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Armour" ) )
	{
		int i = sa.GetInt( 3 );
		int iArmour = pPlayer->GetArmour();

		CScripting::PushFunction( "onPlayerArmourChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iArmour );
		CScripting::PushInteger( (int)( iArmour * i ) );
		CScripting::CallFunction();

		pPlayer->SetArmour( iArmour * i );

		CNetSends::SetPlayerArmour( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}

	sa.Return( false );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, _div )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	const SQChar *s = sa.GetString( 2 );

	// We do the find stuff below because it seems the entire class gets loaded into a virtual class
	// So no changes get applied directly
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	if ( !strcmp( (char*)s, "Score" ) )
	{
		int i = sa.GetInt( 3 );
		int iScore = pPlayer->GetScore();

		CScripting::PushFunction( "onPlayerScoreChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iScore );
		CScripting::PushInteger( (int)( iScore / i ) );
		CScripting::CallFunction();

		pPlayer->SetScore( iScore / i );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Cash" ) )
	{
		int i = sa.GetInt( 3 );
		int iCash = pPlayer->GetCash();

		CScripting::PushFunction( "onPlayerCashChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iCash );
		CScripting::PushInteger( (int)( iCash / i ) );
		CScripting::CallFunction();

		pPlayer->SetCash( iCash / i );

		CNetSends::SetPlayerCash( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Health" ) )
	{
		int i = sa.GetInt( 3 );
		int iHealth = pPlayer->GetHealth();

		CScripting::PushFunction( "onPlayerHealthChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iHealth );
		CScripting::PushInteger( (int)( iHealth / i ) );
		CScripting::CallFunction();

		pPlayer->SetHealth( iHealth / i );

		CNetSends::SetPlayerHealth( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}
	else if ( !strcmp( (char*)s, "Armour" ) )
	{
		int i = sa.GetInt( 3 );
		int iArmour = pPlayer->GetArmour();

		CScripting::PushFunction( "onPlayerArmourChange" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( iArmour );
		CScripting::PushInteger( (int)( iArmour / i ) );
		CScripting::CallFunction();

		pPlayer->SetArmour( iArmour / i );

		CNetSends::SetPlayerArmour( (unsigned char)pPlayer->GetID() );

		sa.Return( true );

		return 1;
	}

	sa.Return( false );

	return 1;
}
*/
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, _tostring )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	sa.Return( self->GetNick() );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, _cmp )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	_CHECK_INST_PARAM( player, 2, CPlayerPool, Player );

	if ( self->GetID() == player->GetID() ) sa.Return( true );
	else sa.Return( false );

	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Player, Name )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetNick() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, ID )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	sa.Return( pPlayer->GetID() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, IP )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetIP() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Ping )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	sa.Return( pPlayer->GetPing() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Health )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetHealth() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Armour )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetArmour() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Score )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetScore() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Class )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetClassID() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Team )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetTeam() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Skin )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	CSpawnPool* pSpawn = CSpawnPoolManager::Find( pPlayer->GetClassID() );
	if ( pSpawn )
	{
		sa.Return( (int)pSpawn->GetSkin() );
		return 1;
	}

	sa.Return( -1 );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Cash )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetCash() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Weapon )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetWeapon() );
	return 1;
}
*/
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, SetWeapon )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	int iWeapon = sa.GetInt( 2 );
	int iAmmo = sa.GetInt( 3 );

	pPlayer->SetWeapon( (unsigned char)iWeapon );
	CNetSends::SetPlayerWeapon( (unsigned char)pPlayer->GetID(), (unsigned char)iWeapon, (unsigned int)iAmmo );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, SetAnim )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	int iAnim = sa.GetInt( 2 );

	pPlayer->SetAction( (unsigned char)iAnim );
	CNetSends::DoAnim( (unsigned char)pPlayer->GetID(), (unsigned char)iAnim );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Player, Keys )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );
	
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	sa.Return( pPlayer->GetKeys() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Vehicle )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( pPlayer->GetVehicle() );
	if ( pVehicle )
	{
		CSquirrel* pScript = CSquirrelManager::Find( v );
		if ( pScript )
		{
			push_Vehicle( pVehicle );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, IsPassenger )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	if ( ( pPlayer->GetVehicle() ) && ( pPlayer->GetVehiclePassenger() != 18 ) )
		sa.Return( true );
	else
		sa.Return( false );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, IsAdmin )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	sa.Return( pPlayer->IsAdmin() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, IsSpawned )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	sa.Return( pPlayer->GetSpawned() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, IsFrozen )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	sa.Return( !pPlayer->GetFrozen() );

	return 1;
}
*/
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, GetSpectating )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	CPlayerPool* pPlayerSpec = CPlayerPoolManager::Find( (unsigned char)pPlayer->GetSpectating() );
	if ( pPlayerSpec )
	{
		CSquirrel* pScript = CSquirrelManager::Find( v );
		if ( pScript )
		{
			push_Player( pPlayerSpec );
			return 1;
		}
	}
	
	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Player, IsMuted )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	sa.Return( pPlayer->GetMuted() );
	return 1;
}
*/
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, GetIgnoredBy )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	_CHECK_INST_PARAM( pOtherPlayer, 2, CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	if ( pOtherPlayer )
	{	
		sa.Return( pPlayer->GetIgnored( pOtherPlayer->GetID() ) );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, SetIgnoredBy )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	_CHECK_INST_PARAM( pOtherPlayer, 2, CPlayerPool, Player );

	bool bIgnored = sa.GetBool( 3 );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	if ( pOtherPlayer )
	{	
		pPlayer->SetIgnored( pOtherPlayer->GetID(), bIgnored );
		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, GetSyncBlockedTo )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	_CHECK_INST_PARAM( pOtherPlayer, 2, CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	if ( pOtherPlayer )
	{
		sa.Return( pPlayer->GetSyncBlockedTo( pOtherPlayer->GetID() ) );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, SetSyncBlockedTo )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	_CHECK_INST_PARAM( pOtherPlayer, 2, CPlayerPool, Player );

	bool bBlocked = sa.GetBool( 3 );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	if ( pOtherPlayer )
	{
		pPlayer->SetSyncBlockedTo( pOtherPlayer->GetID(), bBlocked );
		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}
/*
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, Pos )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	Vector* vPos = new Vector();
	vPos->x = pPlayer->GetX();
	vPos->y = pPlayer->GetY();
	vPos->z = pPlayer->GetZ();

	Push( v, vPos );
	return 1;
}

//--------------------------------------------------

#ifndef PI
#define PI (3.14159265358979323846)
#endif

_MEMBER_FUNCTION_IMPL( Player, Angle )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	float fAngle = pPlayer->GetRotation();
	sa.Return( (FLOAT)(fAngle>0 ? fAngle*180/PI : (2*PI+fAngle)*180/PI) );
	return 1;
}
*/
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, SetInterior )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	int iInterior = sa.GetInt( 2 );

	CNetSends::SetPlayerPos( pPlayer->GetID(), (unsigned char)iInterior );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, SetMarker )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	int iMarker = sa.GetInt( 2 );

	CNetSends::SetPlayerMarker( pPlayer->GetID(), (unsigned char)iMarker );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, RemoveMarker )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	CNetSends::RemovePlayerMarker( pPlayer->GetID() );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, SetLocalMarker )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	_CHECK_INST_PARAM( pPlayerTo, 2, CPlayerPool, Player );
	int iMarker = sa.GetInt( 3 );

	if ( pPlayerTo )
	{
		CNetSends::SetPlayerLocalMarker( pPlayer->GetID(), (unsigned char)iMarker, pPlayerTo->GetID() );
	
		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Player, RemoveLocalMarker )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );

	_CHECK_INST_PARAM( pPlayerTo, 2, CPlayerPool, Player );

	if ( pPlayerTo )
	{
		CNetSends::RemovePlayerLocalMarker( pPlayer->GetID(), pPlayerTo->GetID() );
	
		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Player, Seat )
{
	StackHandler sa(v);
	_CHECK_SELF( CPlayerPool, Player );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( self->GetID() );
	
	sa.Return( (unsigned char)pPlayer->GetVehicleSeat() );
	return 1;
}
*/
//--------------------------------------------------



_BEGIN_CLASS( Player )

_MEMBER_FUNCTION( Player, _get, 2, _T("xs") )
_MEMBER_FUNCTION( Player, _set, 3, _T("xs|n") )
//_MEMBER_FUNCTION( Player, _add, 3, _T("xs|n") )
//_MEMBER_FUNCTION( Player, _sub, 3, _T("xs|n") )
//_MEMBER_FUNCTION( Player, _mul, 3, _T("xs|n") )
//_MEMBER_FUNCTION( Player, _div, 3, _T("xs|n") )
_MEMBER_FUNCTION( Player, _tostring, 1, _T("x") )
_MEMBER_FUNCTION( Player, _cmp, 1, _T("x") )
_MEMBER_FUNCTION( Player, SetWeapon, 3, "xii" )
_MEMBER_FUNCTION( Player, SetAnim, 2, "x" )
_MEMBER_FUNCTION( Player, GetSpectating, 1, "x" )
_MEMBER_FUNCTION( Player, GetIgnoredBy, 2, "x." )
_MEMBER_FUNCTION( Player, SetIgnoredBy, 2, "x.b" )
_MEMBER_FUNCTION( Player, GetSyncBlockedTo, 2, "x." )
_MEMBER_FUNCTION( Player, SetSyncBlockedTo, 2, "x.b" )
_MEMBER_FUNCTION( Player, SetInterior, 2, "xi" )
_MEMBER_FUNCTION( Player, SetMarker, 2, "xi" )
_MEMBER_FUNCTION( Player, RemoveMarker, 1, "x" )
_MEMBER_FUNCTION( Player, SetLocalMarker, 3, "x.i" )
_MEMBER_FUNCTION( Player, RemoveLocalMarker, 2, "x." )

_END_CLASS( Player );