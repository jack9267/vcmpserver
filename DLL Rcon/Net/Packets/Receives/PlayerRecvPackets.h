#ifndef _PLAYERRECVPACKETS_H
#define _PLAYERRECVPACKETS_H

#include "Net.h"
#include "Pools.h"

class CPlayerRecvPackets
{
public:
	static void						Parse				( Packet* pPacket );
private:
	static void						Join				( void );
	static void						Part				( void );
	static void						Spawn				( void );
	static void						Chat				( void );
	static void						Action				( void );
	static void						PM					( void );
	static void						TeamChat			( void );
	static void						Kill				( void );
	static void						TeamKill			( void );
	static void						Death				( void );
	static void						Command				( void );
	static void						StartSpectating		( void );
	static void						StopSpectating		( void );
	static void						SyncData			( void );
	static void						VehicleEnter		( void );
	static void						VehicleExit			( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
