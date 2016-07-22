#ifndef _PLAYERPOOL_H
#define _PLAYERPOOL_H

#include "Main.h"
#include <vector>

class CPlayerPool
{
public:
	CPlayerPool( unsigned char uc );
	~CPlayerPool(void);

	unsigned char						GetID							( void )						{ return m_ucID; }

	char*								GetNick							( void )						{ return m_szNick; }
	void								SetNick							( char* sz )					{ strcpy( m_szNick, sz ); }

private:
	unsigned char						m_ucID;

	char								m_szNick[ MAX_NAME_LEN ];
};

// The Manager
class CPlayerPoolManager
{
public:
	static CPlayerPool*					New								( unsigned char uc );

	static CPlayerPool*					Find							( unsigned char uc );
	static CPlayerPool*					Find							( char* sz );
	static CPlayerPool*					FindExact						( char* sz );

	static bool							Remove							( CPlayerPool* p );
	static bool							Remove							( unsigned char uc );
	static bool							Remove							( char* sz );

	static void							RemoveAll						( void );

	static unsigned char				Count							( void )							{ return m_ucPlayers; }

private:
	static unsigned char				m_ucPlayers;
	static std::vector< CPlayerPool* >	m_PlayerPool;
};

#endif
