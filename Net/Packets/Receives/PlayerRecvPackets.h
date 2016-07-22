#ifndef _PLAYERRECVPACKETS_H
#define _PLAYERRECVPACKETS_H

#include "RconPool.h"
#include "NetSends.h"
#include "Pools.h"

class CPlayerRecvPackets
{
public:
	static void						Parse					( Packet* pPacket );

private:
	static void						SetPosition				( void );
	static void						SetHealth				( void );
	static void						SetArmour				( void );
	static void						SetFrozen				( void );
	static void						SetWeapon				( void );
	static void						SetCash					( void );
	static void						RemoveMarker			( void );
	static void						PutInVehicle			( void );
	static void						SetPlayerMarker			( void );
	static void						DoAnim					( void );
	static void						SetScore				( void );
	static void						IncScore				( void );
	static void						DecScore				( void );
	static void						CreatePickup			( void );
	static void						RespawnPickup			( void );
	static void						DeletePickup			( void );
	static void						SetMute					( void );
	static void						SetIgnore				( void );
	static void						SetLocalMarker			( void );
	static void						RemoveLocalMarker		( void );

	static RakNet::BitStream		bs;
	static Packet*					m_pPacket;
};

#endif
