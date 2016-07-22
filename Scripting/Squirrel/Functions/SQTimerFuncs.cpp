/*
	SQTimerFuncs.cpp
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

	File Author: VRocker

	--------------------------------------------------------------------
*/

#include "SQTimerFuncs.h"

_SQUIRRELDEF( CSQTimerFuncs::New )
{
	// Arguments - Function, Time, TimesToLoop, Params
	SQInteger iArgs = sq_gettop( v ); // Get args

	if ( iArgs >= 4 )
	{
		const SQChar* szFunction = NULL;
		SQInteger iTime = 0, iTimesToLoop = 0;

		sq_getstring( v, 2, &szFunction );

		sq_getinteger( v, 3, &iTime );
		sq_getinteger( v, 4, &iTimesToLoop );

		CSquirrelTimers* pTimer = CSquirrelTimerManager::New();
		if ( pTimer )
		{
			pTimer->SetFunction( (char*)szFunction );
			pTimer->SetInterval( iTime );
			pTimer->SetLoopTimes( iTimesToLoop );

			if ( iArgs > 4 )
			{
				for ( unsigned char uc = 5; uc <= iArgs; uc++ )
				{
					SQInteger i = 0;
					SQFloat f = 0.0f;
					const SQChar* sz = NULL;
					SQUserPointer p = NULL;

					switch( sq_gettype( v, uc ) )
					{
					case OT_INTEGER:	
						sq_getinteger( v, uc, &i );

						pTimer->PushInteger( (int)i );
						break;

					case OT_FLOAT:
						sq_getfloat( v, uc, &f );

						pTimer->PushFloat( (float)f );
						break;

					case OT_STRING:
						sq_getstring( v, uc, &sz );

						pTimer->PushString( (char*)sz );
						break;

					case OT_USERPOINTER:
						sq_getuserpointer( v, uc, &p );

						pTimer->PushUserData( (void*)p );
						break;

					case OT_INSTANCE:
						// Needs doing for players!
						if( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, (SQUserPointer)&__Player_decl ) ) )
						{
							// Push Player
							pTimer->PushPlayerPointer( (CPlayerPool*)p );
						}
						else if( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, (SQUserPointer)&__Vehicle_decl ) ) )
						{
							// Push Vehicle
							pTimer->PushVehiclePointer( (CVehiclePool*)p );
						}
						else if( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, (SQUserPointer)&__Pickup_decl ) ) )
						{
							// Push Pickup
							pTimer->PushPickupPointer( (CPickupPool*)p );
						}
						else if( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, (SQUserPointer)&__Socket_decl ) ) )
						{
							// Push Socket
							pTimer->PushSocketPointer( (CSquirrelSockets*)p );
						}
						else 
						{
							sq_gettypetag( v, uc, &p );
							if ( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, p ) ) )
							{
								//pTimer->PushInstance( 
							}
						}
						break;
					}
				}
			}

			pTimer->Start();

			CSquirrel* pSquirrel = CSquirrelManager::Find( v );
			if ( pSquirrel )
			{
				pTimer->SetParentScript( pSquirrel->GetID() );
				if ( pSquirrel->GetObjects() ) pSquirrel->GetObjects()->AddTimer( pTimer );
			}

			push_Timer( pTimer );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}
/*
_SQUIRRELDEF( CSQTimerFuncs::Delete )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer p = NULL;

		sq_getuserpointer( v, 2, &p );

		CSquirrelTimers* pTimer = (CSquirrelTimers*)p;
		if ( pTimer )
		{
			CSquirrel* pSquirrel = CSquirrelManager::Find( v );
			if ( pSquirrel )
			{
				// Dont send any data! RemoveVehicle will handle it
				if ( pSquirrel->GetObjects() ) pSquirrel->GetObjects()->RemoveTimer( pTimer );

				sq_pushbool( v, true );
				return 1;
			}
		}
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQTimerFuncs::Start )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer p = NULL;

		sq_getuserpointer( v, 2, &p );

		CSquirrelTimers* pTimer = (CSquirrelTimers*)p;
		if ( pTimer )
		{
			pTimer->Start();

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQTimerFuncs::Stop )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer p = NULL;

		sq_getuserpointer( v, 2, &p );

		CSquirrelTimers* pTimer = (CSquirrelTimers*)p;
		if ( pTimer )
		{
			pTimer->Stop();

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}
*/


//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Timer, Delete )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelTimers, Timer );

	CSquirrelTimers* pTimer = CSquirrelTimerManager::Find( self->GetID() );

	CSquirrel* pSquirrel = CSquirrelManager::Find( v );
	if ( pSquirrel )
	{
		if ( pSquirrel->GetObjects() ) pSquirrel->GetObjects()->RemoveTimer( pTimer );

		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Timer, Start )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelTimers, Timer );

	CSquirrelTimers* pTimer = CSquirrelTimerManager::Find( self->GetID() );

	pTimer->Start();

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Timer, Stop )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelTimers, Timer );

	CSquirrelTimers* pTimer = CSquirrelTimerManager::Find( self->GetID() );

	pTimer->Stop();

	sa.Return( true );
	return 1;
}

//--------------------------------------------------


_BEGIN_CLASS( Timer )

_MEMBER_FUNCTION( Timer, Delete, 1, "x" )
_MEMBER_FUNCTION( Timer, Start, 1, "x" )
_MEMBER_FUNCTION( Timer, Stop, 1, "x" )

_END_CLASS( Timer );