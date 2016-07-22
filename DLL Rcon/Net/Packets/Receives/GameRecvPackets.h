#ifndef _GAMERECVPACKETS_H
#define _GAMERECVPACKETS_H

#include "Net.h"
#include "Pools.h"
#include "ConfigLoader.h"

class CGameRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );

private:
	static void						Weather					( void );
	static void						Time					( void );
	static void						TimeRate				( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
