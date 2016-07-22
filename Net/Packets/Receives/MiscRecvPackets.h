#ifndef _MISCRECVPACKETS_H
#define _MISCRECVPACKETS_H

#include "RconPool.h"
#include "NetSends.h"
#include "Pools.h"

class CMiscRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );

private:
	static void						AdminChat				( void );
	static void						RconJoin				( void );
	static void						AdminPM					( void );
	static void						Announce				( void );
	static void						AnnounceAll				( void );
	static void						SetPassword				( void );
	static void						SetMaxPlayers			( void );
	static void						SetScoreBoard			( void );
	static void						SetDriveBy				( void );
	static void						KickPlayer				( void );
	static void						BanPlayer				( void );
	static void						BanIP					( void );
	static void						Unban					( void );
	static void						MessageAllExcept		( void );
	static void						MessagePlayer			( void );
	static void						SendPlayerMessage		( void );
	static void						SetServerName			( void );
	static void						SetGamemodeName			( void );

	// Storage
	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
