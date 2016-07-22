#ifndef _PLAYERPOOL_H
#define _PLAYERPOOL_H

#include "Main.h"
#include "Net.h"

class CPlayerPool : public CEntity
{
public:
	CPlayerPool( unsigned char uc );
	CPlayerPool() { };
	~CPlayerPool(void);

	unsigned char						GetID							( void )							{ return m_ucID; }

	char*								GetNick							( void )							{ return m_szNick; }
	void								SetNick							( const char* sz )						{ strncpy( m_szNick, sz, MAX_NAME_LEN ); }

	PlayerID							GetPlayerID						( void )							{ return m_PlayerID; }
	void								SetPlayerID						( PlayerID s )						{ m_PlayerID = s; }

	char*								GetIP							( void )							{ return m_szIP; }
	void								SetIP							( const char* sz )						{ strncpy( m_szIP, sz, 16 ); }

	short								GetPing							( void )							{ return m_sPing; }
	void								SetPing							( short s )							{ m_sPing = s; }

	unsigned char						GetHealth						( void )							{ return m_ucHealth; }
	void								SetHealth						( unsigned char uc )				{ m_ucHealth = uc; }

	unsigned char						GetArmour						( void )							{ return m_ucArmour; }
	void								SetArmour						( unsigned char uc )				{ m_ucArmour = uc; }

	unsigned char						GetClassID						( void )							{ return m_ucClass; }
	void								SetClassID						( unsigned char uc )				{ m_ucClass = uc; }

	int									GetScore						( void )							{ return m_iScore; }
	void								SetScore						( int i )							{ m_iScore = i; }
	void								IncScore						( void )							{ m_iScore++; }
	void								DecScore						( void )							{ m_iScore--; }

	unsigned char						GetTeam							( void );

	unsigned char						GetAction						( void )							{ return m_ucAction; }
	void								SetAction						( unsigned char uc )				{ m_ucAction = uc; }

	unsigned char						GetWeapon						( void )							{ return m_ucWeapon; }
	void								SetWeapon						( unsigned char uc )				{ m_ucWeapon = uc; }

	WORD								GetKeys							( void )							{ return m_wKeys; }
	void								SetKeys							( WORD w )							{ m_wKeys = w; }

	bool								IsAdmin							( void )							{ return m_bAdmin; }
	void								SetAdmin						( bool b )							{ m_bAdmin = b; }

	bool								GetFrozen						( void )							{ return m_bFrozen; }
	void								SetFrozen						( bool b )							{ m_bFrozen = b; }

	bool								GetMuted						( void )							{ return m_bMuted; }
	void								SetMuted						( bool b )							{ m_bMuted = b; }

	bool								GetSyncBlocked					( void )							{ return m_bSyncBlocked; }
	void								SetSyncBlocked					( bool b )							{ m_bSyncBlocked = b; }

	unsigned char						GetSpectating					( void )							{ return m_ucSpectating; }
	void								SetSpectating					( unsigned char uc )				{ m_ucSpectating = uc; }

	int									GetCash							( void )							{ return m_iCash; }
	void								SetCash							( int i )							{ m_iCash = i; }

	int									GetVehicle						( void )							{ return m_iVehicleID; }
	void								SetVehicle						( int i )							{ m_iVehicleID = i; }

	unsigned int						GetEnterVehicleTime				( void )							{ return m_uiEnterVehicleTime; }
	void								SetEnterVehicleTime				( unsigned int ui )					{ m_uiEnterVehicleTime = ui; }

	bool								GetIgnored						( BYTE b )							{ return m_bIgnored[ b ]; }
	void								SetIgnored						( BYTE bytePlayer, bool b )			{ m_bIgnored[ bytePlayer ] = b; }

	bool								GetSyncBlockedTo				( BYTE b )							{ return m_bSyncBlockedTo[ b ]; }
	void								SetSyncBlockedTo				( BYTE bytePlayer, bool b )			{ m_bSyncBlockedTo[ bytePlayer ] = b; }

	unsigned char						GetVehiclePassenger				( void )							{ return m_ucPassenger; }
	void								SetVehiclePassenger				( unsigned char uc )				{ m_ucPassenger = uc; }

	unsigned int						GetVehicleSeat					( void )							{ return m_uiSeat; }
	void								SetVehicleSeat					( unsigned int ui )				{ m_uiSeat = ui; }

	unsigned long						GetLastSyncTime					( unsigned char uc )				{ return m_ulLastSyncTime[ uc ]; }
	void								SetLastSyncTime					( unsigned char uc, unsigned long ul )	{ m_ulLastSyncTime[ uc ] = ul; }

private:
	unsigned char						m_ucID;

	char								m_szNick[ MAX_NAME_LEN ];
	char								m_szIP[ MAX_IP_LEN ];

	PlayerID							m_PlayerID;

	short								m_sPing;

	unsigned char						m_ucHealth;
	unsigned char						m_ucArmour;

	unsigned char						m_ucAction;
	unsigned char						m_ucWeapon;

	unsigned char						m_ucClass;

	int									m_iScore;

	WORD								m_wKeys;

	bool								m_bAdmin;
	bool								m_bFrozen;
	bool								m_bMuted;
	bool								m_bSyncBlocked;
	unsigned char						m_ucSpectating;

	int									m_iCash;

	int									m_iVehicleID;
	unsigned int						m_uiEnterVehicleTime;
	unsigned char						m_ucPassenger;
	unsigned int						m_uiSeat;

	bool								m_bIgnored[ MAX_PLAYERS ];
	bool								m_bSyncBlockedTo[ MAX_PLAYERS ];

	unsigned long						m_ulLastSyncTime[ MAX_PLAYERS ];
};

// The Manager
class CPlayerPoolManager
{
public:
	static CPlayerPool*					New								( unsigned char uc );

	static CPlayerPool*					Find							( unsigned char uc );
	static CPlayerPool*					Find							( const char* sz );
	static CPlayerPool*					FindExact						( const char* sz );
	static CPlayerPool*					Find							( PlayerID id );

	static bool							Remove							( CPlayerPool* p );
	static bool							Remove							( unsigned char uc );
	static bool							Remove							( const char* sz );
	static bool							Remove							( PlayerID id );

	static void							RemoveAll						( void );

	static unsigned char				Count							( void )							{ return m_ucPlayers; }

private:
	static unsigned char				m_ucPlayers;
	static CPlayerPool*					m_PlayerPool[ MAX_PLAYERS ];
};

#endif
