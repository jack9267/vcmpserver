#ifndef _CONFIGLOADER_H
#define _CONFIGLOADER_H

class CConfig
{
public:
	static void						SetIP							( char* sz )			{ strcpy(m_szIP, sz); }
	static char*					GetIP							( void )				{ return m_szIP; };

	static void						SetPort							( unsigned short us )	{ m_usPort = us; }
	static unsigned short			GetPort							( void )				{ return m_usPort; };

	static void						SetPassword						( char* sz )			{ strcpy(m_szPassword, sz); }
	static char*					GetPassword						( void )				{ return m_szPassword; };

	static void						SetNickname						( char* sz )			{ strcpy(m_szNick, sz); }
	static char*					GetNickname						( void )				{ return m_szNick; };

private:
	static char						m_szIP[ MAX_IP_LEN ];
	static unsigned short			m_usPort;
	static char						m_szPassword[ MAX_PASS_LEN ];
	static char						m_szNick[ MAX_NAME_LEN ];
};

class CGameSettings
{
public:
	static unsigned char			GetWeather				( void )				{ return m_ucWeather; };
	static void						SetWeather				( unsigned char uc )	{ m_ucWeather = uc; }

	static unsigned char			GetHour					( void )				{ return m_ucHour; };
	static void						SetHour					( unsigned char uc )	{ m_ucHour = uc; }

	static unsigned char			GetMinute				( void )				{ return m_ucMinute; };
	static void						SetMinute				( unsigned char uc )	{ m_ucMinute = uc; }

	static unsigned char			GetTimeRate				( void )				{ return m_ucRate; };
	static void						SetTimeRate				( unsigned char uc )	{ m_ucRate = uc; }

	static unsigned int				GetWeatherRate			( void )				{ return m_ucWeatherRate; };
	static void						SetWeatherRate			( unsigned char uc )	{ m_ucWeatherRate = uc; }

	static bool						GetScoreBoard			( void )				{ return m_bScoreBoard; };
	static void						SetScoreBoard			( bool b )				{ m_bScoreBoard = b; }

	static bool						GetDrivebyEnabled		( void )				{ return m_bDriveby; }
	static void						SetDrivebyEnabled		( bool bDriveby )		{ m_bDriveby = bDriveby; }

	static char*					GetPassword				( void )				{ return m_szPass; };
	static void						SetPassword				( char* sz )			{ strncpy( m_szPass, sz, MAX_PASS_LEN ); }

	static char*					GetServerName			( void )				{ return m_szServerName; };
	static void						SetServerName			( char* sz )			{ strncpy( m_szServerName, sz, MAX_SERVNAME_LEN ); }

	static char*					GetServerMode			( void )				{ return m_szServerMode; };
	static void						SetServerMode			( char* sz )			{ strncpy( m_szServerMode, sz, MAX_GAMEMODE_LEN ); }

	static unsigned char			GetMaxPlayers			( void )				{ return m_ucMaxPlayers; };
	static void						SetMaxPlayers			( unsigned int uc )		{ m_ucMaxPlayers = uc; };

	static void						TimeUpdate				( void );

private:
	static unsigned char			m_ucWeather;
	static unsigned char			m_ucHour;
	static unsigned char			m_ucMinute;
	static unsigned char			m_ucRate;
	static unsigned char			m_ucWeatherRate;

	static bool						m_bScoreBoard;
	static bool						m_bDriveby;

	static char						m_szPass[ MAX_PASS_LEN ];
	static char						m_szServerName[ MAX_SERVNAME_LEN ];
	static char						m_szServerMode[ MAX_GAMEMODE_LEN ];

	static unsigned char			m_ucMaxPlayers;

	static unsigned int				m_uiLastTimeSync;
};

#endif