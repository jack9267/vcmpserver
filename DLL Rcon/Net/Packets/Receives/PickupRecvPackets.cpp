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
			NewPickup();
			break;

		case ID_PICKUP_PICKEDUP:
			PickedUp();
			break;

		case ID_PICKUP_DELETEPICKUP:
			DelPickup();
			break;
		}
	}
}

void CPickupRecvPackets::NewPickup( void )
{
	unsigned int ui1 = 0, ui2 = 0;
	float fX = 0.0, fY = 0.0, fZ = 0.0;

	bs.Read(ui1);
	bs.Read(ui2);
	bs.ReadVector(fX,fY,fZ);

	CPickupPool* pPickup = CPickupPoolManager::Find( ui1 );
	if ( !pPickup )
	{
		pPickup = CPickupPoolManager::New( ui1 );
		if ( pPickup ) 
		{
			pPickup->SetModel(ui2);
			pPickup->SetX(fX);
			pPickup->SetY(fY);
			pPickup->SetZ(fZ);
		}
	}

	if ( pPickup )
	{
		CallSignal("vcmp.pickup.new", "%i %i %f %f %f", ui1, ui2, fX, fY, fZ);
	}
}

void CPickupRecvPackets::PickedUp( void )
{
	unsigned char uc = 0;
	unsigned int ui1 = 0, ui2 = 0;

	bs.ReadCompressed(uc);
	bs.ReadCompressed(ui1);
	bs.ReadCompressed(ui2);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CPickupPool* pPickup = CPickupPoolManager::Find( ui1 );
		if ( pPickup )
		{
			CallSignal("vcmp.pickup.hit", "%i %s %i %i", uc, pPlayer->GetNick(), ui1, ui2);
		}
	}
}

void CPickupRecvPackets::DelPickup(void)
{
	unsigned int ui = 0;

	bs.ReadCompressed( ui );

	CPickupPool* pPickup = CPickupPoolManager::Find( ui );
	if ( pPickup )
	{
		CallSignal( "vcmp.pickup.destroy", "%i %i", ui, pPickup->GetModel() );

		CPickupPoolManager::Remove( pPickup );
	}
}