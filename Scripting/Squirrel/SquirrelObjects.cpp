/*
	SquirrelObjects.cpp
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

#include "SquirrelObjects.h"
#include "SendPackets.h"

CSquirrelObjects::~CSquirrelObjects(void)
{
	RemoveAllTimers();
	RemoveAllSockets();
}

void CSquirrelObjects::AddTimer( CSquirrelTimers* p )
{
	if ( p )
	{
		m_Timers.push_back( p );
	}
}

bool CSquirrelObjects::RemoveTimer( CSquirrelTimers* p )
{
	if ( p )
	{
		// This loop checks to see if the pickup is owned by this script...
		std::list< CSquirrelTimers* >::iterator iter = m_Timers.begin();
		for ( ; iter != m_Timers.end(); iter++ )
		{
			if ( (*iter) == p )
			{
				m_Timers.remove( p );

				CSquirrelTimerManager::Remove( p );

				return true;
			}
		}
	}

	return false;
}

void CSquirrelObjects::RemoveAllTimers( void )
{
	std::list< CSquirrelTimers* >::iterator iter = m_Timers.begin();
	for ( ; iter != m_Timers.end(); iter++ )
	{
		CSquirrelTimerManager::Remove( (*iter) );
	}

	m_Timers.clear();
}

void CSquirrelObjects::AddSocket( CSquirrelSockets* p )
{
	if ( p )
	{
		m_Sockets.push_back( p );
	}
}

bool CSquirrelObjects::RemoveSocket( CSquirrelSockets* p )
{
	if ( p )
	{
		// This loop checks to see if the pickup is owned by this script...
		std::list< CSquirrelSockets* >::iterator iter = m_Sockets.begin();
		for ( ; iter != m_Sockets.end(); iter++ )
		{
			if ( (*iter) == p )
			{
				m_Sockets.remove( p );

				CSquirrelSocketManager::Remove( p );

				return true;
			}
		}
	}

	return false;
}

void CSquirrelObjects::RemoveAllSockets( void )
{
	std::list< CSquirrelSockets* >::iterator iter = m_Sockets.begin();
	for ( ; iter != m_Sockets.end(); iter++ )
	{
		CSquirrelSocketManager::Remove( (*iter) );
	}

	m_Sockets.clear();
}