/*
	RconPool.cpp
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

	File Author: Knight2k9

	--------------------------------------------------------------------
*/

#include "RconPool.h"
#include "Utils.h"

CRconPool::CRconPool( unsigned char uc )
{
	m_ucID = uc;

	strcpy( m_szNick, "Unknown" );
	strcpy( m_szIP, "0.0.0.0" );

	m_ConnectionID = UNASSIGNED_PLAYER_ID;
}

CRconPool::~CRconPool(void)
{
	// Dont really have to do this... but its just to be safe
	m_ucID = INVALID_PLAYER_ID;

	strcpy( m_szNick, "Unknown" );
	strcpy( m_szIP, "0.0.0.0" );

	m_ConnectionID = UNASSIGNED_PLAYER_ID;
}

unsigned char CRconPoolManager::m_ucConns = 0;
CRconPool* CRconPoolManager::m_RconPool[ MAX_PLAYERS ] = { 0 };

// The Manager
CRconPool* CRconPoolManager::New( unsigned char uc )
{
	if ( uc < MAX_PLAYERS )
	{
		CRconPool* p = new CRconPool( uc );
		if ( p )
		{
			m_RconPool[ uc ] = p;
			m_ucConns++;
			return p;
		}
	}
	return NULL;
}

CRconPool* CRconPoolManager::Find( BYTE b )
{
	if ( b < MAX_PLAYERS ) return m_RconPool[ b ];

	return NULL;
}

CRconPool* CRconPoolManager::Find( const char* sz )
{
	unsigned char uc = 0, uc1 = 0;
	CRconPool* pRcon = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucConns ) )
	{
		pRcon = m_RconPool[ uc ];
		if ( pRcon )
		{
			if ( CUtils::IsIn( pRcon->GetNick(), sz ) ) return pRcon;

			uc1++;
		}

		uc++;
	}

	return NULL;
}

CRconPool* CRconPoolManager::FindExact( const char* sz )
{
	unsigned char uc = 0, uc1 = 0;
	CRconPool* pRcon = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucConns ) )
	{
		pRcon = m_RconPool[ uc ];
		if ( pRcon )
		{
			if ( !strcmp( pRcon->GetNick(), sz ) ) return pRcon;

			uc1++;
		}
		uc++;
	}

	return NULL;
}

CRconPool* CRconPoolManager::Find( PlayerID id )
{
	unsigned char uc = 0, uc1 = 0;
	CRconPool* pRcon = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucConns ) )
	{
		pRcon = m_RconPool[ uc ];
		if ( pRcon )
		{
			if ( pRcon->GetConnectionID() == id ) return pRcon;

			uc1++;
		}
		uc++;
	}

	return NULL;
}

bool CRconPoolManager::Remove( CRconPool *p )
{
	if ( p )
	{
		m_RconPool[ p->GetID() ] = NULL;
		delete p;

		m_ucConns--;

		return true;
	}
	return false;
}

bool CRconPoolManager::Remove( unsigned char uc )
{
	CRconPool* p = Find( uc );
	if ( p )
	{
		m_RconPool[ uc ] = NULL;
		delete p;

		m_ucConns--;

		return true;
	}
	return false;
}

bool CRconPoolManager::Remove( const char* sz )
{
	CRconPool* p = Find( sz );
	if ( p )
	{
		m_RconPool[ p->GetID() ] = NULL;
		delete p;

		m_ucConns--;

		return true;
	}
	return false;
}

bool CRconPoolManager::Remove( PlayerID id )
{
	CRconPool* p = Find( id );
	if ( p )
	{
		m_RconPool[ p->GetID() ] = NULL;
		delete p;

		m_ucConns--;

		return true;
	}
	return false;
}

void CRconPoolManager::RemoveAll()
{
	unsigned char uc = 0, uc1 = 0;
	CRconPool* pRcon = NULL;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < m_ucConns ) )
	{
		pRcon = m_RconPool[ uc ];
		if ( pRcon )
		{
			m_RconPool[ uc ] = 0;
			delete pRcon;

			uc1++;
		}
		uc++;
	}

	m_ucConns = 0;
}