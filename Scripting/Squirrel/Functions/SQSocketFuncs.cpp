/*
	SQSocketFuncs.cpp
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

#include "SQSocketFuncs.h"

_SQUIRRELDEF( CSQSocketFuncs::New )
{
	// Arguments - Function
	SQInteger iArgs = sq_gettop( v ); // Get args

	if ( iArgs >= 2 )
	{
		const SQChar* szFunction = NULL;

		sq_getstring( v, 2, &szFunction );

		CSquirrelSockets* pSocket = CSquirrelSocketManager::New();
		if ( pSocket )
		{
			pSocket->SetFunction( (char*)szFunction );
			
			CSquirrel* pSquirrel = CSquirrelManager::Find( v );
			if ( pSquirrel )
			{
				pSocket->SetParentScript( pSquirrel->GetID() );
				if ( pSquirrel->GetObjects() ) pSquirrel->GetObjects()->AddSocket( pSocket );
			}

			push_Socket( pSocket );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}
/*
_SQUIRRELDEF( CSQSocketFuncs::Delete )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer p = NULL;

		sq_getuserpointer( v, 2, &p );

		CSquirrelSockets* pSocket = (CSquirrelSockets*)p;
		if ( pSocket )
		{
			CSquirrel* pSquirrel = CSquirrelManager::Find( v );
			if ( pSquirrel )
			{
				// Dont send any data! RemoveVehicle will handle it
				if ( pSquirrel->GetObjects() ) pSquirrel->GetObjects()->RemoveSocket( pSocket );

				sq_pushbool( v, true );
				return 1;
			}
		}
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQSocketFuncs::Connect )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		SQUserPointer p = NULL;
		const SQChar* szHost = NULL;
		SQInteger iPort = 5193;

		sq_getuserpointer( v, 2, &p );
		sq_getstring( v, 3, &szHost );
		sq_getinteger( v, 4, &iPort );

		CSquirrelSockets* pSocket = (CSquirrelSockets*)p;
		if ( pSocket )
		{
			pSocket->Connect( (const char*)szHost, (unsigned short)iPort );
			sq_pushbool( v, true );

			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQSocketFuncs::Start )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		SQUserPointer p = NULL;
		SQInteger iPort = 5193, iMaxConns = 1;

		sq_getuserpointer( v, 2, &p );
		sq_getinteger( v, 3, &iPort );
		sq_getinteger( v, 4, &iMaxConns );

		CSquirrelSockets* pSocket = (CSquirrelSockets*)p;
		if ( pSocket )
		{
			pSocket->Start( (unsigned short)iPort, (unsigned short)iMaxConns );

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQSocketFuncs::Stop )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer p = NULL;

		sq_getuserpointer( v, 2, &p );

		CSquirrelSockets* pSocket = (CSquirrelSockets*)p;
		if ( pSocket )
		{
			pSocket->Stop();

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQSocketFuncs::SetNewConnFunc )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer p = NULL;
		const SQChar* szFunc = 0;

		sq_getuserpointer( v, 2, &p );
		sq_getstring( v, 3, &szFunc );

		CSquirrelSockets* pSocket = (CSquirrelSockets*)p;
		if ( pSocket )
		{
			pSocket->SetNewConnFunction( (char*)szFunc );

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQSocketFuncs::SetLostConnFunc )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer p = NULL;
		const SQChar* szFunc = 0;

		sq_getuserpointer( v, 2, &p );
		sq_getstring( v, 3, &szFunc );

		CSquirrelSockets* pSocket = (CSquirrelSockets*)p;
		if ( pSocket )
		{
			pSocket->SetLostConnFunction( (char*)szFunc );

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQSocketFuncs::Send )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs >= 3 )
	{
		SQUserPointer p = NULL;
		const SQChar* szData = 0;

		sq_getuserpointer( v, 2, &p );
		sq_getstring( v, 3, &szData );

		CSquirrelSockets* pSocket = (CSquirrelSockets*)p;
		if ( pSocket )
		{
			if ( iArgs == 4 )
			{
				SQInteger iConn = 0;
				sq_getinteger( v, 4, &iConn );
				pSocket->Send( (char*)szData, (unsigned char)iConn );
			}
			else pSocket->Send( (char*)szData );

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}
*/


//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Socket, Delete )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelSockets, Socket );

	CSquirrelSockets* pSocket = CSquirrelSocketManager::Find( self->GetID() );
	
	CSquirrel* pSquirrel = CSquirrelManager::Find( v );
	if ( pSquirrel )
	{
		if ( pSquirrel->GetObjects() ) pSquirrel->GetObjects()->RemoveSocket( pSocket );

		sa.Return( true );
		return 1;
	}

	sa.Return( false );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Socket, Connect )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelSockets, Socket );

	CSquirrelSockets* pSocket = CSquirrelSocketManager::Find( self->GetID() );

	const char* szHost = (const char*)sa.GetString( 2 );
	int iPort = sa.GetInt( 3 );

	pSocket->Connect( szHost, (unsigned short)iPort );
		
	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Socket, Start )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelSockets, Socket );

	CSquirrelSockets* pSocket = CSquirrelSocketManager::Find( self->GetID() );

	int iPort = sa.GetInt( 2 );
	int iMaxConns = sa.GetInt( 3 );

	pSocket->Start( (unsigned short)iPort, (unsigned short)iMaxConns );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Socket, Stop )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelSockets, Socket );

	CSquirrelSockets* pSocket = CSquirrelSocketManager::Find( self->GetID() );

	pSocket->Stop();

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Socket, Send )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelSockets, Socket );

	CSquirrelSockets* pSocket = CSquirrelSocketManager::Find( self->GetID() );

	char* szData = (char*)sa.GetString( 2 );

	pSocket->Send( szData );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Socket, SendClient )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelSockets, Socket );

	CSquirrelSockets* pSocket = CSquirrelSocketManager::Find( self->GetID() );

	char* szData = (char*)sa.GetString( 2 );
	int iConn = sa.GetInt( 3 );

	pSocket->Send( szData, (unsigned char)iConn );


	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Socket, SetNewConnFunc )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelSockets, Socket );

	CSquirrelSockets* pSocket = CSquirrelSocketManager::Find( self->GetID() );

	char* szFunc = (char*)sa.GetString( 2 );

	pSocket->SetNewConnFunction( szFunc );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------

_MEMBER_FUNCTION_IMPL( Socket, SetLostConnFunc )
{
	StackHandler sa(v);
	_CHECK_SELF( CSquirrelSockets, Socket );

	CSquirrelSockets* pSocket = CSquirrelSocketManager::Find( self->GetID() );

	char* szFunc = (char*)sa.GetString( 2 );

	pSocket->SetNewConnFunction( szFunc );

	sa.Return( true );
	return 1;
}

//--------------------------------------------------


_BEGIN_CLASS( Socket )

_MEMBER_FUNCTION( Socket, Delete, 1, "x" )
_MEMBER_FUNCTION( Socket, Connect, 3, "xsi" )
_MEMBER_FUNCTION( Socket, Start, 3, "xii" )
_MEMBER_FUNCTION( Socket, Stop, 1, "x" )
_MEMBER_FUNCTION( Socket, Send, 2, "xs" )
_MEMBER_FUNCTION( Socket, SendClient, 3, "xsi" )
_MEMBER_FUNCTION( Socket, SetNewConnFunc, 2, "xs" )
_MEMBER_FUNCTION( Socket, SetLostConnFunc, 2, "xs" )

_END_CLASS( Socket );