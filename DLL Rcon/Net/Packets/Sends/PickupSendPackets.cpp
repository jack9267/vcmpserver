#include "PickupSendPackets.h"

void CPickupSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_PICKUP_PACKET );
	bs.WriteCompressed( uc );
}

void CPickupSendPackets::CreatePickup( unsigned int ui, float fX, float fY, float fZ )
{
	RakNet::BitStream bs;

	PreparePacket(ID_PICKUP_CREATEPICKUP, bs);

	bs.Write(ui);
	bs.WriteVector(fX, fY, fZ);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CPickupSendPackets::DeletePickup( unsigned int ui )
{
	CPickupPool* pPickup = CPickupPoolManager::Find( ui );
	if ( pPickup )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PICKUP_DELETEPICKUP, bs);

		bs.Write(ui);

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);

		CPickupPoolManager::Remove( pPickup );
	}
}

void CPickupSendPackets::RespawnPickup( unsigned int ui )
{
	CPickupPool* pPickup = CPickupPoolManager::Find( ui );
	if ( pPickup )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PICKUP_RESPAWNPICKUP, bs);

		bs.Write(ui);

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPickupSendPackets::RespawnPickupForPlayer( unsigned char uc, unsigned int ui )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( ui );
	if ( pPlayer )
	{
		CPickupPool* pPickup = CPickupPoolManager::Find( ui );
		if ( pPickup )
		{
			RakNet::BitStream bs;

			PreparePacket(ID_PICKUP_RESPAWNPICKUPFORPLAYER, bs);

			bs.WriteCompressed( uc );
			bs.Write( ui );

			CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
		}
	}
}

void CPickupSendPackets::SetOnlyOnce( unsigned int ui, bool b )
{
	CPickupPool* pPickup = CPickupPoolManager::Find( ui );
	if ( pPickup )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PICKUP_PICKUPONCE, bs);

		bs.Write( ui );
		bs.Write( b );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPickupSendPackets::SetPickupRespawnTime( unsigned int ui, unsigned char uc )
{
	CPickupPool* pPickup = CPickupPoolManager::Find( ui );
	if ( pPickup )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PICKUP_SETPICKUPRESPAWNTIME, bs);

		bs.Write( ui );
		bs.WriteCompressed( uc );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPickupSendPackets::SetRespawnTime( unsigned char uc )
{
	RakNet::BitStream bs;

	PreparePacket(ID_PICKUP_SETRESPAWNTIME, bs);

	bs.WriteCompressed( uc );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}