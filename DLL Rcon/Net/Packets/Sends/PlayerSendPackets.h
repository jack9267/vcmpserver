#ifndef _PLAYERSENDPACKETS_H
#define _PLAYERSENDPACKETS_H

#include "Net.h"

class CPlayerSendPackets
{
public:
	static void							SetPlayerPosition					( unsigned char uc1, float fX, float fY, float fZ, unsigned char uc2 );
	static void							SetPlayerHealth						( unsigned char uc1, unsigned char uc2 );
	static void							SetPlayerArmour						( unsigned char uc1, unsigned char uc2 );
	static void							SetPlayerFrozen						( unsigned char uc );
	static void							SetPlayerWeapon						( unsigned char uc, unsigned char uc2, unsigned int ui );
	static void							SetPlayerCash						( unsigned char uc );
	static void							SetPlayerMute						( unsigned char uc );
	static void							SetPlayerIgnore						( unsigned char uc, unsigned char uc2 );
	static void							SetPlayerScore						( unsigned char uc, int iScore );
	static void							IncPlayerScore						( unsigned char uc );
	static void							DecPlayerScore						( unsigned char uc );
	static void							SetPlayerAnim						( unsigned char uc1, unsigned char uc2 );
	static void							SetPlayerMarker						( unsigned char uc1, unsigned char uc2 );
	static void							RemovePlayerMarker					( unsigned char uc );
	static void							PutInVehicle						( unsigned char uc1, unsigned char uc2 );
	static void							SetLocalMarker						( unsigned char uc1, unsigned char uc2, unsigned char uc3 );
	static void							RemoveLocalMarker					( unsigned char uc1, unsigned char uc2 );
private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
