#ifndef _PLAYERSENDPACKETS_H
#define _PLAYERSENDPACKETS_H

#include "Rcon.h"

class CPlayerSendPackets
{
public:
	static void							Chat								( unsigned char uc, char* szText );
	static void							Join								( unsigned char uc, char* szNick, char* szIP );
	static void							Part								( unsigned char uc1, unsigned char uc2 );
	static void							Action								( unsigned char uc, char* szText );
	static void							PM									( unsigned char uc1, unsigned char uc2, unsigned char uc3, char* szText );
	static void							TeamChat							( unsigned char uc1, unsigned char uc2, char* szText );
	static void							Spawn								( unsigned char uc1, unsigned char uc2, unsigned char uc3 );
	static void							EnterVehicle						( unsigned char uc1, unsigned char uc2, unsigned char uc3, float fHealth, unsigned char uc4 );
	static void							ExitVehicle							( unsigned char uc1, unsigned char uc2, unsigned char uc3, float fHealth );
	static void							Kill								( unsigned char uc1, unsigned char uc2, unsigned char uc3, unsigned char uc4, unsigned char uc5);
	static void							TeamKill							( unsigned char uc1, unsigned char uc2, unsigned char uc3, unsigned char uc4, unsigned char uc5);
	static void							Death								( unsigned char uc1, unsigned char uc2 );
	static void							Command								( unsigned char uc, char* szText );
	static void							DoAnim								( unsigned char uc1, unsigned char uc2 );
	static void							RequestAmmuWeapon					( unsigned char uc1, unsigned char uc2, unsigned char uc3 );
	static void							RequestAmmunation					( unsigned char uc1, unsigned char uc2 );
	static void							RequestClass						( unsigned char uc1, unsigned char uc2, unsigned char uc3, unsigned char uc4 );
	static void							StartSpectating						( unsigned char uc );
	static void							StopSpectating						( unsigned char uc );

	// Forwarded player sync packet
	static void							FootSync							( unsigned char uc, ONFOOT_SYNC_DATA ofSync );

private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
