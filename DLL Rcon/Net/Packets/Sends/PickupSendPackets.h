#ifndef _PICKUPSENDPACKETS_H
#define _PICKUPSENDPACKETS_H

#include "Net.h"

class CPickupSendPackets
{
public:
	static void							CreatePickup						( unsigned int ui, float fX, float fY, float fZ );
	static void							RespawnPickup						( unsigned int ui );
	static void							RespawnPickupForPlayer				( unsigned char uc, unsigned int ui );
	static void							DeletePickup						( unsigned int ui );
	static void							SetOnlyOnce							( unsigned int ui, bool b );
	static void							SetPickupRespawnTime				( unsigned int ui, unsigned char uc );
	static void							SetRespawnTime						( unsigned char uc );
private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
