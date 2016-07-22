#ifndef _VEHICLERECVPACKETS_H
#define _VEHICLERECVPACKETS_H

#include "Net.h"
#include "Pools.h"

class CVehicleRecvPackets
{
public:
	static void						Parse							( Packet* pPacket );

private:
	static void						Sync							( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
