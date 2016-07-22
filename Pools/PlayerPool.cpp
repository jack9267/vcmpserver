/*
	PlayerPool.cpp
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

#include "PlayerPool.h"
#include "Utils.h"
#include "SpawnPool.h"

CPlayerPool::CPlayerPool( unsigned char uc )
{
	m_ucID = uc;

	strcpy( m_szNick, "Unknown" );
	strcpy( m_szIP, "0.0.0.0" );

	m_PlayerID = UNASSIGNED_PLAYER_ID;

	m_sPing = 0;

	m_ucHealth = 0;
	m_ucArmour = 0;

	m_ucClass = 0;

	m_iScore = 0;

	m_ucAction = 0;
	m_ucWeapon = 0;

	m_wKeys = 0;

	m_bAdmin = false;
	m_bFrozen = true; // Bad name!
	m_bMuted = false;
	m_bSyncBlocked = false;
	m_ucSpectating = INVALID_PLAYER_ID;

	m_iCash = 0;

	m_iVehicleID = -1;
	m_uiEnterVehicleTime = 0;
	m_ucPassenger = 0;
	m_uiSeat = 255;

	for ( unsigned char uc = 0; uc < MAX_PLAYERS; uc++ )
	{
		m_bIgnored[ uc ] = false;
		m_bSyncBlockedTo[ uc ] = false;
		m_ulLastSyncTime[ uc ] = 0;
	}
}

CPlayerPool::~CPlayerPool(void)
{
	// Make sure that future players with the same ID arn't ignored by other players
	unsigned char uc = 0, uc1 = 0;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			if ( pPlayer->GetIgnored( m_ucID ) ) pPlayer->SetIgnored( m_ucID, false );
			if ( pPlayer->GetSyncBlockedTo( m_ucID ) ) pPlayer->SetSyncBlockedTo( m_ucID, false );

			uc1++;
		}

		uc++;
	}

	// Dont really have to do this... but its just to be safe
	m_ucID = INVALID_PLAYER_ID;

	strcpy( m_szNick, "Unknown" );
	strcpy( m_szIP, "0.0.0.0" );

	m_PlayerID = UNASSIGNED_PLAYER_ID;
}

unsigned char CPlayerPool::GetTeam( void )
{
	CSpawnPool* pSpawn = CSpawnPoolManager::Find( m_ucClass );
	if ( pSpawn )
	{
		return pSpawn->GetTeam();
	}

	return INVALID_TEAM_ID;
}

unsigned char CPlayerPoolManager::m_ucPlayers = 0;
CPlayerPool* CPlayerPoolManager::m_PlayerPool[ MAX_PLAYERS ] = { 0 };

// The Manager
CPlayerPool* CPlayerPoolManager::New( unsigned char uc )
{
	if ( uc < MAX_PLAYERS )
	{
		CPlayerPool* p = new CPlayerPool( uc );
		if ( p )
		{
			m_PlayerPool[ uc ] = p;
			m_ucPlayers++;
			return p;
		}
	}
	return NULL;
}

CPlayerPool* CPlayerPoolManager::Find( BYTE b )
{
	if ( b < MAX_PLAYERS ) return m_PlayerPool[ b ];

	return NULL;
}

CPlayerPool* CPlayerPoolManager::Find( const char* sz )
{
	unsigned char uc = 0, uc1 = 0;
	CPlayerPool* pPlayer = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucPlayers ) )
	{
		pPlayer = m_PlayerPool[ uc ];
		if ( pPlayer )
		{
			if ( CUtils::IsIn( pPlayer->GetNick(), sz ) ) return pPlayer;

			uc1++;
		}

		uc++;
	}

	return NULL;
}

CPlayerPool* CPlayerPoolManager::FindExact( const char* sz )
{
	unsigned char uc = 0, uc1 = 0;
	CPlayerPool* pPlayer = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucPlayers ) )
	{
		pPlayer = m_PlayerPool[ uc ];
		if ( pPlayer )
		{
			if ( !strcmp( pPlayer->GetNick(), sz ) ) return pPlayer;

			uc1++;
		}
		uc++;
	}

	return NULL;
}

CPlayerPool* CPlayerPoolManager::Find( PlayerID id )
{
	unsigned char uc = 0, uc1 = 0;
	CPlayerPool* pPlayer = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucPlayers ) )
	{
		pPlayer = m_PlayerPool[ uc ];
		if ( pPlayer )
		{
			if ( pPlayer->GetPlayerID() == id ) return pPlayer;

			uc1++;
		}
		uc++;
	}

	return NULL;
}

bool CPlayerPoolManager::Remove( CPlayerPool *p )
{
	if ( p )
	{
		m_PlayerPool[ p->GetID() ] = NULL;
		delete p;

		m_ucPlayers--;

		return true;
	}
	return false;
}

bool CPlayerPoolManager::Remove( unsigned char uc )
{
	CPlayerPool* p = Find( uc );
	if ( p )
	{
		m_PlayerPool[ uc ] = NULL;
		delete p;

		m_ucPlayers--;

		return true;
	}
	return false;
}

bool CPlayerPoolManager::Remove( const char* sz )
{
	CPlayerPool* p = Find( sz );
	if ( p )
	{
		m_PlayerPool[ p->GetID() ] = NULL;
		delete p;

		m_ucPlayers--;

		return true;
	}
	return false;
}

bool CPlayerPoolManager::Remove( PlayerID id )
{
	CPlayerPool* p = Find( id );
	if ( p )
	{
		m_PlayerPool[ p->GetID() ] = NULL;
		delete p;

		m_ucPlayers--;

		return true;
	}
	return false;
}

void CPlayerPoolManager::RemoveAll()
{
	unsigned char uc = 0, uc1 = 0;
	CPlayerPool* pPlayer = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucPlayers ) )
	{
		pPlayer = m_PlayerPool[ uc ];
		if ( pPlayer )
		{
			m_PlayerPool[ uc ] = 0;
			delete pPlayer;

			uc1++;
		}
		uc++;
	}

	m_ucPlayers = 0;
}