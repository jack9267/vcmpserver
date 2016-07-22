#ifndef _VEHICLERECVPACKETS_H
#define _VEHICLERECVPACKETS_H

#include "RconPool.h"
#include "NetSends.h"
#include "Pools.h"

class CVehicleRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );

private:
	static void						SetPosition				( void );
	static void						SetDoorStatus			( void );
	static void						KillEngine				( void );
	static void						SetVehicleHealth		( void );
	static void						RespawnVehicle			( void );
	static void						RespawnVehicleAt		( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
