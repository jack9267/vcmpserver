#ifndef _RCONPOOL_H
#define _RCONPOOL_H

#include "Main.h"
#include "Net.h"

class CRconPool : public CEntity
{
public:
	CRconPool( unsigned char uc );
	~CRconPool(void);

	unsigned char						GetID							( void )							{ return m_ucID; }
	void								SetID							( unsigned char uc )				{ m_ucID = uc; }

	char*								GetNick							( void )							{ return m_szNick; }
	void								SetNick							( char* sz )						{ strncpy( m_szNick, sz, MAX_NAME_LEN ); }

	PlayerID							GetConnectionID					( void )							{ return m_ConnectionID; }
	void								SetConnectionID					( PlayerID s )						{ m_ConnectionID = s; }

	char*								GetIP							( void )							{ return m_szIP; }
	void								SetIP							( char* sz )						{ strncpy( m_szIP, sz, 16 ); }

private:
	unsigned char						m_ucID;

	char								m_szNick[ MAX_NAME_LEN ];
	char								m_szIP[ MAX_IP_LEN ];

	PlayerID							m_ConnectionID;
};

// The Manager
class CRconPoolManager
{
public:
	static CRconPool*					New								( unsigned char uc );

	static CRconPool*					Find							( unsigned char uc );
	static CRconPool*					Find							( char* sz );
	static CRconPool*					FindExact						( char* sz );
	static CRconPool*					Find							( PlayerID id );

	static bool							Remove							( CRconPool* p );
	static bool							Remove							( unsigned char uc );
	static bool							Remove							( char* sz );
	static bool							Remove							( PlayerID id );

	static void							RemoveAll						( void );

	static unsigned char				Count							( void )							{ return m_ucConns; }

private:
	static unsigned char				m_ucConns;
	static CRconPool*					m_RconPool[ MAX_PLAYERS ];
};

#endif
