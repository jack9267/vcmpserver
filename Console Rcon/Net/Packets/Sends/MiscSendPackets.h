#ifndef _MISCSENDPACKETS_H
#define _MISCSENDPACKETS_H

#include "Net.h"

class CMiscSendPackets
{
public:
	static void							JoinRequest					( void );

private:
	static void							PreparePacket				( unsigned char uc, RakNet::BitStream &bs );
};

#endif
