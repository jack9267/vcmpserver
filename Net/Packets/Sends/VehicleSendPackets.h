#ifndef _VEHICLESENDPACKETS_H
#define _VEHICLESENDPACKETS_H

#include "Rcon.h"

class CVehicleSendPackets
{
public:
	static void							VehicleSync							( unsigned char uc1, unsigned char uc2, INCAR_SYNC_DATA icSync );
private:
	static void							PreparePacket						( unsigned char ucPacket, RakNet::BitStream &bs );
};

#endif
