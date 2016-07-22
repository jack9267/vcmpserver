#ifndef _NET_H
#define _NET_H

#include "RakClient.h"
#include "RakNetworkFactory.h"
#include "BitStream.h"
#include "PacketEnumerations.h"
#include "StringCompressor.h"
#include "Main.h"
#include "Packets/SendPackets.h"
#include "Packets/RecvPackets.h"

class CNet
{
public:
	static void						Start						( void );
	static void						Stop						( void );

	static bool						IsConnected					( void )							{ return m_pRakClient->IsConnected(); }

	static bool						GetStarted					( void )							{ return m_bStarted; }

	static RakClientInterface*		GetInterface				( void )							{ return m_pRakClient; }

private:
	static RakClientInterface*		m_pRakClient;
	static HANDLE					m_hThread;
	static bool						m_bStarted;
};

#endif
