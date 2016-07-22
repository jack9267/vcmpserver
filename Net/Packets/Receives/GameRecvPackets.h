#ifndef _GAMERECVPACKETS_H
#define _GAMERECVPACKETS_H

#include "RconPool.h"
#include "NetSends.h"
#include "Pools.h"

class CGameRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );

private:
	static void						RecvWeather				( void );
	static void						RecvTime				( void );
	static void						SetMarker				( void );
	static void						EnableWeaponAtAmmu		( void );
	static void						SetTimeRate				( void );
	static void						SetWeatherRate			( void );
	static void						CreateMarkerForPlayer	( void );
	static void						SetAmmuWeapon			( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
