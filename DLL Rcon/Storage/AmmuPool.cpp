#include "AmmuPool.h"

CAmmuPool::CAmmuPool( unsigned char uc )
{
	m_ucID = uc;

	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;

	m_fCameraX = 0.0f;
	m_fCameraY = 0.0f;
	m_fCameraZ = 0.0f;

	m_fCameraAngleX = 0.0f;
	m_fCameraAngleY = 0.0f;
	m_fCameraAngleZ = 0.0f;

	m_fWeaponX = 0.0f;
	m_fWeaponY = 0.0f;
	m_fWeaponZ = 0.0f;

	for ( unsigned char uc = 0; uc < 4; uc++ )
	{
		m_uiWeapon[ uc ] = 0;
		m_uiWeaponCost[ uc ] = 0;
		m_bWeaponEnabled[ uc ] = false;
	}
}

CAmmuPool::~CAmmuPool(void)
{
}

unsigned char CAmmuPoolManager::m_ucAmmunations = 0;
CAmmuPool* CAmmuPoolManager::m_AmmuPool[ MAX_AMMUS ] = { 0 };

// The Manager
CAmmuPool* CAmmuPoolManager::New( unsigned char uc )
{
	if ( uc < MAX_AMMUS )
	{
		CAmmuPool* p = new CAmmuPool( uc );
		if ( p )
		{
			m_AmmuPool[ uc ] = p;
			m_ucAmmunations++;
			return p;
		}
	}
	return NULL;
}

CAmmuPool* CAmmuPoolManager::Find( BYTE b )
{
	if ( b < MAX_AMMUS ) return m_AmmuPool[ b ];

	return NULL;
}

bool CAmmuPoolManager::Remove( CAmmuPool *p )
{
	if ( p )
	{
		m_AmmuPool[ p->GetID() ] = NULL;
		delete p;

		m_ucAmmunations--;

		return true;
	}
	return false;
}

bool CAmmuPoolManager::Remove( unsigned char uc )
{
	CAmmuPool* p = Find( uc );
	if ( p )
	{
		m_AmmuPool[ uc ] = NULL;
		delete p;

		m_ucAmmunations--;

		return true;
	}
	return false;
}

void CAmmuPoolManager::RemoveAll()
{
	unsigned char uc = 0, uc1 = 0;
	CAmmuPool* pPlayer = NULL;
	while ( ( uc < MAX_AMMUS ) && ( uc1 < m_ucAmmunations ) )
	{
		pPlayer = m_AmmuPool[ uc ];
		if ( pPlayer )
		{
			m_AmmuPool[ uc ] = 0;
			delete pPlayer;

			uc1++;
		}
		uc++;
	}

	m_ucAmmunations = 0;
}