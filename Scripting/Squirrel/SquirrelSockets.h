#ifndef _SQUIRRELSOCKETS_H
#define _SQUIRRELSOCKETS_H

#include <string.h>
#include "NetworkTypes.h"

class TCPInterface;
class CSquirrelSocketConnectionsMgr;

#ifndef MAX_SOCKETS
	#define MAX_SOCKETS 50
#endif

class CSquirrelSockets
{
public:
	CSquirrelSockets( unsigned char uc );
	~CSquirrelSockets(void);

	unsigned char							GetID					( void )					{ return m_ucID; }

	bool									GetStarted				( void )					{ return m_bStarted; }
	bool									IsServer				( void )					{ return m_bServer; }
	TCPInterface*							GetInterface			( void )					{ return m_pInterface; }

	int										GetParentScript			( void )					{ return m_iParentScript; }
	void									SetParentScript			( int i )					{ m_iParentScript = i; }

	/// \summary Connects to a TCP server
	bool									Connect					( const char* szHost, unsigned short usPort );

	/// \summary Starts a TCP server
	bool									Start					( unsigned short usPort, unsigned short usMaxConns );
	/// \summary Kills the TCP server
	void									Stop					( void );

	void									Send					( char* sz, unsigned char ucConn = 255 );

	void									SetFunction				( char* sz )				{ strncpy( m_szFunction, sz, 128 ); }
	void									SetNewConnFunction		( char* sz )				{ strncpy( m_szNewConnFunc, sz, 128 ); }
	void									SetLostConnFunction		( char* sz )				{ strncpy( m_szLostConnFunc, sz, 128 ); }

	void									Process					( void );

private:
	unsigned char							m_ucID;

	bool									m_bStarted;
	bool									m_bServer;
	TCPInterface*							m_pInterface;
	CSquirrelSocketConnectionsMgr*			m_pConnMgr;
	PlayerID								m_playerId;

	int										m_iParentScript;

	char									m_szFunction[ 128 ];
	char									m_szNewConnFunc[ 128 ];
	char									m_szLostConnFunc[ 128 ];
};

class CSquirrelSocketManager
{
public:
	static CSquirrelSockets*				New						( void );
	static CSquirrelSockets*				New						( unsigned char uc );

	static CSquirrelSockets*				Find					( unsigned char uc );

	static bool								Remove					( CSquirrelSockets* p );
	static bool								Remove					( unsigned char uc );

	static void								RemoveAll				( void );

	static unsigned char					FindFreeID				( void );

	static unsigned char					Count					( void )					{ return m_ucSockets; }

	// Functions
	static void								ProcessSockets			( void );

private:
	static unsigned char					m_ucSockets;
	static CSquirrelSockets*				m_Sockets[ MAX_SOCKETS ];
};

class CSquirrelSocketConnectionsMgr
{
public:
	CSquirrelSocketConnectionsMgr();
	~CSquirrelSocketConnectionsMgr() {};

	unsigned char					New						( PlayerID pID );

	unsigned char					Find					( PlayerID pID );
	PlayerID						Find					( unsigned char uc );

	bool							Remove					( PlayerID pID );
	bool							Remove					( unsigned char uc );

	void							RemoveAll				( void );

	unsigned char					FindFreeID				( void );

	unsigned char					Count					( void )					{ return m_ucConnections; }

private:
	unsigned char					m_ucConnections;
	PlayerID						m_Connections[ 128 ];
};

#endif
