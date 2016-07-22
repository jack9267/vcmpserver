#ifndef _MISCRECVPACKETS_H
#define _MISCRECVPACKETS_H

#include "Net.h"

class CMiscRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );

private:
	static void						PlayerList				( void );

	// Storage
	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
