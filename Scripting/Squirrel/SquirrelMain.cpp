/*
SquirrelMain.cpp
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

#include "SquirrelMain.h"
#include "SquirrelFuncs.h"
#include "SquirrelConstants.h"
#include <stdarg.h>
#include <stdio.h>
#include "Console.h"

void Print( HSQUIRRELVM v, const SQChar *s, ... )
{
	char sz[ 512 ] = {0};
	va_list arglist;
	va_start( arglist, s );
	vsnprintf( sz, sizeof( sz ), s, arglist );
	va_end( arglist );

	CConsole::PrintC( "SCRIPT", COLOUR_CYAN, sz );
}

void Error( HSQUIRRELVM v, const SQChar *s, ... )
{
	va_list arglist;
	va_start(arglist, s);
	scvprintf(s, arglist);
	va_end(arglist);
}

CSquirrel::CSquirrel( unsigned char uc, const char* sz )
{
	m_ucID = uc;
	strncpy( m_szFilename, sz, 32 );

	m_pObjects = NULL;

	m_bWaiting = false;
	m_bExecuting = false;
	m_bContinueProcessing = true;
	m_ucParams = 0;
	m_iOrigStack = 0;

	/*SquirrelVM::Init();
	m_pVM = SquirrelVM::GetVMPtr();
	m_pVMSys = m_pVM;*/
	m_pVM = sq_open( 1024 ); // creates a VM with initial stack size 1024
	if ( m_pVM )
	{
		sq_setprintfunc( m_pVM, Print, Error );

		sqstd_seterrorhandlers( m_pVM );

		SquirrelVM::InitNoRef( m_pVM );
		m_pVMSys = m_pVM;
		CSquirrelFuncs::RegisterFuncs( m_pVM );
		CSquirrelConstants::RegisterConstants( m_pVM );

		m_pObjects = new CSquirrelObjects();

		sq_pushroottable( m_pVM );
		sqstd_dofile( m_pVM, _SC( sz ), 0, 1 );

		sqstd_register_iolib( m_pVM );
		sqstd_register_mathlib( m_pVM );
		sqstd_register_bloblib( m_pVM );
		// sqstd_register_systemlib( m_pVM ); // This could be dangerous!
		sqstd_register_stringlib( m_pVM );
	}
}

CSquirrel::~CSquirrel(void)
{
	if ( m_pVM )
	{
		PushFunction( "onScriptUnload" );
		CallFunction();

		SquirrelVM::SetVMSys( m_pVMSys );
		SquirrelVM::Shutdown();

		sq_pop( m_pVM, 1 );

		sq_close( m_pVM );

		m_pVM = NULL;
		m_pVMSys = NULL;
	}

	if ( m_pObjects )
	{
		delete m_pObjects;
		m_pObjects = NULL;
	}
}

void CSquirrel::PushFunction( const char* sz )
{
	if ( m_pVM )
	{
		if ( ( !m_bWaiting ) && ( !m_bExecuting ) )
		{
			m_iOrigStack = sq_gettop( m_pVM );
			sq_pushroottable( m_pVM );
			sq_pushstring( m_pVM,_SC( sz ), -1 );
			if( SQ_SUCCEEDED( sq_get( m_pVM, -2 ) ) )
			{
				sq_pushroottable( m_pVM );

				m_ucParams++;
				m_bWaiting = true;
			}
			else sq_pop( m_pVM, 1 );
		}
	}
}

void CSquirrel::PushInteger( int i )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			sq_pushinteger( m_pVM, i );
			m_ucParams++;
		}
	}
}

void CSquirrel::PushFloat( float f )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting )  && ( !m_bExecuting ) )
		{
			sq_pushfloat( m_pVM, f );
			m_ucParams++;
		}
	}
}

void CSquirrel::PushString( const char* sz )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting )  && ( !m_bExecuting ) )
		{
			if ( strlen( sz ) > 0 ) sq_pushstring( m_pVM, _SC( sz ), -1 );
			else sq_pushnull( m_pVM );

			m_ucParams++;
		}
	}
}

void CSquirrel::PushBool( bool b )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			sq_pushbool( m_pVM, b );
			m_ucParams++;
		}
	}
}

void CSquirrel::PushPointer( void* p )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			sq_pushuserpointer( m_pVM, (SQUserPointer)p );

			m_ucParams++;
		}
	}
}

void CSquirrel::PushPlayerPointer( CPlayerPool* p )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			SquirrelVM::SetVMSys( m_pVMSys );
			push_Player( p );

			m_ucParams++;
		}
	}
}

void CSquirrel::PushVehiclePointer( CVehiclePool* p )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			SquirrelVM::SetVMSys( m_pVMSys );
			push_Vehicle( p );

			m_ucParams++;
		}
	}
}

void CSquirrel::PushPickupPointer( CPickupPool* p )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			SquirrelVM::SetVMSys( m_pVMSys );
			push_Pickup( p );

			m_ucParams++;
		}
	}
}

void CSquirrel::PushSocketPointer( CSquirrelSockets* p )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			SquirrelVM::SetVMSys( m_pVMSys );
			push_Socket( p );

			m_ucParams++;
		}
	}
}

void CSquirrel::PushNull( void )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			sq_pushnull( m_pVM );

			m_ucParams++;
		}
	}
}

bool CSquirrel::CallFunction( void )
{
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			m_bExecuting = true;

			SquirrelVM::SetVMSys( m_pVMSys );
			sq_call( m_pVM, m_ucParams, 0, 0 );
			sq_settop( m_pVM, m_iOrigStack );

			m_ucParams = 0;
			m_bWaiting = false;

			m_bExecuting = false;

			if ( !m_bContinueProcessing ) 
			{
				m_bContinueProcessing = true;
				return false;
			}
		}
	}

	return true;
}

SquirrelRetn CSquirrel::CallFunctionAndRetn( void )
{
	SquirrelRetn result;

	result.pType = ST_NULL;
	if ( m_pVM )
	{
		if ( ( m_bWaiting ) && ( !m_bExecuting ) )
		{
			m_bExecuting = true;
			SquirrelVM::SetVMSys( m_pVMSys );

			sq_call( m_pVM, m_ucParams, SQTrue, 0 );

			SQBool b = SQFalse;
			SQUserPointer p = NULL;
			const SQChar* sz = NULL;

			// Seems squirrel pushes the return value to the 4th slot, wierd yes, but lets ask for it here
			switch( sq_gettype( m_pVM, 4 ) )
			{
			case OT_STRING:
				result.pType = ST_STRING;
				sq_getstring( m_pVM, 4, &sz );
				strncpy( result.pValue.szString, sz, 256 );
				break;

			case OT_INTEGER:
				result.pType = ST_INTEGER;
				sq_getinteger( m_pVM, 4, &result.pValue.i );
				break;
			case OT_FLOAT:
				result.pType = ST_FLOAT;
				sq_getfloat( m_pVM, 4, &result.pValue.f );
				break;

			case OT_BOOL:
				result.pType = ST_BOOL;
				sq_getbool( m_pVM, 4, &b );
				result.pValue.b = b;
				break;

			case OT_USERPOINTER:
				result.pType = ST_POINTER;
				sq_getuserpointer( m_pVM, 4, &result.pValue.p );
				break;

			case OT_NULL:
				result.pType = ST_NULL;
				break;

			case OT_INSTANCE:
				// Needs doing for players!
				if( !SQ_FAILED( sq_getinstanceup( m_pVM, 4, (SQUserPointer*)&p, (SQUserPointer)&__Player_decl ) ) )
				{
					// Push Player
					result.pType = ST_PLAYER;
					result.pValue.p = p;
				}
				else if( !SQ_FAILED( sq_getinstanceup( m_pVM, 4, (SQUserPointer*)&p, (SQUserPointer)&__Vehicle_decl ) ) )
				{
					// Push Vehicle
					result.pType = ST_VEHICLE;
					result.pValue.p = p;
				}
				else if( !SQ_FAILED( sq_getinstanceup( m_pVM, 4, (SQUserPointer*)&p, (SQUserPointer)&__Pickup_decl ) ) )
				{
					// Push Pickup
					result.pType = ST_PICKUP;
					result.pValue.p = p;
				}
				else if( !SQ_FAILED( sq_getinstanceup( m_pVM, 4, (SQUserPointer*)&p, (SQUserPointer)&__Socket_decl ) ) )
				{
					// Push Socket
					result.pType = ST_SOCKET;
					result.pValue.p = p;
				}
				else if( !SQ_FAILED( sq_getinstanceup( m_pVM, 4, (SQUserPointer*)&p, (SQUserPointer)&__Timer_decl ) ) )
				{
					// Push Timer
					result.pType = ST_TIMER;
					result.pValue.p = p;
				}
				break;

			default:
				result.pType = ST_NULL;
				break;

			}

			sq_settop( m_pVM, m_iOrigStack );

			m_ucParams = 0;
			m_bWaiting = false;
			m_bExecuting = false;
		}
	}

	return result;
}

// The Manager
unsigned char CSquirrelManager::m_ucScripts = 0;
CSquirrel* CSquirrelManager::m_Scripts[ MAX_SCRIPTS ] = { 0 };

CSquirrel* CSquirrelManager::New( const char* sz )
{
	unsigned char uc = FindFreeID();
	if ( uc < MAX_SCRIPTS )
	{
		CSquirrel* p = new CSquirrel( uc, sz );
		if ( p )
		{
			m_Scripts[ uc ] = p;
			m_ucScripts++;

			return p;
		}
	}

	return NULL;
}

CSquirrel* CSquirrelManager::New( unsigned char uc, const char* sz )
{
	if ( uc < MAX_SCRIPTS )
	{
		CSquirrel* p = new CSquirrel( uc, sz );
		if ( p )
		{
			m_Scripts[ uc ] = p;
			m_ucScripts++;

			return p;
		}
	}
	return NULL;
}

CSquirrel* CSquirrelManager::Find( unsigned char uc )
{
	if ( uc < MAX_SCRIPTS ) return m_Scripts[ uc ];

	return NULL;
}

CSquirrel* CSquirrelManager::Find( const char* sz )
{
	unsigned char uc = 0, uc1 = 0;
	CSquirrel* pScript = NULL;
	while ( ( uc < MAX_SCRIPTS ) && ( uc1 < m_ucScripts ) )
	{
		pScript = m_Scripts[ uc ];
		if ( pScript )
		{
			if ( !strcmp( pScript->GetScriptName(), sz ) ) return pScript;

			uc1++;
		}

		uc++;
	}

	return NULL;
}

CSquirrel* CSquirrelManager::Find( HSQUIRRELVM v )
{
	unsigned char uc = 0, uc1 = 0;
	CSquirrel* pScript = NULL;
	while ( ( uc < MAX_SCRIPTS ) && ( uc1 < m_ucScripts ) )
	{
		pScript = m_Scripts[ uc ];
		if ( pScript )
		{
			if ( pScript->GetVM() == v ) return pScript;

			uc1++;
		}

		uc++;
	}

	return NULL;
}

bool CSquirrelManager::Remove( CSquirrel* p )
{
	if ( p )
	{
		m_Scripts[ p->GetID() ] = NULL;
		delete p;

		m_ucScripts--;

		return true;
	}
	return false;
}

bool CSquirrelManager::Remove( unsigned char uc )
{
	CSquirrel* p = Find( uc );
	if ( p )
	{
		m_Scripts[ uc ] = NULL;
		delete p;

		m_ucScripts--;

		return true;
	}
	return false;
}

bool CSquirrelManager::Remove( const char* sz )
{
	CSquirrel* p = Find( sz );
	if ( p )
	{
		m_Scripts[ p->GetID() ] = NULL;
		delete p;

		m_ucScripts--;

		return true;
	}
	return false;
}

void CSquirrelManager::RemoveAll( void )
{
	unsigned char uc = 0, uc1 = 0;
	CSquirrel* pScript = NULL;
	while ( ( uc < MAX_SCRIPTS ) && ( uc1 < m_ucScripts ) )
	{
		pScript = m_Scripts[ uc ];
		if ( pScript )
		{
			m_Scripts[ uc ] = 0;
			delete pScript;

			uc1++;
		}

		uc++;
	}

	m_ucScripts = 0;
}

unsigned char CSquirrelManager::FindFreeID( void )
{
	for ( unsigned char uc = 0; uc < MAX_SCRIPTS; uc++ )
	{
		if ( !m_Scripts[ uc ] ) return uc;
	}

	return MAX_SCRIPTS;
}

// Objects Stuff
void CSquirrelManager::RemoveTimer( CSquirrelTimers* p )
{
	unsigned char uc = 0, uc1 = 0;
	CSquirrel* pScript = NULL;
	while ( ( uc < MAX_SCRIPTS ) && ( uc1 < m_ucScripts ) )
	{
		pScript = m_Scripts[ uc ];
		if ( pScript )
		{
			if ( pScript->GetObjects() )
			{
				if ( pScript->GetObjects()->RemoveTimer( p ) ) return;
			}

			uc1++;
		}

		uc++;
	}
}
