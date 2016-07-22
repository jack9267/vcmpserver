/*
	Net.cpp
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

#include "Net.h"
#include "Pools.h"
#include "NetRPC.h"
#include "NetSends.h"
#include "ConfigLoader.h"

RakServerInterface* CNet::m_pRakServer = NULL;
unsigned int CNet::iNextPingUpdate = 0;
unsigned int CNet::iNextAnnounceTime = 0;

extern SERVER_INFO pServerInfo;

/// This function wipes the vehicle stuff
void RemoveFromVehicle( CPlayerPool* pPlayer )
{
	if ( pPlayer )
	{
		if ( pPlayer->GetVehicle() )
		{
			CVehiclePool* pVehicle = CVehiclePoolManager::Find( pPlayer->GetVehicle() );
			if ( pVehicle )
			{
				pPlayer->SetVehicle( -1 );
				pPlayer->SetVehicleSeat( 255 );

				pVehicle->SetLastUsedTime( RakNet::GetTime() );
				pVehicle->SetBeenUsed( true );

				pVehicle->SetDriver( INVALID_PLAYER_ID );

				CScripting::PushFunction( "onPlayerExitVehicle" );
				CScripting::PushPlayerPointer( pPlayer );
				CScripting::PushVehiclePointer( pVehicle );
				CScripting::PushBool( pPlayer->GetVehiclePassenger() != 18 ? true : false );
				CScripting::CallFunction();

				CPlayerSendPackets::ExitVehicle( pPlayer->GetID(), pVehicle->GetID(), pVehicle->GetModel(), pVehicle->GetHealth() );
			}
		}
	}
}

bool CNet::Start( void )
{
	m_pRakServer = RakNetworkFactory::GetRakServerInterface();
	if ( m_pRakServer )
	{
		m_pRakServer->InitializeSecurity( 0, 0 );
		m_pRakServer->Start( CConfigLoader::GetMaxPlayers(), 0, 5, CConfigLoader::GetListenPort() );
		m_pRakServer->StartOccasionalPing();
		m_pRakServer->StartSynchronizedRandomInteger();

		if( strcmp( CConfigLoader::GetPassword(), "none" ) ) 
		{
			m_pRakServer->SetPassword( CConfigLoader::GetPassword() );

			pServerInfo.bLocked = true;
		}

		RegisterRPCs( m_pRakServer );

		Announce();

		return true;
	}

	return false;
}

void CNet::Stop( void )
{
	if ( m_pRakServer )
	{
		UnRegisterRPCs( m_pRakServer );

		m_pRakServer->Disconnect( 100 );
		RakNetworkFactory::DestroyRakServerInterface( m_pRakServer );
		m_pRakServer = 0;
	}
}

unsigned char CNet::GetPacketID( Packet* pPacket )
{
	unsigned char ucReturn;

	if ( pPacket == 0 ) 
	{
		ucReturn = (unsigned char)255;
	}
	else
	{
		if ( (unsigned char)pPacket->data[0] == ID_TIMESTAMP )
		{
			assert( pPacket->length > sizeof( unsigned char ) + sizeof( unsigned long ) );
			ucReturn = (unsigned char) pPacket->data[sizeof( unsigned char ) + sizeof( unsigned long )];
		}
		else 
		{
			ucReturn = (unsigned char) pPacket->data[0];
		}
	}
	return ucReturn;
}

void CNet::GetIP ( char szIP[22], unsigned char bID )
{
	if ( m_pRakServer )
	{
		char sz[16] = { NULL };
		unsigned short usPort = 0;
		PlayerID pID = m_pRakServer->GetPlayerIDFromIndex ( bID );
		m_pRakServer->GetPlayerIPFromID( pID, szIP, &usPort );
	}
}

void CNet::Listen( void )
{
	if ( m_pRakServer )
	{
		Packet* pPacket = 0;
		CPlayerPool* pPlayer = NULL;
		char szIP[ 22 ] = { 0 };

		while ( pPacket = m_pRakServer->Receive( ) )
		{
			switch ( GetPacketID( pPacket ) )
			{
				case ID_NEW_INCOMING_CONNECTION:
					GetIP( szIP, static_cast< unsigned char >( pPacket->playerIndex ) );

					pPlayer = CPlayerPoolManager::New( pPacket->playerIndex );
					if ( pPlayer )
					{
						pPlayer->SetIP( szIP );
						pPlayer->SetPlayerID( pPacket->playerId );
					}

					CConsole::PrintC( "CONNECTION", COLOUR_RED, "Incoming [%s] | Index [%i]", szIP, pPacket->playerIndex );
					break;

				case ID_DISCONNECTION_NOTIFICATION:
					pPlayer = CPlayerPoolManager::Find( pPacket->playerId );
					if ( pPlayer )
					{
						CConsole::PrintC( "CONNECTION", COLOUR_RED, "Closed [%s] | Index [%i]", pPlayer->GetIP(), pPacket->playerIndex );

						CNetSends::PlayerPart( pPlayer->GetID(), PARTREASON_CLOSED );
						CPlayerSendPackets::Part( pPlayer->GetID(), PARTREASON_CLOSED );

						RemoveFromVehicle( pPlayer );

						CPlayerPoolManager::Remove( pPlayer );
					}
					break;

				case ID_CONNECTION_LOST:
					pPlayer = CPlayerPoolManager::Find( pPacket->playerId );
					if ( pPlayer )
					{
						CConsole::PrintC( "CONNECTION", COLOUR_RED, "Lost [%s] | Index [%i]", pPlayer->GetIP(), pPacket->playerIndex );

						CScripting::PushFunction( "onPlayerPart" );
						CScripting::PushPlayerPointer( pPlayer );
						CScripting::PushInteger( PARTREASON_LOST );
						CScripting::CallFunction();

						CNetSends::PlayerPart( pPlayer->GetID(), PARTREASON_LOST );
						CPlayerSendPackets::Part( pPlayer->GetID(), PARTREASON_LOST );

						RemoveFromVehicle( pPlayer );

						CPlayerPoolManager::Remove( pPlayer );
					}
					break;

				case ID_RECEIVED_STATIC_DATA:
					// Static Data
					break;

				case ID_PLAYER_SYNC:
					//Player Sync
					PlayerSync( pPacket );
					break;

				case ID_AIM_SYNC:
					// Aim Sync
					AimSync( pPacket );
					break;

				case ID_VEHICLE_SYNC:
					VehicleSync( pPacket );
					break;

				case ID_PASSENGER_SYNC:
					PassengerSync( pPacket );
					break;

				default:
#ifdef _DEBUG
					//CConsole::PrintC( "PACKET", COLOUR_BLUE, "Unknown packet %i received", pPacket->data[0] );
#endif
					break;
			}

			m_pRakServer->DeallocatePacket( pPacket );	
		}

		ProcessPings();
		Announce();
	}
}

void CNet::ProcessPings( void )
{
	if ( ( m_pRakServer ) && ( m_pRakServer->IsActive() ) )
	{
		unsigned int iTime = RakNet::GetTime();

		if ( iTime > iNextPingUpdate )
		{
			iNextPingUpdate = iTime + 1000;

			unsigned char uc = 0, uc1 = 0;
			unsigned short usPing = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					usPing = m_pRakServer->GetLastPing( pPlayer->GetPlayerID() );

					pPlayer->SetPing( usPing );

					uc1++;
				}
				uc++;
			}
		}
	}
}

void CNet::Announce( void )
{
	unsigned int iTime = RakNet::GetTime();

	if ( iTime > iNextAnnounceTime )
	{
		iNextAnnounceTime = iTime + 21600;

		char szAnnounce[ 64 ] = { NULL };
#ifdef _WIN32
		sprintf( szAnnounce, "%i", CConfigLoader::GetListenPort() );

		ShellExecute( NULL, "open", "Announce.exe", szAnnounce, NULL, NULL );
#else
		sprintf( szAnnounce, "./announce %i &", CConfigLoader::GetListenPort() );

		system( szAnnounce );
#endif
	}
}