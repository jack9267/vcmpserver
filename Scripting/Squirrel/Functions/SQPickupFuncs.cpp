/*
	SQPickupFuncs.cpp
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

#include "SQPickupFuncs.h"
#include "Pools.h"

_SQUIRRELDEF( CSQPickupFuncs::Create )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 || iArgs == 4 )
	{
		SQInteger iModel = NULL;
		sq_getinteger( v, 2, &iModel );

		_CHECK_INST_PARAM( vPos, 3, Vector, Vector );

		unsigned char ucForPlayer = INVALID_PLAYER_ID;

		if ( iArgs == 4 )
		{
			_CHECK_INST_PARAM( pPlayer, 4, CPickupPool, Pickup );

			if ( pPlayer ) ucForPlayer = pPlayer->GetID();

			else
			{
				sq_pushnull( v );
				return 1;
			}
		}
		
		CPickupPool* pPickup = CPickupPoolManager::New();
		if ( pPickup )
		{

			pPickup->SetModel( iModel );
			pPickup->SetX( vPos->x );
			pPickup->SetY( vPos->y );
			pPickup->SetZ( vPos->z );

			if ( ucForPlayer != INVALID_PLAYER_ID )
			{
				pPickup->SetSpawnForPlayer( ucForPlayer );
				CNetSends::SpawnPickupForPlayer( ucForPlayer, pPickup->GetID(), ucForPlayer );

				CSquirrel* pScript = CSquirrelManager::Find( v );
				if ( pScript )
				{
					push_Pickup( pPickup );
					return 1;
				}
			}

			unsigned char uc = 0, uc1 = 0;

			while ( uc < MAX_PLAYERS && uc1 < CPlayerPoolManager::Count() )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnPickupForPlayer( uc, pPickup->GetID() );
					uc1++;
				}
				uc++;
			}

			CSquirrel* pScript = CSquirrelManager::Find( v );
			if ( pScript )
			{
				push_Pickup( pPickup );
				return 1;
			}
		}
	}
	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------
/*
_SQUIRRELDEF( CSQPickupFuncs::Remove )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			CNetSends::DeletePickupForAll( pPickup->GetID() );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}
*/
//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::FindPickup )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iPickupID = NULL;
		sq_getinteger( v, 2, &iPickupID );

		CPickupPool* pPickup = CPickupPoolManager::Find( (unsigned int)iPickupID );
		if ( pPickup )
		{
			CSquirrel* pScript = CSquirrelManager::Find( v );
			if ( pScript )
			{
				push_Pickup( pPickup );

				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::GetCount )
{
	sq_pushinteger( v, CPickupPoolManager::Count() );
	return 1;
}

//--------------------------------------------------
/*
_SQUIRRELDEF( CSQPickupFuncs::GetID )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			sq_pushinteger( v, (SQInteger)pPickup->GetID() );
			return 1;
		}
	}

	sq_pushinteger( v, (SQInteger)-1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::GetModel )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			sq_pushinteger( v, (SQInteger)pPickup->GetModel() );
			return 1;
		}
	}

	sq_pushinteger( v, (SQInteger)-1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::SetModel )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iModel = NULL;
		sq_getinteger( v, 3, &iModel );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			pPickup->SetModel( iModel );

			if ( pPickup->GetSpawned() )
			{
				if ( pPickup->GetSpawnForPlayer() != INVALID_PLAYER_ID )
				{
					CNetSends::DeletePickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID() );
					CNetSends::SpawnPickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID(), pPickup->GetSpawnForPlayer() );

					sq_pushbool( v, (SQBool)true );
					return 1;
				}

				unsigned int uc = 0, uc1 = 0;

				while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
				{
					CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
					if ( pPlayer )
					{
						CNetSends::DeletePickupForPlayer( uc, pPickup->GetID() );
						CNetSends::SpawnPickupForPlayer( uc, pPickup->GetID(), uc );
						uc1++;
					}
					uc++;
				}
			}

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::IsSpawned )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			sq_pushbool( v, (SQBool)pPickup->GetSpawned() );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::SetOnceOnly )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			pPickup->SetOneTime( true );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::SetRespawnTime )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iTime = NULL;
		sq_getinteger( v, 3, &iTime );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			pPickup->SetRespawnTime( iTime * 1000 );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}
*/
//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::GetGlobalRespawnTime )
{
	SQInteger iArgs = sq_gettop( v );

	sq_pushinteger( v, (SQInteger)( CConfigLoader::GetPickupRespawnTime() / 1000 ) );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::SetGlobalRespawnTime )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQInteger iTime = NULL;
		sq_getinteger( v, 2, &iTime );

		CConfigLoader::SetPickupRespawnTime( iTime * 1000 );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------
/*
_SQUIRRELDEF( CSQPickupFuncs::GetPos )
{
	SQInteger iArgs = sq_gettop( v );
	Vector *vPos = new Vector();

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			vPos->x = pPickup->GetX(); 
			vPos->y = pPickup->GetY();
			vPos->z = pPickup->GetZ();
		}
	}
	Push( v, vPos );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::SetPos )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		Vector* vPos = Get( TypeWrapper<Vector*>(), v, 3 );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			pPickup->SetX( vPos->x );
			pPickup->SetY( vPos->y );
			pPickup->SetZ( vPos->z );

			if ( pPickup->GetSpawned() )
			{
				if ( pPickup->GetSpawnForPlayer() != INVALID_PLAYER_ID )
				{
					CNetSends::DeletePickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID() );
					CNetSends::SpawnPickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID(), pPickup->GetSpawnForPlayer() );

					sq_pushbool( v, (SQBool)true );
					return 1;
				}

				unsigned int uc = 0, uc1 = 0;

				while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
				{
					CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
					if ( pPlayer )
					{
						CNetSends::DeletePickupForPlayer( uc, pPickup->GetID() );
						CNetSends::SpawnPickupForPlayer( uc, pPickup->GetID(), uc );
						uc1++;
					}
					uc++;
				}
			}

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::Respawn )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		if ( pPickup )
		{
			if ( !pPickup->GetSpawned() )
			{
				if ( pPickup->GetSpawnForPlayer() != INVALID_PLAYER_ID )
				{
					CNetSends::SpawnPickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID(), pPickup->GetSpawnForPlayer() );

					sq_pushbool( v, (SQBool)true );
					return 1;
				}

				unsigned int uc = 0, uc1 = 0;

				while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
				{
					CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
					if ( pPlayer )
					{
						CNetSends::SpawnPickupForPlayer( uc, pPickup->GetID(), uc );
						uc1++;
					}
					uc++;
				}
			}

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::SpawnForPlayer )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQUserPointer pPlayerPointer = NULL;
		sq_getuserpointer( v, 3, &pPlayerPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		CPlayerPool* pPlayer = (CPlayerPool*)pPlayerPointer;

		if ( ( pPickup ) && ( pPlayer ) )
		{
			if ( !pPickup->GetSpawned() )
			{
				CNetSends::SpawnPickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID(), pPickup->GetSpawnForPlayer() );

				sq_pushbool( v, (SQBool)true );
				return 1;
			}

			if ( pPickup->GetSpawnForPlayer() != pPlayer->GetID() )
			{
				CNetSends::SpawnPickupForPlayer( pPlayer->GetID(), pPickup->GetID(), pPlayer->GetID() );
			}

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQPickupFuncs::HideForPlayer )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQUserPointer pPlayerPointer = NULL;
		sq_getuserpointer( v, 3, &pPlayerPointer );

		CPickupPool* pPickup = (CPickupPool*)pPointer;
		CPlayerPool* pPlayer = (CPlayerPool*)pPlayerPointer;

		if ( ( pPickup ) && ( pPlayer ) )
		{
			if ( pPickup->GetSpawned() )
			{
				CNetSends::DeletePickupForPlayer( pPlayer->GetID(), pPickup->GetID() );
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


_MEMBER_FUNCTION_IMPL( Pickup, _get )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );
	const SQChar *s = sa.GetString( 2 );
	
	if ( self )
	{
		CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );

		if ( !strcmp( (char*)s, "ID" ) )
		{
			sa.Return( (int)pPickup->GetID() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Model" ) )
		{
			sa.Return( (int)pPickup->GetModel() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "IsSpawned" ) )
		{
			sa.Return( pPickup->GetSpawned() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "RespawnTime" ) )
		{
			sa.Return( (int)pPickup->GetRespawnTime() );
			return 1;
		}

		else if ( !strcmp( (char*)s, "Pos" ) )
		{
			Vector* vPos = new Vector();
			vPos->x = pPickup->GetX();
			vPos->y = pPickup->GetY();
			vPos->z = pPickup->GetZ();

			push_Vector( vPos );
			return 1;
		}
	}

	sa.Return();
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, _set )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );
	const SQChar *s = sa.GetString( 2 );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );
	if ( !strcmp( (char*)s, "Pos" ) )
	{
		_CHECK_INST_PARAM( vPos, 3, Vector, Vector );

		pPickup->SetX( vPos->x );
		pPickup->SetY( vPos->y );
		pPickup->SetZ( vPos->z );

		if ( pPickup->GetSpawned() )
		{
			if ( pPickup->GetSpawnForPlayer() != INVALID_PLAYER_ID )
			{
				CNetSends::DeletePickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID() );
				CNetSends::SpawnPickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID(), pPickup->GetSpawnForPlayer() );

				sa.Return( true );
				return 1;
			}

			unsigned int uc = 0, uc1 = 0;

			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::DeletePickupForPlayer( uc, pPickup->GetID() );
					CNetSends::SpawnPickupForPlayer( uc, pPickup->GetID(), uc );
					uc1++;
				}
				uc++;
			}
		}

		sa.Return( true );
		return 1;
	}
	else if ( !strcmp( (char*)s, "Model" ) )
	{
		int iModel = sa.GetInt( 3 );

		pPickup->SetModel( (unsigned int)iModel );

		if ( pPickup->GetSpawned() )
		{
			if ( pPickup->GetSpawnForPlayer() != INVALID_PLAYER_ID )
			{
				CNetSends::DeletePickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID() );
				CNetSends::SpawnPickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID(), pPickup->GetSpawnForPlayer() );

				sa.Return( true );
				return 1;
			}

			unsigned int uc = 0, uc1 = 0;

			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::DeletePickupForPlayer( uc, pPickup->GetID() );
					CNetSends::SpawnPickupForPlayer( uc, pPickup->GetID(), uc );
					uc1++;
				}
				uc++;
			}
		}

		sa.Return( true );
		return 1;
	}
	else if ( !strcmp( (char*)s, "RespawnTime" ) )
	{
		int iTime = sa.GetInt( 3 );

		pPickup->SetRespawnTime( (unsigned int)iTime * 1000 );

		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, _cmp )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );
	_CHECK_INST_PARAM( pickup, 2, CPickupPool, Pickup );

	if ( self->GetID() == pickup->GetID() ) sa.Return( true );
	else sa.Return( false );

	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, Remove )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CScripting::PushFunction( "onPickupDestroy" );
	CScripting::PushPickupPointer( self );
	CScripting::CallFunction();

	CNetSends::DeletePickupForAll( self->GetID() );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Pickup, ID )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	sa.Return( (int)self->GetID() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, Model )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );
	
	sa.Return( (int)pPickup->GetModel() );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, IsSpawned )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );
	
	sa.Return( pPickup->GetSpawned() );
	return 1;
}
*/
//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, Respawn )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );

	if ( !pPickup->GetSpawned() )
	{
		if ( pPickup->GetSpawnForPlayer() != INVALID_PLAYER_ID )
		{
			CNetSends::SpawnPickupForPlayer( pPickup->GetSpawnForPlayer(), pPickup->GetID(), pPickup->GetSpawnForPlayer() );
			pPickup->SetSpawned( true );

			CScripting::PushFunction( "onPickupRespawn" );
			CScripting::PushPickupPointer( pPickup );
			CScripting::CallFunction();

			sa.Return( true );
			return 1;
		}

		unsigned int uc = 0, uc1 = 0;

		while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				CNetSends::SpawnPickupForPlayer( uc, pPickup->GetID(), uc );
				uc1++;
			}
			uc++;
		}

		CScripting::PushFunction( "onPickupRespawn" );
		CScripting::PushPickupPointer( pPickup );
		CScripting::CallFunction();

		pPickup->SetSpawned( true );

		sa.Return( true );
		return 1;
	}
	
	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, SpawnForPlayer )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );

	_CHECK_INST_PARAM( pPlayer, 2, CPlayerPool, Player );

	if ( pPlayer )
	{
		CNetSends::SpawnPickupForPlayer( pPlayer->GetID(), pPickup->GetID(), pPlayer->GetID() );
		if ( pPickup->GetSpawnForPlayer() == pPlayer->GetID() ) pPickup->SetSpawned( true );

		sa.Return( true );
		return 1;
	}
	
	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, HideForPlayer )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );

	_CHECK_INST_PARAM( pPlayer, 2, CPlayerPool, Player );

	if ( pPlayer )
	{
		if ( pPickup->GetSpawned() )
		{
			CNetSends::DeletePickupForPlayer( pPlayer->GetID(), pPickup->GetID() );
			if ( pPickup->GetSpawnForPlayer() == pPlayer->GetID() ) pPickup->SetSpawned( false );
		}

		sa.Return( true );
		return 1;
	}
	
	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, SetOnceOnly )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );

	pPickup->SetOneTime( true );
	
	sa.Return( true );
	return 1;
}

//--------------------------------------------------
/*
_MEMBER_FUNCTION_IMPL( Pickup, RespawnTime )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );

	sa.Return( (int)( pPickup->GetRespawnTime() / 1000 ) );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Pickup, Pos )
{
	StackHandler sa(v);
	_CHECK_SELF( CPickupPool, Pickup );

	CPickupPool* pPickup = CPickupPoolManager::Find( self->GetID() );
	
	Vector* vPos = new Vector();
	vPos->x = pPickup->GetX();
	vPos->y = pPickup->GetY();
	vPos->z = pPickup->GetZ();

	Push( v, vPos );
	return 1;
}
*/
//--------------------------------------------------


_BEGIN_CLASS( Pickup )

_MEMBER_FUNCTION( Pickup, _get, 2, _T("xs") )
_MEMBER_FUNCTION( Pickup, _set, 3, _T("xs|n") )
_MEMBER_FUNCTION( Pickup, _cmp, 1, _T("x") )
_MEMBER_FUNCTION( Pickup, Remove, 1, "x" )
//_MEMBER_FUNCTION( Pickup, ID, 1, "x" )
//_MEMBER_FUNCTION( Pickup, Model, 1, "x" )
//_MEMBER_FUNCTION( Pickup, IsSpawned, 1, "x" )
_MEMBER_FUNCTION( Pickup, Respawn, 1, "x" )
_MEMBER_FUNCTION( Pickup, SpawnForPlayer, 2, "x." )
_MEMBER_FUNCTION( Pickup, HideForPlayer, 2, "x." )
_MEMBER_FUNCTION( Pickup, SetOnceOnly, 1, "x" )
//_MEMBER_FUNCTION( Pickup, RespawnTime, 1, "x" )
//_MEMBER_FUNCTION( Pickup, Pos, 1, "x" )

_END_CLASS( Pickup );