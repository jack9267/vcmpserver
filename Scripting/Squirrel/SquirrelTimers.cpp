/*
	SquirrelTimers.cpp
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

#include "SquirrelTimers.h"
#include "GetTime.h"
#include "Scripting.h"

CSquirrelTimers::CSquirrelTimers( unsigned int ui )
{
	m_uiID = ui;

	m_uiTime = 0;
	m_uiInterval = 1000;
	m_uiTimesToLoop = 1;
	m_uiLooped = 0;

	m_bStarted = false;

	strncpy( m_szFunction, "", 128 );

	m_iParentScript = -1;

	m_pParams.resize( MAX_TIMER_PARAMS );
	m_pTypes.resize( MAX_TIMER_PARAMS );
	m_iParams = 0;
}

CSquirrelTimers::~CSquirrelTimers(void)
{
	Stop();

	m_pParams.clear();
	m_pTypes.clear();

	m_iParams = 0;
}

void CSquirrelTimers::SetInterval( unsigned int ui )
{
	m_uiInterval = ui;

	m_uiTime = RakNet::GetTime();
}

void CSquirrelTimers::PushInteger( int i )
{
	if ( m_iParams < MAX_TIMER_PARAMS )
	{
		m_pParams[ m_iParams ].i = i;
		m_pTypes[ m_iParams ] = eInt;
		m_iParams++;
	}
}

void CSquirrelTimers::PushFloat( float f )
{
	if ( m_iParams < MAX_TIMER_PARAMS )
	{
		m_pParams[ m_iParams ].f = f;
		m_pTypes[ m_iParams ] = eFloat;
		m_iParams++;
	}
}

void CSquirrelTimers::PushUserData( void* p )
{
	if ( m_iParams < MAX_TIMER_PARAMS )
	{
		m_pParams[ m_iParams ].p = p;
		m_pTypes[ m_iParams ] = eUserData;
		m_iParams++;
	}
}

void CSquirrelTimers::PushString( char* sz )
{
	if ( m_iParams < MAX_TIMER_PARAMS )
	{
		strncpy( m_pParams[ m_iParams ].sz, sz, 256 );
		m_pTypes[ m_iParams ] = eString;
		m_iParams++;
	}
}

void CSquirrelTimers::PushPlayerPointer( CPlayerPool* p )
{
	if ( m_iParams < MAX_TIMER_PARAMS )
	{
		m_pParams[ m_iParams ].p = p;
		m_pTypes[ m_iParams ] = ePlayer;
		m_iParams++;
	}
}

void CSquirrelTimers::PushVehiclePointer( CVehiclePool* p )
{
	if ( m_iParams < MAX_TIMER_PARAMS )
	{
		m_pParams[ m_iParams ].p = p;
		m_pTypes[ m_iParams ] = eVehicle;
		m_iParams++;
	}
}

void CSquirrelTimers::PushPickupPointer( CPickupPool* p )
{
	if ( m_iParams < MAX_TIMER_PARAMS )
	{
		m_pParams[ m_iParams ].p = p;
		m_pTypes[ m_iParams ] = ePickup;
		m_iParams++;
	}
}

void CSquirrelTimers::PushSocketPointer( CSquirrelSockets* p )
{
	if ( m_iParams < MAX_TIMER_PARAMS )
	{
		m_pParams[ m_iParams ].p = p;
		m_pTypes[ m_iParams ] = eSocket;
		m_iParams++;
	}
}

void CSquirrelTimers::Process( void )
{
	if ( m_bStarted )
	{
		if ( RakNet::GetTime() >= ( m_uiTime + m_uiInterval ) )
		{
			if ( m_iParentScript != -1 )
			{
				// Fire the timer! ***BOOM!***
				if ( strlen( m_szFunction ) > 0 )
				{
					CSquirrel* pScript = CSquirrelManager::Find( m_iParentScript );
					if ( pScript )
					{
						pScript->PushFunction( m_szFunction );

						for ( unsigned char uc = 0; uc < m_iParams; uc++ )
						{
							if ( m_pTypes[ uc ] != eNone ) 
							{
								switch ( m_pTypes[ uc ] )
								{
								case eInt:
									pScript->PushInteger( m_pParams[ uc ].i );
									break;

								case eFloat:
									pScript->PushFloat( m_pParams[ uc ].f );
									break;

								case eUserData:
									pScript->PushPointer( m_pParams[ uc ].p );
									break;

								case eString:
									pScript->PushString( m_pParams[ uc ].sz );
									break;

								case ePlayer:
									pScript->PushPlayerPointer( (CPlayerPool*)m_pParams[ uc ].p );
									break;

								case eVehicle:
									pScript->PushVehiclePointer( (CVehiclePool*)m_pParams[ uc ].p );
									break;

								case ePickup:
									pScript->PushPickupPointer( (CPickupPool*)m_pParams[ uc ].p );
									break;

								case eSocket:
									pScript->PushSocketPointer( (CSquirrelSockets*)m_pParams[ uc ].p );
									break;
								}
							}
						}

						pScript->CallFunction();
					}
				}
			}

			if ( m_uiTimesToLoop != 0 )
			{
				m_uiLooped++;
				if ( m_uiLooped >= m_uiTimesToLoop )
				{
					// Kill ze timer!
					m_bStarted = false;

					// Delete the timer
					CSquirrelManager::RemoveTimer( this );
				}
				else m_uiTime = RakNet::GetTime();
			}
			else m_uiTime = RakNet::GetTime();
		}
	}
}

// The Manager
unsigned int CSquirrelTimerManager::m_uiTimers = 0;
CSquirrelTimers* CSquirrelTimerManager::m_Timers[ MAX_TIMERS ] = { 0 };

CSquirrelTimers* CSquirrelTimerManager::New( void )
{
	unsigned int uiID = FindFreeID();
	if ( uiID < MAX_TIMERS )
	{
		CSquirrelTimers* pTimer = new CSquirrelTimers( uiID );
		if ( pTimer )
		{
			m_Timers[ uiID ] = pTimer;
			m_uiTimers++;
			return pTimer;
		}
	}
	
	return 0;
}

CSquirrelTimers* CSquirrelTimerManager::New( unsigned int uiID )
{
	if ( uiID < MAX_TIMERS )
	{
		CSquirrelTimers* pTimer = new CSquirrelTimers( uiID );
		if ( pTimer )
		{
			m_Timers[ uiID ] = pTimer;
			m_uiTimers++;
			return pTimer;
		}
	}
	
	return 0;
}

CSquirrelTimers* CSquirrelTimerManager::Find( unsigned int ui )
{
	if ( ui < MAX_TIMERS ) return m_Timers[ ui ];
	return 0;
}

bool CSquirrelTimerManager::Remove( CSquirrelTimers* p )
{
	if ( p )
	{
		if ( p->GetID() < MAX_TIMERS )
		{
			m_Timers[ p->GetID() ] = 0;
			m_uiTimers--;

			delete p;

			return true;
		}
	}

	return false;
}

bool CSquirrelTimerManager::Remove( unsigned int ui )
{
	CSquirrelTimers* p = Find( ui );
	if ( p )
	{
		m_Timers[ ui ] = 0;
		m_uiTimers--;
		delete p;

		return true;
	}

	return false;
}

void CSquirrelTimerManager::RemoveAll( void )
{
	unsigned int ui = 0, ui1 = 0;
	while ( ( ui < MAX_TIMERS ) && ( ui1 < m_uiTimers ) )
	{
		if ( m_Timers[ ui ] )
		{
			delete m_Timers[ ui ];

			m_Timers[ ui ] = 0;

			ui1++;
		}
		ui++;
	}

	m_uiTimers = 0;
}

unsigned int CSquirrelTimerManager::FindFreeID( void )
{
	for ( unsigned int ui = 0; ui < MAX_TIMERS; ui++ )
	{
		if ( !m_Timers[ ui ] ) return ui;
	}

	return MAX_TIMERS;
}

void CSquirrelTimerManager::ProcessTimers( void )
{
	unsigned int ui = 0, ui1 = 0;
	while ( ( ui < MAX_TIMERS ) && ( ui1 < m_uiTimers ) )
	{
		if ( m_Timers[ ui ] )
		{
			m_Timers[ ui ]->Process();

			ui1++;
		}
		ui++;
	}
}