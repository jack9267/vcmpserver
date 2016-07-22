#include "VehicleSendPackets.h"

void CVehicleSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_VEHICLE_PACKET );
	bs.WriteCompressed( uc );
}

void CVehicleSendPackets::SetPosition( unsigned char uc, float fX, float fY, float fZ )
{
	CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
	if ( pVehicle )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_VEHICLE_SETPOSITION, bs);

		bs.WriteCompressed(uc);
		bs.WriteVector(fX,fY,fZ);

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CVehicleSendPackets::SetDoorStatus( unsigned char uc )
{
	CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
	if ( pVehicle )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_VEHICLE_SETDOORSTATUS, bs);

		bs.WriteCompressed(uc);
		bs.Write(pVehicle->GetLocked());

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CVehicleSendPackets::KillEngine( unsigned char uc )
{
	CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
	if ( pVehicle )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_VEHICLE_KILLENGINE, bs);

		bs.WriteCompressed(uc);

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CVehicleSendPackets::SetHealth( unsigned char uc )
{
	CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
	if ( pVehicle )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_VEHICLE_SETHEALTH, bs);

		bs.WriteCompressed( uc );
		bs.Write( pVehicle->GetHealth() );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}