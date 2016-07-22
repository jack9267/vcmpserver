#ifndef _MISCSENDPACKETS_H
#define _MISCSENDPACKETS_H

#include "Net.h"

class CMiscSendPackets
{
public:
	static void							RconJoin							( char* szNick );
	static void							AdminChat							( char* szText );
	static void							AdminPM								( unsigned char uc, char* szText );
	static void							Announce							( unsigned char uc, char* szText );
	static void							AnnounceAll							( char* szText );
	static void							KickPlayer							( unsigned char uc );
	static void							BanPlayer							( unsigned char uc );
	static void							BanIP								( char* szIP );
	static void							UnbanIP								( char* szIP );
	static void							SetPassword							( char* szPassword );
	static void							SetMaxPlayers						( unsigned char uc );
	static void							SetScoreBoard						( bool b );
	static void							SetDriveBy							( bool b );
	static void							MessageAllExcept					( unsigned char uc, char* szText );
	static void							MessagePlayer						( unsigned char uc, char* szText );
	static void							SendPlayerMessage					( unsigned char uc1, unsigned char uc2, char* szText );
	static void							SetServerName						( char* szName );
	static void							SetGamemodeName						( char* szMode );
private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
