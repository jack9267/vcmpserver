#ifndef _SQUIRRELTIMERS_H
#define _SQUIRRELTIMERS_H

#include <vector>

#ifndef MAX_TIMERS
	#define MAX_TIMERS 50
#endif

#ifndef MAX_TIMER_PARAMS
	#define MAX_TIMER_PARAMS 25
#endif

union pParams
{
	int i;
	float f;
	void* p;
	char sz[ 256 ];
};

enum enumTypes
{
	eNone,
	eInt,
	eFloat,
	eUserData,
	eString,
	ePlayer,
	eVehicle,
	ePickup,
	eSocket,
};

class CPlayerPool;
class CVehiclePool;
class CPickupPool;
class CSquirrelSockets;

class CSquirrelTimers
{
public:
	CSquirrelTimers( unsigned int ui );
	~CSquirrelTimers(void);

	unsigned int							GetID					( void )					{ return m_uiID; }

	unsigned int							GetInterval				( void )					{ return m_uiInterval; }
	void									SetInterval				( unsigned int ui );

	unsigned int							GetLoopTimes			( void )					{ return m_uiTimesToLoop; }
	void									SetLoopTimes			( unsigned int ui )			{ m_uiTimesToLoop = ui; }

	bool									GetStarted				( void )					{ return m_bStarted; }

	void									Start					( void )					{ m_bStarted = true; }
	void									Stop					( void )					{ m_bStarted = false; }

	int										GetParentScript			( void )					{ return m_iParentScript; }
	void									SetParentScript			( int i )					{ m_iParentScript = i; }

	// Functions
	void									SetFunction				( char* sz )				{ strncpy( m_szFunction, sz, 128 ); }
	void									PushInteger				( int i );
	void									PushFloat				( float f );
	void									PushUserData			( void* p );
	void									PushString				( char* sz );

	void									PushPlayerPointer		( CPlayerPool* p );
	void									PushVehiclePointer		( CVehiclePool* p );
	void									PushPickupPointer		( CPickupPool* p );
	void									PushSocketPointer		( CSquirrelSockets* p );

	void									Process					( void );

private:
	unsigned int							m_uiID;

	unsigned int							m_uiTime;
	unsigned int							m_uiInterval;
	unsigned int							m_uiTimesToLoop;
	unsigned int							m_uiLooped;
	bool									m_bStarted;

	char									m_szFunction[ 128 ];

	std::vector< pParams >					m_pParams;
	std::vector< enumTypes >				m_pTypes;
	unsigned int							m_iParams;

	int										m_iParentScript;
};

class CSquirrelTimerManager
{
public:
	static CSquirrelTimers*					New						( void );
	static CSquirrelTimers*					New						( unsigned int ui );

	static CSquirrelTimers*					Find					( unsigned int ui );

	static bool								Remove					( CSquirrelTimers* p );
	static bool								Remove					( unsigned int ui );

	static void								RemoveAll				( void );

	static unsigned int						FindFreeID				( void );

	static unsigned int						Count					( void )					{ return m_uiTimers; }

	// Functions
	static void								ProcessTimers			( void );

private:
	static unsigned int						m_uiTimers;
	static CSquirrelTimers*					m_Timers[ MAX_TIMERS ];
};

#endif
