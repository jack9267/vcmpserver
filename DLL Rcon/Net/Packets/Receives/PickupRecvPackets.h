#ifndef _PICKUPRECVPACKETS_H
#define _PICKUPRECVPACKETS_H

#include "Net.h"
#include "Pools.h"

class CPickupRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );
private:
	static void						NewPickup				( void );
	static void						PickedUp				( void );
	static void						DelPickup				( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
