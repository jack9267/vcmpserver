#ifndef _MISCSENDPACKETS_H
#define _MISCSENDPACKETS_H

#include "Rcon.h"

class CMiscSendPackets
{
public:
	static void							VersionReject						( unsigned char uc, unsigned int ui);

	static void							AdminChat							( unsigned char uc, char* szText );
	static void							AdminPM								( unsigned char uc1, unsigned char uc2, char* szText );
	static void							Announce							( unsigned char uc1, unsigned char uc2, char* szText );
	static void							AnnounceAll							( unsigned char uc, char* szText );

	static void							ServerData							( unsigned char uc );

	static void							RconList							( unsigned char uc );
	static void							PlayerList							( unsigned char uc );
	static void							VehicleList							( unsigned char uc );
	static void							PickupList							( unsigned char uc );
	static void							AmmuList							( unsigned char uc );

private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
