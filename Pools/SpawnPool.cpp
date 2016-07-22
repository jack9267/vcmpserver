/*
	SpawnPool.cpp
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

#include "SpawnPool.h"

CSpawnPool::CSpawnPool( unsigned char uc )
{
	m_ucID = uc;

	m_ucSkin = 0;
	m_ucTeam = 0;

	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;

	m_fAngle = 0.0f;

	for ( unsigned char uc = 0; uc < 3; uc++ )
	{
		m_iWeapons[ uc ] = 0;
		m_iWeaponAmmo[ uc ] = 0;
	}
}

CSpawnPool::~CSpawnPool(void)
{
}


unsigned char CSpawnPoolManager::m_ucSpawns = 0;
CSpawnPool* CSpawnPoolManager::m_SpawnPool[ MAX_SPAWNS ] = { 0 };

// The Manager
CSpawnPool* CSpawnPoolManager::New( unsigned char uc )
{
	if ( uc < MAX_SPAWNS )
	{
		CSpawnPool* p = new CSpawnPool( uc );
		if ( p )
		{
			m_SpawnPool[ uc ] = p;
			m_ucSpawns++;
			return p;
		}
	}
	return NULL;
}

CSpawnPool* CSpawnPoolManager::Find( BYTE b )
{
	if ( b < MAX_SPAWNS ) return m_SpawnPool[ b ];

	return NULL;
}

CSpawnPool* CSpawnPoolManager::FindTeam( BYTE b )
{
	unsigned char uc = 0, uc1 = 0;
	CSpawnPool* pPlayer = NULL;
	while ( ( uc < MAX_SPAWNS ) && ( uc1 < m_ucSpawns ) )
	{
		pPlayer = m_SpawnPool[ uc ];
		if ( pPlayer )
		{
			if ( pPlayer->GetTeam() == b ) return pPlayer;

			uc1++;
		}
		uc++;
	}

	return NULL;
}

bool CSpawnPoolManager::Remove( CSpawnPool *p )
{
	if ( p )
	{
		m_SpawnPool[ p->GetID() ] = NULL;
		delete p;

		m_ucSpawns--;

		return true;
	}
	return false;
}

bool CSpawnPoolManager::Remove( unsigned char uc )
{
	CSpawnPool* p = Find( uc );
	if ( p )
	{
		m_SpawnPool[ uc ] = NULL;
		delete p;

		m_ucSpawns--;

		return true;
	}
	return false;
}

void CSpawnPoolManager::RemoveAll()
{
	unsigned char uc = 0, uc1 = 0;
	CSpawnPool* pPlayer = NULL;
	while ( ( uc < MAX_SPAWNS ) && ( uc1 < m_ucSpawns ) )
	{
		pPlayer = m_SpawnPool[ uc ];
		if ( pPlayer )
		{
			m_SpawnPool[ uc ] = 0;
			delete pPlayer;

			uc1++;
		}
		uc++;
	}

	m_ucSpawns = 0;
}