#ifndef _PLAYERPOOL_H
#define _PLAYERPOOL_H

#include "Main.h"
#include "Net.h"

class CPlayerPool : public CEntity
{
public:
	CPlayerPool( unsigned char uc );
	~CPlayerPool(void);

	unsigned char						GetID							( void )							{ return m_ucID; }

	char*								GetNick							( void )							{ return m_szNick; }
	void								SetNick							( char* sz )						{ strncpy( m_szNick, sz, MAX_NAME_LEN ); }

	PlayerID							GetPlayerID						( void )							{ return m_PlayerID; }
	void								SetPlayerID						( PlayerID s )						{ m_PlayerID = s; }

	char*								GetIP							( void )							{ return m_szIP; }
	void								SetIP							( char* sz )						{ strncpy( m_szIP, sz, 16 ); }

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

	unsigned char						GetTeam							( void )							{ return m_ucTeam; }
	void								SetTeam							( unsigned char uc )				{ m_ucTeam = uc; }

	unsigned char						GetSkin							( void )							{ return m_ucSkin; }
	void								SetSkin							( unsigned char uc )				{ m_ucSkin = uc; }

	unsigned char						GetAction						( void )							{ return m_ucAction; }
	void								SetAction						( unsigned char uc )				{ m_ucAction = uc; }

	unsigned char						GetWeapon						( void )							{ return m_ucWeapon; }
	void								SetWeapon						( unsigned char uc )				{ m_ucWeapon = uc; }

	bool								IsAdmin							( void )							{ return m_bAdmin; }
	void								SetAdmin						( bool b )							{ m_bAdmin = b; }

	bool								GetFrozen						( void )							{ return m_bFrozen; }
	void								SetFrozen						( bool b )							{ m_bFrozen = b; }

	bool								GetMuted						( void )							{ return m_bMuted; }
	void								SetMuted						( bool b )							{ m_bMuted = b; }

	unsigned char						GetSpectating					( void )							{ return m_bSpectating; }
	void								SetSpectating					( bool b )							{ m_bSpectating = b; }

	int									GetCash							( void )							{ return m_iCash; }
	void								SetCash							( int i )							{ m_iCash = i; }

	int									GetVehicle						( void )							{ return m_iVehicleID; }
	void								SetVehicle						( int i )							{ m_iVehicleID = i; }

	bool								GetIgnored						( BYTE b )							{ return m_bIgnored[ b ]; }
	void								SetIgnored						( BYTE bytePlayer, bool b )			{ m_bIgnored[ bytePlayer ] = b; }

	unsigned char						GetVehiclePassenger				( void )							{ return m_ucPassenger; }
	void								SetVehiclePassenger				( unsigned char uc )				{ m_ucPassenger = uc; }

private:
	unsigned char						m_ucID;

	char								m_szNick[ MAX_NAME_LEN ];
	char								m_szIP[ MAX_IP_LEN ];

	PlayerID							m_PlayerID;

	unsigned char						m_ucHealth;
	unsigned char						m_ucArmour;

	unsigned char						m_ucAction;
	unsigned char						m_ucWeapon;

	unsigned char						m_ucClass;

	int									m_iScore;

	unsigned char						m_ucTeam;
	unsigned char						m_ucSkin;

	bool								m_bAdmin;
	bool								m_bFrozen;
	bool								m_bMuted;
	bool								m_bSpectating;

	int									m_iCash;

	int									m_iVehicleID;
	unsigned char						m_ucPassenger;

	bool								m_bIgnored[ MAX_PLAYERS ];
};

// The Manager
class CPlayerPoolManager
{
public:
	static CPlayerPool*					New								( unsigned char uc );

	static CPlayerPool*					Find							( unsigned char uc );
	static CPlayerPool*					Find							( char* sz );
	static CPlayerPool*					FindExact						( char* sz );
	static CPlayerPool*					Find							( PlayerID id );

	static bool							Remove							( CPlayerPool* p );
	static bool							Remove							( unsigned char uc );
	static bool							Remove							( char* sz );
	static bool							Remove							( PlayerID id );

	static void							RemoveAll						( void );

	static unsigned char				Count							( void )							{ return m_ucPlayers; }

private:
	static unsigned char				m_ucPlayers;
	static CPlayerPool*					m_PlayerPool[ MAX_PLAYERS ];
};

#endif
