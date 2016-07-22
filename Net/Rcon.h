#ifndef _RCON_H
#define _RCON_H

#include "RakNetworkFactory.h"
#include "RakServerInterface.h"
#include "PacketEnumerations.h"
#include "Main.h"
#include "StringCompressor.h"
#include "RecvPackets.h"
#include "SendPackets.h"

class CRcon
{
public:
	static bool							Start							( void );
	static void							Stop							( void );

	static void							GetIP							( char szIP[22], unsigned char bID );

	static RakServerInterface*			GetInterface					( void )								{ return m_pRak; }

	static void							Listen							( void );

private:
	static RakServerInterface*			m_pRak;
};

#endif