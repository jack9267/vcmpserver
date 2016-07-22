/*
	SQVehicleFuncs.cpp
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


#include "SQVehicleFuncs.h"
#include "Pools.h"
#include "Utils.h"
#include "CarCols.h"

/*#ifndef PI
#define PI (3.14159265358979323846)
#endif*/

const float PI = 3.14159265358979323846f;

_SQUIRRELDEF( CSQVehicleFuncs::CreateStaticVehicle )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 6 )
	{
		SQInteger iModel = NULL;
		sq_getinteger( v, 2, &iModel );

		_CHECK_INST_PARAM( vPos, 3, Vector, Vector );

		SQFloat fAngle = NULL;
		sq_getfloat( v, 4, &fAngle );

		SQInteger iColour1 = NULL;
		sq_getinteger( v, 5, &iColour1 );

		SQInteger iColour2 = NULL;
		sq_getinteger( v, 6, &iColour2 );

		if ( CVehiclePoolManager::Count() < MAX_VEHICLES - 1 )
		{
			unsigned char ucID = CVehiclePoolManager::Count() + 1;
			CVehiclePool* pVehicle = CVehiclePoolManager::New( ucID );

			pVehicle->SetModel( (BYTE)iModel );
			pVehicle->SetX( vPos->x );
			pVehicle->SetY( vPos->y );
			pVehicle->SetZ( vPos->z );
			
			CCarCols* pCarCols = CCarColManager::Find( iModel );
			if ( pCarCols )
			{
				pCarCols->GenerateRandCol();
				if ( iColour1 == -1 ) 
				{
					iColour1 = pCarCols->RandomColour1();
					pVehicle->SetRandomColour1( true );
				}
				if ( iColour2 == -1 ) 
				{
					iColour2 = pCarCols->RandomColour2();
					pVehicle->SetRandomColour2( true );
				}
			}
			else
			{
				if ( iColour1 == -1 ) iColour1 = 0;
				if ( iColour2 == -1 ) iColour2 = 0;
			}

			pVehicle->SetColour1( (BYTE)iColour1 );
			pVehicle->SetColour2( (BYTE)iColour2 );

			pVehicle->SetSpawnX( vPos->x );
			pVehicle->SetSpawnY( vPos->y );
			pVehicle->SetSpawnZ( vPos->z );
			pVehicle->SetSpawnRotation( fAngle );

			pVehicle->SetSpawned( true );

			unsigned char uc = 0, uc1 = 0;

			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnVehicleForPlayer( uc, ucID );
					uc1++;
				}
				uc++;
			}

			CSquirrel* pScript = CSquirrelManager::Find( v );
			if ( pScript )
			{
				push_Vehicle( pVehicle );
				return 1;
			}
		}
	}
	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::FindVehicle )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iVehicleID = NULL;
		sq_getinteger( v, 2, &iVehicleID );

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( (unsigned int)iVehicleID );
		if ( pVehicle )
		{
			CSquirrel* pScript = CSquirrelManager::Find( v );
			if ( pScript )
			{
				push_Vehicle( pVehicle );
				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetCount )
{
	sq_pushinteger( v, CVehiclePoolManager::Count() );
	return 1;
}

/*
//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetID )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			sq_pushinteger( v, (SQInteger)pVehicle->GetID() );
			return 1;
		}
	}

	sq_pushinteger( v, (SQInteger)255 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetModel )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			sq_pushinteger( v, (SQInteger)pVehicle->GetModel() );
			return 1;
		}
	}

	sq_pushinteger( v, (SQInteger)255 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetHealth )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			sq_pushfloat( v, (SQFloat)pVehicle->GetHealth() );
			return 1;
		}
	}

	sq_pushfloat( v, (SQFloat)0 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::SetHealth )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQFloat fHealth = NULL;
		sq_getfloat( v, 3, &fHealth );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			pVehicle->SetHealth( (float)fHealth );
			CNetSends::SetVehicleHealth( (unsigned char)pVehicle->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::Blow )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			pVehicle->SetHealth( 100000.0f );
			CNetSends::SetVehicleHealth( pVehicle->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::SetIndestructible )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			pVehicle->SetHealth( 1000.0f );
			CNetSends::SetVehicleHealth( pVehicle->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetColour1 )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			sq_pushinteger( v, (SQInteger)pVehicle->GetColour1() );
			return 1;
		}
	}

	sq_pushinteger( v, (SQInteger)-1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetColour2 )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			sq_pushinteger( v, (SQInteger)pVehicle->GetColour2() );
			return 1;
		}
	}

	sq_pushinteger( v, (SQInteger)-1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetDriver )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			sq_pushinteger( v, (SQInteger)pVehicle->GetDriver() );
			return 1;
		}
	}

	sq_pushinteger( v, (SQInteger)INVALID_PLAYER_ID );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::IsLocked )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			sq_pushbool( v, (SQBool)pVehicle->GetLocked() );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::SetLocked )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQBool bLocked = NULL;
		sq_getbool( v, 3, &bLocked );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{	
			pVehicle->SetLocked( (bool)bLocked );
			CNetSends::SetVehicleDoorStatus( (unsigned char)pVehicle->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::KillEngine )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{	
			CNetSends::KillVehicleEngine( (unsigned char)pVehicle->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetPos )
{
	SQInteger iArgs = sq_gettop( v );
	Vector *vPos = new Vector();

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			vPos->x = pVehicle->GetX(); 
			vPos->y = pVehicle->GetY();
			vPos->z = pVehicle->GetZ();
		}
	}
	Push( v, vPos );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::SetPos )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		Vector* vPos = Get( TypeWrapper<Vector*>(), v, 3 );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			pVehicle->SetX( vPos->x );
			pVehicle->SetY( vPos->y );
			pVehicle->SetZ( vPos->z );

			CNetSends::SetVehiclePosition( pVehicle->GetID() );
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

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			pVehicle->SetX( (float)fX ); 
			pVehicle->SetY( (float)fY ); 
			pVehicle->SetZ( (float)fZ );
			CNetSends::SetVehiclePosition( pVehicle->GetID() );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::SetSpawnInfo )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 7 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iModel = NULL;
		sq_getinteger( v, 3, &iModel );

		Vector* vPos = Get( TypeWrapper<Vector*>(), v, 4 );

		SQFloat fAngle = NULL;
		sq_getfloat( v, 5, &fAngle );

		SQInteger iColour1 = NULL;
		sq_getinteger( v, 6, &iColour1 );

		SQInteger iColour2 = NULL;
		sq_getinteger( v, 7, &iColour2 );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			pVehicle->SetModel( (BYTE)iModel );

			pVehicle->SetSpawnX( vPos->x );
			pVehicle->SetSpawnY( vPos->y );
			pVehicle->SetSpawnZ( vPos->z );
			pVehicle->SetSpawnRotation( fAngle );

			CCarCols* pCarCols = CCarColManager::Find( iModel );
			if ( pCarCols )
			{
				pCarCols->GenerateRandCol();
				if ( iColour1 == -1 ) iColour1 = pCarCols->RandomColour1();
				if ( iColour2 == -1 ) iColour2 = pCarCols->RandomColour2();
			}

			pVehicle->SetColour1( iColour1 );
			pVehicle->SetColour2( iColour2 );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::MoveTo )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		Vector* vPos = Get( TypeWrapper<Vector*>(), v, 3 );

		SQFloat fAngle = NULL;
		sq_getfloat( v, 4, &fAngle );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			pVehicle->SetX( vPos->x );
			pVehicle->SetY( vPos->y );
			pVehicle->SetZ( vPos->z );
			pVehicle->SetRotation( fAngle );

			unsigned int uc = 0, uc1 = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnVehicleForPlayer( uc, pVehicle->GetID() );
					uc1++;
				}
				uc++;
			}

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::Respawn )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CVehiclePool* pVehicle = (CVehiclePool*)pPointer;
		if ( pVehicle )
		{
			pVehicle->SetX( pVehicle->GetSpawnX() );
			pVehicle->SetY( pVehicle->GetSpawnY() );
			pVehicle->SetZ( pVehicle->GetSpawnZ() );
			pVehicle->SetRotation( pVehicle->GetSpawnRotation() );
			pVehicle->SetBeenUsed( false );
			pVehicle->SetLastUsedTime( 0 );

			if ( ( pVehicle->GetRandomColour1() ) || ( pVehicle->GetRandomColour2() ) )
			{
				CCarCols* pCarCols = CCarColManager::Find( pVehicle->GetModel() );
				if ( pCarCols )
				{
					if ( pVehicle->GetRandomColour1() ) pVehicle->SetColour1( pCarCols->RandomColour1() );
					if ( pVehicle->GetRandomColour2() ) pVehicle->SetColour2( pCarCols->RandomColour2() );
				}
			}

			unsigned int uc = 0, uc1 = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnVehicleForPlayer( uc, pVehicle->GetID() );
					uc1++;
				}
				uc++;
			}

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}
*/
//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetNameFromModel )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iModelID = NULL;
		sq_getinteger( v, 2, &iModelID );

		sq_pushstring( v, CUtils::GetVehicleName( (BYTE)iModelID ), -1 );
		return 1;
	}

	sq_pushstring( v, "Unknown", 7 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQVehicleFuncs::GetModelFromName )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		const SQChar* szName = 0;
		sq_getstring( v, 2, &szName );

		sq_pushinteger( v, CUtils::GetVehicleModel( (char*)szName ) );
		return 1;
	}

	sq_pushinteger( v, 255 );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, _get )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );
	const SQChar *s = sa.GetString( 2 );

	if ( self )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );

		if ( !strcmp( (char*)s, "ID" ) )
		{
			sa.Return( pVehicle->GetID() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Model" ) )
		{
			sa.Return( pVehicle->GetModel() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Health" ) )
		{
			sa.Return( pVehicle->GetHealth() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Colour1" ) )
		{
			sa.Return( pVehicle->GetColour1() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Colour2" ) )
		{
			sa.Return( pVehicle->GetColour2() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Driver" ) )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( pVehicle->GetDriver() );
			if ( pPlayer )
			{
				CSquirrel* pScript = CSquirrelManager::Find( v );
				if ( pScript )
				{
					push_Player( pPlayer );
					return 1;
				}
			}

			sq_pushnull( v );
			return 1;
		}

		else if ( !strcmp( (char*)s, "IsLocked" ) )
		{
			sa.Return( pVehicle->GetLocked() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Pos" ) )
		{
			Vector* vPos = new Vector();
			vPos->x = pVehicle->GetX();
			vPos->y = pVehicle->GetY();
			vPos->z = pVehicle->GetZ();

			push_Vector( vPos );
			return 1;
		}
	}

	sa.Return();
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, _set )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );
	const SQChar *s = sa.GetString( 2 );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	if ( !strcmp( (char*)s, "Pos" ) )
	{
		_CHECK_INST_PARAM( vPos, 3, Vector, Vector );

		if ( pVehicle )
		{
			pVehicle->SetX( vPos->x );
			pVehicle->SetY( vPos->y );
			pVehicle->SetZ( vPos->z );

			CNetSends::SetVehiclePosition( pVehicle->GetID() );

			sa.Return( true );
			return 1;
		}
	}
	else if ( !strcmp( (char*)s, "IsLocked" ) )
	{
		bool bLocked = sa.GetBool( 3 );

		pVehicle->SetLocked( (bool)bLocked );
		CNetSends::SetVehicleDoorStatus( (unsigned char)pVehicle->GetID() );

		sa.Return( true );
		return 1;
	}
	else if ( !strcmp( (char*)s, "Health" ) )
	{
		bool fHealth = sa.GetFloat( 3 );

		pVehicle->SetHealth( fHealth );
		CNetSends::SetVehicleHealth( pVehicle->GetID() );

		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, _tostring )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	sa.Return( CUtils::GetVehicleName( (BYTE)pVehicle->GetModel() ) );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, _cmp )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );
	_CHECK_INST_PARAM( vehicle, 2, CVehiclePool, Vehicle );

	if ( self->GetID() == vehicle->GetID() ) sa.Return( true );
	else sa.Return( false );

	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Vehicle, ID )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	sa.Return( pVehicle->GetID() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, Model )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	sa.Return( pVehicle->GetModel() );
	return 1;
}
*/
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, Respawn )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	pVehicle->SetX( pVehicle->GetSpawnX() );
	pVehicle->SetY( pVehicle->GetSpawnY() );
	pVehicle->SetZ( pVehicle->GetSpawnZ() );
	pVehicle->SetRotation( pVehicle->GetSpawnRotation() );

	pVehicle->SetLastUsedTime( RakNet::GetTime() );
	pVehicle->SetBeenUsed( false );
	pVehicle->SetHealth( 1000.0f );

	unsigned int uc = 0, uc1 = 0;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CNetSends::SpawnVehicleForPlayer( uc, pVehicle->GetID() );
			uc1++;
		}
		uc++;
	}

	CScripting::PushFunction( "onVehicleRespawn" );
	CScripting::PushVehiclePointer( pVehicle );
	CScripting::CallFunction();

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, RespawnAt )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	_CHECK_INST_PARAM( vPos, 2, Vector, Vector );
	float fAngle = sa.GetFloat( 3 );

	pVehicle->SetX( vPos->x );
	pVehicle->SetY( vPos->y );
	pVehicle->SetZ( vPos->z );
	pVehicle->SetRotation( fAngle );

	pVehicle->SetLastUsedTime( RakNet::GetTime() );
	pVehicle->SetBeenUsed( false );
	pVehicle->SetHealth( 1000.0f );

	unsigned int uc = 0, uc1 = 0;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CNetSends::SpawnVehicleForPlayer( uc, pVehicle->GetID() );
			uc1++;
		}
		uc++;
	}

	CScripting::PushFunction( "onVehicleRespawn" );
	CScripting::PushVehiclePointer( pVehicle );
	CScripting::CallFunction();

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, RespawnAs )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	int iModel = sa.GetInt( 2 );
	_CHECK_INST_PARAM( vPos, 3, Vector, Vector );
	float fAngle = sa.GetFloat( 4 );
	int iColour1 = sa.GetInt( 5 );
	int iColour2 = sa.GetInt( 6 );

	pVehicle->SetX( vPos->x );
	pVehicle->SetY( vPos->y );
	pVehicle->SetZ( vPos->z );
	pVehicle->SetRotation( fAngle );

	pVehicle->SetModel( (BYTE)iModel );

	pVehicle->SetSpawnX( vPos->x );
	pVehicle->SetSpawnY( vPos->y );
	pVehicle->SetSpawnZ( vPos->z );
	pVehicle->SetSpawnRotation( fAngle );

	pVehicle->SetLastUsedTime( RakNet::GetTime() );
	pVehicle->SetBeenUsed( false );
	pVehicle->SetHealth( 1000.0f );

	CCarCols* pCarCols = CCarColManager::Find( (BYTE)iModel );
	if ( pCarCols )
	{
		pCarCols->GenerateRandCol();
		if ( iColour1 == -1 )
		{
				iColour1 = pCarCols->RandomColour1();
				pVehicle->SetRandomColour1( true );
		}
		if ( iColour2 == -1 )
		{
			iColour2 = pCarCols->RandomColour2();
			pVehicle->SetRandomColour2( true );
		}
	}
	else
	{
		if ( iColour1 == -1 ) iColour1 = 0;
		if ( iColour2 == -1 ) iColour2 = 0;
	}

	pVehicle->SetColour1( (BYTE)iColour1 );
	pVehicle->SetColour2( (BYTE)iColour2 );

	unsigned int uc = 0, uc1 = 0;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CNetSends::SpawnVehicleForPlayer( uc, pVehicle->GetID() );
			uc1++;
		}
		uc++;
	}

	CScripting::PushFunction( "onVehicleRespawn" );
	CScripting::PushVehiclePointer( pVehicle );
	CScripting::CallFunction();

	sa.Return( true );
	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Vehicle, Health )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	sa.Return( pVehicle->GetHealth() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, Colour1 )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	sa.Return( pVehicle->GetColour1() );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, Colour2 )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	sa.Return( pVehicle->GetColour2() );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, Driver )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( pVehicle->GetDriver() );
	if ( pPlayer )
	{
		CSquirrel* pScript = CSquirrelManager::Find( v );
		if ( pScript )
		{
			push_Player( pPlayer );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, IsLocked )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	sa.Return( pVehicle->GetLocked() );
	return 1;
}
*/
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Vehicle, KillEngine )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	CNetSends::KillVehicleEngine( (unsigned char)pVehicle->GetID() );
	sa.Return( true );
	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Vehicle, Pos )
{
	StackHandler sa(v);
	_CHECK_SELF( CVehiclePool, Vehicle );

	CVehiclePool* pVehicle = CVehiclePoolManager::Find( self->GetID() );
	
	Vector* vPos = new Vector();
	vPos->x = pVehicle->GetX();
	vPos->y = pVehicle->GetY();
	vPos->z = pVehicle->GetZ();

	Push( v, vPos );
	return 1;
}
*/
//--------------------------------------------------


_BEGIN_CLASS( Vehicle )

_MEMBER_FUNCTION( Vehicle, _get, 2, _T("xs") )
_MEMBER_FUNCTION( Vehicle, _set, 3, _T("xs|n") )
_MEMBER_FUNCTION( Vehicle, _tostring, 1, _T("x") )
_MEMBER_FUNCTION( Vehicle, _cmp, 1, _T("x") )
//_MEMBER_FUNCTION( Vehicle, ID, 1, "x" )
//_MEMBER_FUNCTION( Vehicle, Model, 1, "x" )
_MEMBER_FUNCTION( Vehicle, Respawn, 1, "x" )
_MEMBER_FUNCTION( Vehicle, RespawnAt, 3, "x.f" )
_MEMBER_FUNCTION( Vehicle, RespawnAs, 6, "xi.fii" )
//_MEMBER_FUNCTION( Vehicle, Health, 1, "x" )
//_MEMBER_FUNCTION( Vehicle, Colour1, 1, "x" )
//_MEMBER_FUNCTION( Vehicle, Colour2, 1, "x" )
//_MEMBER_FUNCTION( Vehicle, Driver, 1, "x" )
//_MEMBER_FUNCTION( Vehicle, IsLocked, 1, "x" )
_MEMBER_FUNCTION( Vehicle, KillEngine, 1, "x" )
//_MEMBER_FUNCTION( Vehicle, Pos, 1, "x" )

_END_CLASS( Vehicle );