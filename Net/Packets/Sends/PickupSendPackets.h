#ifndef _PICKUPSENDPACKETS_H
#define _PICKUPSENDPACKETS_H

#include "Rcon.h"

class CPickupSendPackets
{
public:
	static void							NewPickup							( unsigned int ui );
	static void							PickedUpPickup						( unsigned char uc, unsigned int ui1, unsigned int ui2 );
	static void							DelPickup							( unsigned int ui );

	// Forwarded player sync packet
	static void							FootSync							( unsigned char uc, ONFOOT_SYNC_DATA ofSync );

private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
