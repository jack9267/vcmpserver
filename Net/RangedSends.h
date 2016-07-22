#ifndef _RANGEDSENDS_H
#define _RANGEDSENDS_H

#include "Main.h"
#include "Pools.h"
#include "GetTime.h"

class CRangedSends
{
public:
	static void							LPUR						( CPlayerPool* pPlayer, RakNet::BitStream* bs );
	static void							MPUR						( CPlayerPool* pPlayer, RakNet::BitStream* bs );
	static void							HPUR						( CPlayerPool* pPlayer, RakNet::BitStream* bs );
};

#endif
