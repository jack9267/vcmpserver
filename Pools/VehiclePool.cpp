/*
	VehiclePool.cpp
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

#include "VehiclePool.h"

CVehiclePool::CVehiclePool( unsigned char uc )
{
	m_ucID = uc;

	m_bModel = 0;

	m_fHealth = 1000.0f;

	m_bColour1 = 0;
	m_bColour2 = 0;

	m_bDriver = INVALID_PLAYER_ID;

	m_fSpawnX = 0.0f;
	m_fSpawnY = 0.0f;
	m_fSpawnZ = 0.0f;
	m_fSpawnRotation = 0.0f;

	m_bLocked = false;

	m_bRandomColour1 = false;
	m_bRandomColour2 = false;

	m_bUsed = false;
	m_uiLastUsed = 0;
}

CVehiclePool::~CVehiclePool(void)
{
}

unsigned char CVehiclePoolManager::m_ucVehicles = 0;
CVehiclePool* CVehiclePoolManager::m_VehiclePool[ MAX_VEHICLES ] = { 0 };

// The Manager
CVehiclePool* CVehiclePoolManager::New( unsigned char uc )
{
	if ( uc < MAX_VEHICLES )
	{
		CVehiclePool* p = new CVehiclePool( uc );
		if ( p )
		{
			m_VehiclePool[ uc ] = p;
			m_ucVehicles++;
			return p;
		}
	}
	return NULL;
}

CVehiclePool* CVehiclePoolManager::Find( BYTE b )
{
	if ( b < MAX_VEHICLES ) return m_VehiclePool[ b ];

	return NULL;
}

bool CVehiclePoolManager::Remove( CVehiclePool *p )
{
	if ( p )
	{
		m_VehiclePool[ p->GetID() ] = NULL;
		delete p;

		m_ucVehicles--;

		return true;
	}
	return false;
}

bool CVehiclePoolManager::Remove( unsigned char uc )
{
	CVehiclePool* p = Find( uc );
	if ( p )
	{
		m_VehiclePool[ uc ] = NULL;
		delete p;

		m_ucVehicles--;

		return true;
	}
	return false;
}

void CVehiclePoolManager::RemoveAll()
{
	unsigned char uc = 0, uc1 = 0;
	CVehiclePool* pPlayer = NULL;
	while ( ( uc < MAX_VEHICLES ) && ( uc1 < m_ucVehicles ) )
	{
		pPlayer = m_VehiclePool[ uc ];
		if ( pPlayer )
		{
			m_VehiclePool[ uc ] = 0;
			delete pPlayer;

			uc1++;
		}
		uc++;
	}

	m_ucVehicles = 0;
}