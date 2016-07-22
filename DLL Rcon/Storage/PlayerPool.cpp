#include "PlayerPool.h"
#include "Utils.h"

CPlayerPool::CPlayerPool( unsigned char uc )
{
	m_ucID = uc;

	strcpy( m_szNick, "Unknown" );
	strcpy( m_szIP, "0.0.0.0" );

	m_PlayerID = UNASSIGNED_PLAYER_ID;

	m_ucHealth = 0;
	m_ucArmour = 0;

	m_ucClass = 0;

	m_iScore = 0;

	m_ucTeam = 0;
	m_ucSkin = 0;

	m_ucAction = 0;
	m_ucWeapon = 0;

	m_bAdmin = false;
	m_bFrozen = false;
	m_bMuted = false;
	m_bSpectating = false;

	m_iCash = 0;

	m_iVehicleID = -1;
	m_ucPassenger = 0;

	for ( unsigned char uc = 0; uc < MAX_PLAYERS; uc++ )
	{
		m_bIgnored[ uc ] = false;
	}
}

CPlayerPool::~CPlayerPool(void)
{
	unsigned char uc = 0, uc1 = 0;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			if ( pPlayer->GetIgnored( m_ucID ) ) pPlayer->SetIgnored( m_ucID, false );

			uc1++;
		}

		uc++;
	}

	m_ucID = INVALID_PLAYER_ID;

	strcpy( m_szNick, "Unknown" );
	strcpy( m_szIP, "0.0.0.0" );

	m_PlayerID = UNASSIGNED_PLAYER_ID;
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

CPlayerPool* CPlayerPoolManager::Find( char* sz )
{
	unsigned char uc = 0, uc1 = 0;
	CPlayerPool* pPlayer = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucPlayers ) )
	{
		pPlayer = m_PlayerPool[ uc ];
		if ( pPlayer )
		{
			if ( Utils::IsIn( pPlayer->GetNick(), sz ) ) return pPlayer;

			uc1++;
		}

		uc++;
	}

	return NULL;
}

CPlayerPool* CPlayerPoolManager::FindExact( char* sz )
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

bool CPlayerPoolManager::Remove( char* sz )
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