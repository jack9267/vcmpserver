#ifndef _NET_H
#define _NET_H

#include "RakServer.h"
#include "RakNetworkFactory.h"
#include "BitStream.h"
#include "PacketEnumerations.h"
#include "Main.h"

class CNet
{
public:
	static bool							Start							( void );
	static void							Stop							( void );

	static void							Disconnected					( const char* szText, ... );

	static unsigned char				GetPacketID						( Packet *pPacket );
	static void							GetIP							( char szIP[22], unsigned char bID );

	static RakServerInterface*			GetInterface					( void )								{ return m_pRakServer; }

	static void							Listen							( void );

	static void							ProcessPings					( void );
	static void							Announce						( void );

private:
	static RakServerInterface*			m_pRakServer;
	static unsigned int					iNextPingUpdate;
	static unsigned int					iNextAnnounceTime;
};

#endif
