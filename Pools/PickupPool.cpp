/*
	PickupPool.cpp
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

#include "PickupPool.h"
#include "Main.h"

CPickupPool::CPickupPool( unsigned int ui )
{
	m_uiID = ui;

	m_uiModel = 0;
	m_ucSpawnFor = INVALID_PLAYER_ID;
	m_bSpawned = true;
	m_bOneTime = false;

	m_uiRespawnTime = CConfigLoader::GetPickupRespawnTime();
	m_uiPickedUpTime = 0;
}

CPickupPool::~CPickupPool(void)
{
}

unsigned int CPickupPoolManager::m_uiPickups = 0;
unsigned int CPickupPoolManager::m_uiLastFreeID = 1;
CPickupPool* CPickupPoolManager::m_PickupPool[ MAX_PICKUPS ] = { 0 };

// The Manager
CPickupPool* CPickupPoolManager::New( void )
{
	unsigned int ui = m_uiLastFreeID;
	if ( ui < MAX_PICKUPS )
	{
		CPickupPool* p = new CPickupPool( ui );
		if ( p )
		{
			m_PickupPool[ ui ] = p;
			m_uiPickups++;

			m_uiLastFreeID++;
			m_uiLastFreeID = FindFreeID();
			return p;
		}
	}
	return NULL;
}

CPickupPool* CPickupPoolManager::New( unsigned int uc )
{
	if ( uc < MAX_PICKUPS )
	{
		CPickupPool* p = new CPickupPool( uc );
		if ( p )
		{
			m_PickupPool[ uc ] = p;
			m_uiPickups++;

			if ( uc == m_uiLastFreeID ) m_uiLastFreeID = FindFreeID();
			return p;
		}
	}
	return NULL;
}

CPickupPool* CPickupPoolManager::Find( unsigned int ui )
{
	if ( ui < MAX_PICKUPS ) return m_PickupPool[ ui ];

	return NULL;
}

bool CPickupPoolManager::Remove( CPickupPool *p )
{
	if ( p )
	{
		if ( p->GetID() < m_uiLastFreeID ) m_uiLastFreeID = p->GetID();

		m_PickupPool[ p->GetID() ] = NULL;
		m_uiPickups--;
		delete p;

		return true;
	}
	return false;
}

bool CPickupPoolManager::Remove( unsigned int ui )
{
	CPickupPool* p = Find( ui );
	if ( p )
	{
		if ( ui < m_uiLastFreeID ) m_uiLastFreeID = ui;

		m_PickupPool[ ui ] = NULL;
		m_uiPickups--;
		delete p;

		return true;
	}
	return false;
}

void CPickupPoolManager::RemoveAll()
{
	unsigned int ui = 0, ui1 = 0;
	CPickupPool* pPlayer = NULL;
	while ( ( ui < MAX_PICKUPS ) && ( ui1 < m_uiPickups ) )
	{
		pPlayer = m_PickupPool[ ui ];
		if ( pPlayer )
		{
			m_PickupPool[ ui ] = 0;
			delete pPlayer;

			ui1++;
		}
		ui++;
	}

	m_uiPickups = 0;
	m_uiLastFreeID = 0;
}

unsigned int CPickupPoolManager::FindFreeID( void )
{
	for ( unsigned int ui = m_uiLastFreeID; ui < MAX_PICKUPS; ui++ )
	{
		if ( !m_PickupPool[ ui ] ) return ui;
	}

	return MAX_PICKUPS;
}