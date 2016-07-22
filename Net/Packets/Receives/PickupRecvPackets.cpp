/*
	PickupRecvPackets.cpp
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

#include "PickupRecvPackets.h"

RakNet::BitStream CPickupRecvPackets::bs;
Packet* CPickupRecvPackets::m_pPacket = NULL;

void CPickupRecvPackets::Parse( Packet* p )
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
		case ID_PICKUP_CREATEPICKUP:
			CreatePickup();
			break;
		case ID_PICKUP_RESPAWNPICKUP:
			RespawnPickup();
			break;
		case ID_PICKUP_RESPAWNPICKUPFORPLAYER:
			RespawnPickupForPlayer();
			break;
		case ID_PICKUP_DELETEPICKUP:
			DeletePickup();
			break;
		case ID_PICKUP_PICKUPONCE:
			SetOnlyOnce();
			break;
		case ID_PICKUP_SETPICKUPRESPAWNTIME:
			SetPickupRespawnTime();
			break;
		case ID_PICKUP_SETRESPAWNTIME:
			SetRespawnTime();
			break;
		}
	}
}

void CPickupRecvPackets::CreatePickup( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned int uiModel = 0;
		float fX = 0.0, fY = 0.0, fZ = 0.0;

		bs.Read(uiModel);
		bs.ReadVector(fX,fY,fZ);

		CPickupPool* pPickup = CPickupPoolManager::New();
		if (pPickup)
		{
			pPickup->SetModel(uiModel);
			pPickup->SetX(fX);
			pPickup->SetY(fY);
			pPickup->SetZ(fZ);

			CPickupSendPackets::NewPickup( pPickup->GetID() );

			unsigned char uc = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnPickupForPlayer( uc, pPickup->GetID() );
				}
				uc++;
			}
		}
	}
}

void CPickupRecvPackets::RespawnPickup( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned int uiPickup = 0;

		bs.Read(uiPickup);

		CPickupPool* pPickup = CPickupPoolManager::Find( uiPickup );
		if ( pPickup )
		{
			unsigned char uc = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnPickupForPlayer( uc, uiPickup, uc );
				}
				uc++;
			}
		}
	}
}

void CPickupRecvPackets::RespawnPickupForPlayer( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		unsigned int ui = 0;

		bs.ReadCompressed( uc );
		bs.Read( ui );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CPickupPool* pPickup = CPickupPoolManager::Find( ui );
			if ( pPickup )
			{
				CNetSends::SpawnPickupForPlayer( uc, ui, uc );
			}
		}
	}
}

void CPickupRecvPackets::DeletePickup( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned int uiPickup = 0;

		bs.Read(uiPickup);

		CPickupPool* pPickup = CPickupPoolManager::Find( uiPickup );
		if ( pPickup )
		{
			CNetSends::DeletePickupForAll( uiPickup );
		}
	}
}

void CPickupRecvPackets::SetOnlyOnce( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned int ui = 0;
		bool b = false;

		bs.Read( ui );
		bs.Read( b );

		CPickupPool* pPickup = CPickupPoolManager::Find( ui );
		if ( pPickup )
		{
			pPickup->SetOneTime( b );
		}
	}
}

void CPickupRecvPackets::SetPickupRespawnTime( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned int ui = 0;
		unsigned char uc = 0;

		bs.Read( ui );
		bs.ReadCompressed( uc );

		CPickupPool* pPickup = CPickupPoolManager::Find( ui );
		if ( pPickup )
		{
			pPickup->SetRespawnTime( uc * 1000 );
		}
	}
}

void CPickupRecvPackets::SetRespawnTime( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed( uc );

		CConfigLoader::SetPickupRespawnTime( uc * 1000 );
	}
}