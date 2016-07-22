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