/*
	SquirrelSockets.cpp
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

#include "SquirrelSockets.h"
#include "Scripting.h"
#include "TCPInterface.h"

CSquirrelSockets::CSquirrelSockets( unsigned char uc )
{
	m_ucID = uc;

	m_bStarted = false;
	m_bServer = false;
	m_pInterface = new TCPInterface();
	m_pConnMgr = new CSquirrelSocketConnectionsMgr();
	m_playerId = UNASSIGNED_PLAYER_ID;

	m_iParentScript = -1;

	strcpy( m_szFunction, "" );
	strcpy( m_szNewConnFunc, "" );
	strcpy( m_szLostConnFunc, "" );
}

CSquirrelSockets::~CSquirrelSockets(void)
{
	Stop();

	if ( m_pInterface )
	{
		delete m_pInterface;
		m_pInterface = NULL;
	}
}

bool CSquirrelSockets::Connect( const char* szHost, unsigned short usPort )
{
	if ( m_pInterface )
	{
		if ( !m_bStarted )
		{
			m_pInterface->Start( 0, 0 );

			m_playerId = m_pInterface->Connect( szHost, usPort, false );

			if ( m_playerId != UNASSIGNED_PLAYER_ID ) 
			{
				m_bServer = false;

				return true;
			}
		}
	}

	return false;
}

bool CSquirrelSockets::Start( unsigned short usPort, unsigned short usMaxConns )
{
	if ( m_pInterface )
	{
		if ( !m_bStarted )
		{
			if ( m_pInterface->Start( usPort, usMaxConns ) )
			{
				m_bStarted = true;
				m_bServer = true;

				return true;
			}
		}
	}

	return false;
}

void CSquirrelSockets::Stop( void )
{
	if ( m_pInterface )
	{
		m_pInterface->Stop();

		m_bStarted = false;
		m_bServer = false;
	}
}

void CSquirrelSockets::Send( char* sz, unsigned char ucConn )
{
	if ( m_pInterface )
	{
		if ( m_bServer )
		{
			if ( ucConn == 255 )
			{
				unsigned char uc = 0, uc1 = 0;
				while ( ( uc < 128 ) && ( uc1 < m_pConnMgr->Count() ) )
				{
					PlayerID pID = m_pConnMgr->Find( uc );
					if ( pID != UNASSIGNED_PLAYER_ID )
					{
						m_pInterface->Send( sz, strlen( sz ), pID );
						uc1++;
					}
					uc++;
				}
			}
			else
			{
				PlayerID pID = m_pConnMgr->Find( ucConn );
				if ( pID != UNASSIGNED_PLAYER_ID ) m_pInterface->Send( sz, strlen( sz ), pID );
			}
		}
		else
		{
			if ( m_playerId != UNASSIGNED_PLAYER_ID ) m_pInterface->Send( sz, strlen( sz ), m_playerId );
		}
	}
}

void CSquirrelSockets::Process( void )
{
	if ( m_pInterface )
	{
		if ( strlen( m_szFunction ) > 0 )
		{
			CSquirrel* pScript = CSquirrelManager::Find( m_iParentScript );

			Packet *p = m_pInterface->Receive();

			PlayerID player = m_pInterface->HasCompletedConnectionAttempt();
			if ( ( player != UNASSIGNED_PLAYER_ID )  )
			{
				m_bStarted = true;
				m_playerId = player;

				if ( strlen( m_szNewConnFunc ) > 0 )
				{
					CSquirrel* pScript = CSquirrelManager::Find( m_iParentScript );
					if ( pScript )
					{
						pScript->PushFunction( m_szNewConnFunc );
						pScript->CallFunction();
					}
				}
			}

			player = m_pInterface->HasFailedConnectionAttempt();
			if ( ( player != UNASSIGNED_PLAYER_ID )  )
			{
				m_bStarted = false;

				if ( strlen( m_szLostConnFunc ) > 0 )
				{
					CSquirrel* pScript = CSquirrelManager::Find( m_iParentScript );
					if ( pScript )
					{
						pScript->PushFunction( m_szLostConnFunc );
						pScript->CallFunction();
					}
				}
			}

			player = m_pInterface->HasNewConnection();
			if ( player != UNASSIGNED_PLAYER_ID ) 
			{
				unsigned char uc = m_pConnMgr->New( player );
				if ( strlen( m_szNewConnFunc ) > 0 )
				{
					if ( pScript )
					{
						pScript->PushFunction( m_szNewConnFunc );
						pScript->PushInteger( uc );
						pScript->PushString( (char*)player.ToString( false ) );
						pScript->PushInteger( player.port );
						pScript->CallFunction();
					}
				}
			}

			player = m_pInterface->HasLostConnection();
			if ( player != UNASSIGNED_PLAYER_ID ) 
			{
				unsigned char uc = m_pConnMgr->Find( player );
				if ( uc != 255 )
				{
					if ( strlen( m_szNewConnFunc ) > 0 )
					{
						if ( pScript )
						{
							pScript->PushFunction( m_szLostConnFunc );
							pScript->PushInteger( uc );
							pScript->PushString( (char*)player.ToString( false ) );
							pScript->PushInteger( player.port );
							pScript->CallFunction();
						}
					}

					m_pConnMgr->Remove( player );
				}
			}

			if ( p )
			{
				if ( pScript )
				{
					if ( m_bServer )
					{
						unsigned char uc = m_pConnMgr->Find( p->playerId );

						pScript->PushFunction( m_szFunction );
						pScript->PushInteger( uc );
						pScript->PushString( (char*)p->data );
						pScript->CallFunction();
					}
					else
					{
						pScript->PushFunction( m_szFunction );
						pScript->PushString( (char*)p->data );
						pScript->CallFunction();
					}
				}

				m_pInterface->DeallocatePacket( p );
			}
		}
	}
}

// The Manager
unsigned char CSquirrelSocketManager::m_ucSockets = 0;
CSquirrelSockets* CSquirrelSocketManager::m_Sockets[ MAX_SOCKETS ] = { 0 };

CSquirrelSockets* CSquirrelSocketManager::New( void )
{
	unsigned int uiID = FindFreeID();
	if ( uiID < MAX_SOCKETS )
	{
		CSquirrelSockets* pTimer = new CSquirrelSockets( uiID );
		if ( pTimer )
		{
			m_Sockets[ uiID ] = pTimer;
			m_ucSockets++;
			return pTimer;
		}
	}
	
	return 0;
}

CSquirrelSockets* CSquirrelSocketManager::New( unsigned char ucID )
{
	if ( ucID < MAX_SOCKETS )
	{
		CSquirrelSockets* pTimer = new CSquirrelSockets( ucID );
		if ( pTimer )
		{
			m_Sockets[ ucID ] = pTimer;
			m_ucSockets++;
			return pTimer;
		}
	}
	
	return 0;
}

CSquirrelSockets* CSquirrelSocketManager::Find( unsigned char uc )
{
	if ( uc < MAX_SOCKETS ) return m_Sockets[ uc ];
	return 0;
}

bool CSquirrelSocketManager::Remove( CSquirrelSockets* p )
{
	if ( p )
	{
		if ( p->GetID() < MAX_SOCKETS )
		{
			m_Sockets[ p->GetID() ] = 0;
			m_ucSockets--;

			delete p;

			return true;
		}
	}

	return false;
}

bool CSquirrelSocketManager::Remove( unsigned char uc )
{
	CSquirrelSockets* p = Find( uc );
	if ( p )
	{
		m_Sockets[ uc ] = 0;
		m_ucSockets--;
		delete p;

		return true;
	}

	return false;
}

void CSquirrelSocketManager::RemoveAll( void )
{
	unsigned int uc = 0, uc1 = 0;
	while ( ( uc < MAX_SOCKETS ) && ( uc1 < m_ucSockets ) )
	{
		if ( m_Sockets[ uc ] )
		{
			delete m_Sockets[ uc ];

			m_Sockets[ uc ] = 0;

			uc1++;
		}
		uc++;
	}

	m_ucSockets = 0;
}

unsigned char CSquirrelSocketManager::FindFreeID( void )
{
	for ( unsigned char uc = 0; uc < MAX_SOCKETS; uc++ )
	{
		if ( !m_Sockets[ uc ] ) return uc;
	}

	return MAX_SOCKETS;
}

void CSquirrelSocketManager::ProcessSockets( void )
{
	unsigned char uc = 0, uc1 = 0;
	while ( ( uc < MAX_SOCKETS ) && ( uc1 < m_ucSockets ) )
	{
		if ( m_Sockets[ uc ] )
		{
			m_Sockets[ uc ]->Process();

			uc1++;
		}
		uc++;
	}
}

// The Connection Manager
CSquirrelSocketConnectionsMgr::CSquirrelSocketConnectionsMgr()
{
	for ( unsigned char uc = 0; uc < 128; uc++ )
	{
		m_Connections[ uc ] = UNASSIGNED_PLAYER_ID;
	}
}

unsigned char CSquirrelSocketConnectionsMgr::New( PlayerID pID )
{
	unsigned char ucID = FindFreeID();
	if ( ucID < 128 )
	{
		m_Connections[ ucID ] = pID;
		m_ucConnections++;
		return ucID;
	}
	
	return 255;
}

unsigned char CSquirrelSocketConnectionsMgr::Find( PlayerID pID )
{
	for ( unsigned char uc = 0; uc < 128; uc++ )
	{
		if ( m_Connections[ uc ] == pID ) return uc;
	}

	return 255;
}

PlayerID CSquirrelSocketConnectionsMgr::Find( unsigned char uc )
{
	if ( uc < 128 ) return m_Connections[ uc ];
	return UNASSIGNED_PLAYER_ID;
}

bool CSquirrelSocketConnectionsMgr::Remove( PlayerID pID )
{
	if ( pID != UNASSIGNED_PLAYER_ID )
	{
		unsigned char uc = Find( pID );
		if ( uc != 255 )
		{
			m_Connections[ uc ] = UNASSIGNED_PLAYER_ID;
			m_ucConnections--;

			return true;
		}
	}

	return false;
}

bool CSquirrelSocketConnectionsMgr::Remove( unsigned char uc )
{
	if ( uc != 255 )
	{
		m_Connections[ uc ] = UNASSIGNED_PLAYER_ID;
		m_ucConnections--;

		return true;
	}

	return false;
}

void CSquirrelSocketConnectionsMgr::RemoveAll( void )
{
	unsigned int uc = 0, uc1 = 0;
	while ( ( uc < 128 ) && ( uc1 < m_ucConnections ) )
	{
		if ( m_Connections[ uc ] != UNASSIGNED_PLAYER_ID )
		{
			m_Connections[ uc ] = UNASSIGNED_PLAYER_ID;

			uc1++;
		}
		uc++;
	}

	m_ucConnections = 0;
}

unsigned char CSquirrelSocketConnectionsMgr::FindFreeID( void )
{
	for ( unsigned char uc = 0; uc < 128; uc++ )
	{
		if ( m_Connections[ uc ] == UNASSIGNED_PLAYER_ID ) return uc;
	}

	return 128;
}