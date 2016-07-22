#include "CarCols.h"
#include "Main.h"

CCarCols::CCarCols( unsigned char ucModel )
{
	m_ucModel = ucModel;

	m_ucColours = 0;

	m_ucRandCol = 0;

	for ( unsigned char uc = 0; uc < 50; uc++ )
	{
		m_ucCol1[ uc ] = 0;
		m_ucCol2[ uc ] = 0;
	}
}

CCarCols::~CCarCols(void)
{
}

void CCarCols::GenerateRandCol( void )
{
	m_ucRandCol = rand() % m_ucColours;
}

void CCarCols::AddColour( unsigned char ucCol1, unsigned char ucCol2 )
{
	if ( m_ucColours < 50 )
	{
		m_ucCol1[ m_ucColours ] = ucCol1;
		m_ucCol2[ m_ucColours ] = ucCol2;

		m_ucColours++;
	}
}

unsigned char CCarCols::RandomColour1( void )
{
	if ( m_ucCol1[ m_ucRandCol ] ) return m_ucCol1[ m_ucRandCol ];

	return 0;
}

unsigned char CCarCols::RandomColour2( void )
{
	if ( m_ucCol2[ m_ucRandCol ] ) return m_ucCol2[ m_ucRandCol ];

	return 0;
}

// The Manager
unsigned char CCarColManager::m_ucCols = 0;
CCarCols* CCarColManager::m_CarCols[ 110 ] = { 0 };

CCarCols* CCarColManager::New( unsigned char ucModel )
{
	ucModel -= 130;

	if ( ucModel < 110 )
	{
		CCarCols* pCarCol = new CCarCols( ucModel );
		if ( pCarCol )
		{
			m_CarCols[ ucModel ] = pCarCol;
			m_ucCols++;

			return pCarCol;
		}
	}

	return NULL;
}

CCarCols* CCarColManager::Find( unsigned char ucModel )
{
	ucModel -= 130;

	if ( ucModel < 110 ) return m_CarCols[ ucModel ];

	return NULL;
}

bool CCarColManager::Remove( CCarCols* p )
{
	if ( p )
	{
		m_CarCols[ p->GetModel() ] = 0;
		m_ucCols--;

		delete p;

		return true;
	}

	return false;
}

bool CCarColManager::Remove( unsigned char uc )
{
	uc -= 130;

	CCarCols* p = Find( uc );
	if ( p )
	{
		m_CarCols[ uc ] = 0;
		m_ucCols--;

		delete p;

		return true;
	}

	return false;
}

void CCarColManager::RemoveAll( void )
{
	unsigned char uc = 0, uc1 = 0;

	while ( ( uc < 110 ) && ( uc1 < m_ucCols ) )
	{
		CCarCols* pCol = m_CarCols[ uc ];
		if ( pCol )
		{
			m_CarCols[ uc ] = 0;

			delete pCol;

			uc1++;
		}

		uc++;
	}

	m_ucCols = 0;
}