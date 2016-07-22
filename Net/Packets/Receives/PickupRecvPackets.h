#ifndef _PICKUPRECVPACKETS_H
#define _PICKUPRECVPACKETS_H

#include "RconPool.h"
#include "NetSends.h"
#include "Pools.h"

class CPickupRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );

private:
	static void						CreatePickup			( void );
	static void						RespawnPickup			( void );
	static void						RespawnPickupForPlayer	( void );
	static void						DeletePickup			( void );
	static void						SetOnlyOnce				( void );
	static void						SetPickupRespawnTime	( void );
	static void						SetRespawnTime			( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
