#include "PickupPool.h"
#include "Main.h"

CPickupPool::CPickupPool( unsigned int ui )
{
	m_uiID = ui;

	m_uiModel = 0;
	m_ucSpawnFor = INVALID_PLAYER_ID;
	m_bSpawned = true;
	m_bOneTime = false;

	m_uiRespawnTime = 0;
	m_uiPickedUpTime = 0;
}

CPickupPool::~CPickupPool(void)
{
}

unsigned int CPickupPoolManager::m_uiPickups = 0;
CPickupPool* CPickupPoolManager::m_PickupPool[ MAX_PICKUPS ] = { 0 };

// The Manager
CPickupPool* CPickupPoolManager::New( unsigned int uc )
{
	if ( uc < MAX_PICKUPS )
	{
		CPickupPool* p = new CPickupPool( uc );
		if ( p )
		{
			m_PickupPool[ uc ] = p;
			m_uiPickups++;
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
		m_PickupPool[ p->GetID() ] = NULL;
		delete p;

		m_uiPickups--;

		return true;
	}
	return false;
}

bool CPickupPoolManager::Remove( unsigned int ui )
{
	CPickupPool* p = Find( ui );
	if ( p )
	{
		m_PickupPool[ ui ] = NULL;
		delete p;

		m_uiPickups--;

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
}