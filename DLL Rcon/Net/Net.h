#ifndef _RCON_H
#define _RCON_H

#include "RakClient.h"
#include "RakNetworkFactory.h"
#include "BitStream.h"
#include "PacketEnumerations.h"
#include "GetTime.h"
#include "StringCompressor.h"
#include "Main.h"
#include "RecvPackets.h"
#include "SendPackets.h"

class CNet
{
public:
	static void						Start						( void );
	static void						Stop						( void );
	static void						ClearPools					( void );

	static bool						IsConnected					( void )				{ return m_pRak->IsConnected(); }

	static RakClientInterface*		GetInterface				( void )				{ return m_pRak; }

	static void						Listen						( void );

private:
	static RakClientInterface*		m_pRak;
};

#endif