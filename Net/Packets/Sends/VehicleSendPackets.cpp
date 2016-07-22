/*
	VehicleSendPackets.cpp
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

#include "VehicleSendPackets.h"

void CVehicleSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_VEHICLE_PACKET );
	bs.WriteCompressed( uc );
}

void CVehicleSendPackets::VehicleSync( unsigned char uc1, unsigned char uc2, INCAR_SYNC_DATA icSync )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc2 );
		if ( pVehicle )
		{
			RakNet::BitStream bs;

			PreparePacket( ID_VEHICLE_SYNCDATA, bs );

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );
			bs.WriteVector( icSync.vecPos.X, icSync.vecPos.Y, icSync.vecPos.Z );
			bs.WriteCompressed( icSync.bytePlayerHealth );
			bs.Write( icSync.fHealth );

			RconInterface->Send( &bs, LOW_PRIORITY, UNRELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}