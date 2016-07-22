#ifndef _GAMESENDPACKETS_H
#define _GAMESENDPACKETS_H

#include "Rcon.h"

class CGameSendPackets
{
public:
	static void							SetWeather							( unsigned char uc1, unsigned char uc2 );
	static void							SetTime								( unsigned char uc1, unsigned char uc2, unsigned char uc3 );
	static void							UpdateTimeRate						( unsigned char uc );
private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
