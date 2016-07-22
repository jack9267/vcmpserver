#ifndef _SQUIRRELOBJECTS_H
#define _SQUIRRELOBJECTS_H

#include <list>
#include "SquirrelTimers.h"
#include "SquirrelSockets.h"

class CSquirrelObjects
{
public:
	CSquirrelObjects( void ) { }
	~CSquirrelObjects(void);

	void									AddTimer							( CSquirrelTimers* p );
	bool									RemoveTimer							( CSquirrelTimers* p );
	void									RemoveAllTimers						( void );
	unsigned short							CountTimers							( void )							{ return static_cast< unsigned short >( m_Timers.size() ); }

	void									AddSocket							( CSquirrelSockets* p );
	bool									RemoveSocket						( CSquirrelSockets* p );
	void									RemoveAllSockets					( void );
	unsigned short							CountSockets						( void )							{ return static_cast< unsigned short >( m_Sockets.size() ); }

private:
	std::list< CSquirrelTimers* >			m_Timers;
	std::list< CSquirrelSockets* >			m_Sockets;
};

#endif
