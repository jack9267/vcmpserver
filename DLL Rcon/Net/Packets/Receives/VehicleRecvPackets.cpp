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
		case ID_VEHICLE_SYNCDATA:
			Sync();
			break;
		}
	}
}

void CVehicleRecvPackets::Sync(void)
{
	unsigned char uc1 = 0, uc2 = 0;
	float fX = 0.0, fY = 0.0, fZ = 0.0, fHealth = 0.0;
	BYTE bHealth = 0;

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);
	bs.ReadVector(fX,fY,fZ);
	bs.ReadCompressed(bHealth);
	bs.Read(fHealth);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc2 );
		if ( pVehicle )
		{
			if ((pPlayer->GetX() != fX) && (pPlayer->GetY() != fY) && (pPlayer->GetZ() != fZ))
			{
				CallSignal("vcmp.vehicle.move", "%i %s %f %f %f %f %f %f", uc1, pPlayer->GetNick(), pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetZ(), fX, fY, fZ );
			}

			if (pPlayer->GetHealth() != bHealth) CallSignal( "vcmp.player.healthchange", "%i %s %i %i", uc1, pPlayer->GetNick(), pPlayer->GetHealth(), bHealth );
			if (pVehicle->GetHealth() != fHealth) CallSignal( "vcmp.vehicle.healthchange", "%i %f %f", uc2, pVehicle->GetHealth(), fHealth );

			pPlayer->SetX(fX);
			pPlayer->SetY(fY);
			pPlayer->SetZ(fZ);
			pPlayer->SetHealth(bHealth);
			pPlayer->SetVehicle( uc2 );

			pVehicle->SetX(fX);
			pVehicle->SetY(fY);
			pVehicle->SetZ(fZ);
			pVehicle->SetHealth(fHealth);
		}
	}
}