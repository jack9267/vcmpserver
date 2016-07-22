/*
	VehicleRecvPackets.cpp
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

#include "VehicleRecvPackets.h"

RakNet::BitStream CVehicleRecvPackets::bs;
Packet* CVehicleRecvPackets::m_pPacket = NULL;

void CVehicleRecvPackets::Parse( Packet* p )
{
	if ( p )
	{
		unsigned char ucPacket1 = 0, ucPacket2 = 0;

		m_pPacket = p;

		bs = RakNet::BitStream( (char*)p->data, p->bitSize, false );

		bs.Read( ucPacket1 );
		bs.ReadCompressed( ucPacket2 );

		switch ( ucPacket2 )
		{
		case ID_VEHICLE_SETPOSITION:
			SetPosition();
			break;
		case ID_VEHICLE_SETDOORSTATUS:
			SetDoorStatus();
			break;
		case ID_VEHICLE_KILLENGINE:
			KillEngine();
			break;
		case ID_VEHICLE_SETHEALTH:
			SetVehicleHealth();
			break;

		case ID_VEHICLE_RESPAWN:
			RespawnVehicle();
			break;

		case ID_VEHICLE_RESPAWNAT:
			RespawnVehicleAt();
			break;
		}
	}
}

void CVehicleRecvPackets::SetPosition( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		float fX = 0.0, fY = 0.0, fZ = 0.0;

		bs.ReadCompressed(uc);
		bs.ReadVector(fX,fY,fZ);

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
		if ( pVehicle )
		{
			pVehicle->SetX(fX);
			pVehicle->SetY(fY);
			pVehicle->SetZ(fZ);
			CNetSends::SetVehiclePosition( uc );
		}
	}
}

void CVehicleRecvPackets::SetDoorStatus( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		bool bLocked = false;

		bs.ReadCompressed(uc);
		bs.Read(bLocked);

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
		if ( pVehicle )
		{
			pVehicle->SetLocked(bLocked);
			CNetSends::SetVehicleDoorStatus( uc );
		}
	}
}

void CVehicleRecvPackets::KillEngine( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed(uc);

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
		if ( pVehicle )
		{
			CNetSends::KillVehicleEngine( uc );
		}
	}
}

void CVehicleRecvPackets::SetVehicleHealth( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		float fHealth = 0.0;

		bs.ReadCompressed( uc );
		bs.Read( fHealth );

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
		if ( pVehicle )
		{
			pVehicle->SetHealth( fHealth );
			CNetSends::SetVehicleHealth( pVehicle->GetID() );
		}
	}
}

void CVehicleRecvPackets::RespawnVehicle( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char ucVehicle = 0;

		bs.ReadCompressed( ucVehicle );

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( ucVehicle );
		if ( pVehicle )
		{
			pVehicle->SetX( pVehicle->GetSpawnX() );
			pVehicle->SetY( pVehicle->GetSpawnY() );
			pVehicle->SetZ( pVehicle->GetSpawnZ() );
			pVehicle->SetRotation( pVehicle->GetSpawnRotation() );

			pVehicle->SetLastUsedTime( RakNet::GetTime() );
			pVehicle->SetBeenUsed( false );
			pVehicle->SetHealth( 1000.0f );

			unsigned int uc = 0, uc1 = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnVehicleForPlayer( uc, ucVehicle );
					uc1++;
				}
				uc++;
			}

			CScripting::PushFunction( "onVehicleRespawn" );
			CScripting::PushVehiclePointer( pVehicle );
			CScripting::CallFunction();
		}
	}
}

void CVehicleRecvPackets::RespawnVehicleAt( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char ucVehicle = 0;
		float fX, fY, fZ, fAngle;
		fX = fY = fZ = fAngle = 0.0f;

		bs.ReadCompressed( ucVehicle );
		bs.ReadVector( fX, fY, fZ );
		bs.Read( fAngle );

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( ucVehicle );
		if ( pVehicle )
		{
			pVehicle->SetX( fX );
			pVehicle->SetY( fY );
			pVehicle->SetZ( fZ );
			pVehicle->SetRotation( fAngle );

			pVehicle->SetLastUsedTime( RakNet::GetTime() );
			pVehicle->SetBeenUsed( false );
			pVehicle->SetHealth( 1000.0f );

			unsigned int uc = 0, uc1 = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnVehicleForPlayer( uc, ucVehicle );
					uc1++;
				}
				uc++;
			}

			CScripting::PushFunction( "onVehicleRespawn" );
			CScripting::PushVehiclePointer( pVehicle );
			CScripting::CallFunction();
		}
	}
}