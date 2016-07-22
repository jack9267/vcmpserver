/*
	Scripting.cpp
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

#include "Scripting.h"

ScriptCall* CScripting::m_pScriptCall = 0;

void CScripting::PushFunction( const char* sz )
{
	/*unsigned char uc = 0;

	CSquirrel* pSquirrel = NULL;
	for ( uc = 0; uc < CSquirrelManager::Count(); uc++ )
	{
		pSquirrel = CSquirrelManager::Find( uc );
		if ( pSquirrel ) pSquirrel->PushFunction( sz );
	}*/

	// ^ that up there (yes that) can be very bad... think about it, if you have 100 scripts loaded and call a function that takes 10 arguments, 
	// we'd be looping a thousand times or more to call the script! Lets do it the new way (which also fixed CallFunc)

	if ( m_pScriptCall )
	{
		delete m_pScriptCall;
		m_pScriptCall = 0;
	}

	m_pScriptCall = new ScriptCall;
	strncpy( m_pScriptCall->szFunction, sz, 64 );
	m_pScriptCall->uiParams = 0;
}

void CScripting::PushInteger( int i )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_INTEGER;
		m_pScriptCall->pParams[ uiParams ].i = i;

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

void CScripting::PushFloat( float f )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_FLOAT;
		m_pScriptCall->pParams[ uiParams ].f = f;

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

void CScripting::PushString( const char* sz )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_STRING;
		strncpy( m_pScriptCall->pParams[ uiParams ].szString, sz, 256 );

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

void CScripting::PushBool( bool b )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_BOOL;
		m_pScriptCall->pParams[ uiParams ].b = b;

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

void CScripting::PushPointer( void* p )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_POINTER;
		m_pScriptCall->pParams[ uiParams ].p = p;

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

void CScripting::PushPlayerPointer( CPlayerPool* p )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_PLAYER;
		m_pScriptCall->pParams[ uiParams ].p = p;

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

void CScripting::PushVehiclePointer( CVehiclePool* p )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_VEHICLE;
		m_pScriptCall->pParams[ uiParams ].p = p;

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

void CScripting::PushPickupPointer( CPickupPool* p )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_PICKUP;
		m_pScriptCall->pParams[ uiParams ].p = p;

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

void CScripting::PushNull( void )
{
	if ( m_pScriptCall )
	{
		unsigned int uiParams = m_pScriptCall->uiParams;
		m_pScriptCall->pType[ uiParams ] = ST_NULL;

		m_pScriptCall->uiParams = (uiParams + 1);
	}
}

bool CScripting::CallFunction()
{
	unsigned char uc = 0;

	CSquirrel* pSquirrel = NULL;
	for ( uc = 0; uc < CSquirrelManager::Count(); uc++ )
	{
		pSquirrel = CSquirrelManager::Find( uc );
		if ( pSquirrel )
		{
			if ( m_pScriptCall )
			{
				pSquirrel->PushFunction( m_pScriptCall->szFunction );

				for ( unsigned int ui = 0; ui < m_pScriptCall->uiParams; ui++ )
				{
					switch ( m_pScriptCall->pType[ ui ] )
					{
					case ST_STRING:
						pSquirrel->PushString( m_pScriptCall->pParams[ ui ].szString );
						break;

					case ST_INTEGER:
						pSquirrel->PushInteger( m_pScriptCall->pParams[ ui ].i );
						break;

					case ST_FLOAT:
						pSquirrel->PushFloat( m_pScriptCall->pParams[ ui ].f );
						break;

					case ST_BOOL:
						pSquirrel->PushBool( m_pScriptCall->pParams[ ui ].b );
						break;

					case ST_POINTER:
						pSquirrel->PushPointer( m_pScriptCall->pParams[ ui ].p );
						break;

					case ST_PLAYER:
						pSquirrel->PushPlayerPointer( (CPlayerPool*)m_pScriptCall->pParams[ ui ].p );
						break;

					case ST_VEHICLE:
						pSquirrel->PushVehiclePointer( (CVehiclePool*)m_pScriptCall->pParams[ ui ].p );
						break;

					case ST_PICKUP:
						pSquirrel->PushPickupPointer( (CPickupPool*)m_pScriptCall->pParams[ ui ].p );
						break;

					case ST_NULL:
						pSquirrel->PushNull();
						break;
					}
				}

				if ( !pSquirrel->CallFunction() ) 
				{
					if ( m_pScriptCall )
					{
						delete m_pScriptCall;
						m_pScriptCall = 0;
					}
					return false;
				}
			}
		}
	}

	if ( m_pScriptCall )
	{
		delete m_pScriptCall;
		m_pScriptCall = 0;
	}

	return true;
}