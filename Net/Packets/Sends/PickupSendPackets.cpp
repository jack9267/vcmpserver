/*
	PickupSendPackets.cpp
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

#include "PickupSendPackets.h"
#include "Pools.h"

void CPickupSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_PICKUP_PACKET );
	bs.WriteCompressed( uc );
}

void CPickupSendPackets::NewPickup( unsigned int ui )
{
	CPickupPool* pPickup = CPickupPoolManager::Find( ui );
	if (pPickup)
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PICKUP_CREATEPICKUP, bs );

		bs.Write( ui );
		bs.Write( pPickup->GetModel() );
		bs.WriteVector( pPickup->GetX(), pPickup->GetY(), pPickup->GetZ() );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPickupSendPackets::PickedUpPickup( unsigned char uc, unsigned int ui1, unsigned int ui2 )
{
	RakNet::BitStream bs;

	PreparePacket( ID_PICKUP_PICKEDUP, bs );

	bs.WriteCompressed( uc );
	bs.WriteCompressed( ui1 );
	bs.WriteCompressed( ui2 );

	RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
}

void CPickupSendPackets::DelPickup( unsigned int ui )
{
	RakNet::BitStream bs;

	PreparePacket( ID_PICKUP_DELETEPICKUP, bs );

	bs.Write( ui );

	RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
}