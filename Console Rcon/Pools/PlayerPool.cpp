#include "PlayerPool.h"

CPlayerPool::CPlayerPool( unsigned char uc )
{
	m_ucID = uc;

	strcpy( m_szNick, "" );
}

CPlayerPool::~CPlayerPool(void)
{
}

unsigned char CPlayerPoolManager::m_ucPlayers = 0;
std::vector< CPlayerPool* > CPlayerPoolManager::m_PlayerPool( MAX_PLAYERS );

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
			//if ( CUtils::IsIn( pPlayer->GetNick(), sz ) ) return pPlayer;

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