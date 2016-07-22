#ifndef _VEHICLESENDPACKETS_H
#define _VEHICLESENDPACKETS_H

#include "Net.h"

class CVehicleSendPackets
{
public:
	static void							SetPosition							( unsigned char uc, float fX, float fY, float fZ );
	static void							SetDoorStatus						( unsigned char uc );
	static void							KillEngine							( unsigned char uc );
	static void							SetHealth							( unsigned char uc );
private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
