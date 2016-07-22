/*
	Rcon.cpp
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

#include "Rcon.h"
#include "Pools.h"
#include "ConfigLoader.h"
#include "Console.h"

RakServerInterface* CRcon::m_pRak = NULL;

bool CRcon::Start(void)
{
	m_pRak = RakNetworkFactory::GetRakServerInterface();
	if ( m_pRak )
	{
		m_pRak->Start(CConfigLoader::GetRconMaxUsers(), 0, 5, CConfigLoader::GetRconPort());

		m_pRak->StartOccasionalPing();

		m_pRak->SetPassword(CConfigLoader::GetRconPassword());

		CConsole::PrintTime( "*** Remote Console started on port %i ***", COLOUR_GREY, 25, CConfigLoader::GetRconPort());

		return true;
	}

	return false;
}

void CRcon::Stop(void)
{
	if (m_pRak)
	{
		RakNetworkFactory::DestroyRakServerInterface(m_pRak);
		m_pRak = 0;
	}
}

void CRcon::GetIP(char szIP[22], unsigned char bID)
{
	if (m_pRak)
	{
		unsigned short usPort = 0;
		PlayerID pID = m_pRak->GetPlayerIDFromIndex(bID);
		m_pRak->GetPlayerIPFromID(pID, szIP, &usPort);
	}
}

void CRcon::Listen(void)
{
	if (m_pRak)
	{
		Packet* pPacket = 0;
		CRconPool* pRcon = NULL;
		char szIP[ 22 ] = { 0 };

		while (pPacket = m_pRak->Receive())
		{
			switch((unsigned char)pPacket->data[0])
			{
				case ID_NEW_INCOMING_CONNECTION:
					GetIP( szIP, static_cast< unsigned char >( pPacket->playerIndex ) );

					pRcon = CRconPoolManager::New( pPacket->playerIndex );
					if ( pRcon )
					{
						pRcon->SetIP( szIP );
						pRcon->SetConnectionID( pPacket->playerId );
					}

					CConsole::PrintC("RCON", COLOUR_GREEN, "%s Connected", szIP);
					break;

				case ID_DISCONNECTION_NOTIFICATION:
					pRcon = CRconPoolManager::Find( pPacket->playerId );
					if ( pRcon )
					{
						CConsole::PrintC("RCON", COLOUR_YELLOW, "%s Disconnected", pRcon->GetIP());

						CRconPoolManager::Remove( pRcon );
					}
					break;

				case ID_CONNECTION_LOST:
					pRcon = CRconPoolManager::Find( pPacket->playerId );
					if ( pRcon )
					{
						CConsole::PrintC("RCON", COLOUR_RED, "%s Connection lost.", pRcon->GetIP());

						CRconPoolManager::Remove( pRcon );
					}
					break;

				case ID_PLAYER_PACKET:
					CPlayerRecvPackets::Parse( pPacket );
					break;

				case ID_MISC_PACKET:
					CMiscRecvPackets::Parse( pPacket );
					break;

				case ID_VEHICLE_PACKET:
					CVehicleRecvPackets::Parse( pPacket );
					break;

				case ID_GAME_PACKET:
					CGameRecvPackets::Parse( pPacket );
					break;

				case ID_PICKUP_PACKET:
					CPickupRecvPackets::Parse( pPacket );
					break;
			}

			m_pRak->DeallocatePacket(pPacket);
		}
	}
}