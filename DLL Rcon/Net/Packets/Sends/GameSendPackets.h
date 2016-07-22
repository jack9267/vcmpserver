#ifndef _GAMESENDPACKETS_H
#define _GAMESENDPACKETS_H

#include "Net.h"

class CGameSendPackets
{
public:
	static void							SetWeather							( unsigned char uc );
	static void							SetTime								( unsigned char uc1, unsigned char uc2 );
	static void							SetMarker							( unsigned char uc, float fX, float fY, float fZ );
	static void							EnableWeaponAtAmmu					( unsigned char uc, unsigned char uc2 );
	static void							SetTimeRate							( unsigned char uc );
	static void							SetWeatherRate						( unsigned char uc );
	static void							CreateMarkerForPlayer				( unsigned char uc1, unsigned char uc2, float fX, float fY, float fZ );
	static void							SetAmmuWeapon						( unsigned char uc1, unsigned char uc2 );
private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
