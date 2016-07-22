#ifndef _MISCRECVPACKETS_H
#define _MISCRECVPACKETS_H

#include "Net.h"
#include "Pools.h"
#include "ConfigLoader.h"

class CMiscRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );
private:
	static void						RconReject				( void );
	static void						RconList				( void );
	static void						PlayerList				( void );
	static void						VehicleList				( void );
	static void						PickupList				( void );
	static void						AmmuList				( void );
	static void						ServerData				( void );
	static void						AdminChat				( void );
	static void						AdminPM					( void );
	static void						Announce				( void );
	static void						AnnounceAll				( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
