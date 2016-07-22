#ifndef _CONFIGLOADER_H
#define _CONFIGLOADER_H

#include <tinyxml.h>
#include "Main.h"

class CConfigLoader
{
public:
	static void						Initialize						( const char* szConfig );
	static void						Destroy							( void );
	static void						LoadBans						( void );
	static void						LoadSettings					( void );
	static void						LoadClasses						( void );
	static void						LoadPickups						( void );
	static void						LoadVehicles					( void );
	static void						LoadAmmunations					( void );
	static void						LoadSquirrelScripts				( void );
	static void						LoadVehicleColours				( void );

	static unsigned char			GetMaxPlayers					( void )			{ return m_ucMaxPlayers; };
	static void						SetMaxPlayers					( unsigned int uc )	{ m_ucMaxPlayers = uc; };

	static unsigned short			GetListenPort					( void )			{ return m_usListenPort; };

	static char*					GetServerName					( void )			{ return m_szServerName; };
	static void						SetServerName					( const char* sz )	{ strncpy( m_szServerName, sz, MAX_SERVNAME_LEN ); };

	static unsigned char			GetMap							( void )			{ return m_ucMap; };

	static char*					GetPassword						( void )			{ return m_szPassword; };
	static void						SetPassword						( const char* sz )	{ strncpy( m_szPassword, sz, MAX_PASS_LEN ); }

	static char*					GetRconPassword					( void )			{ return m_szAdminPassword; };

	static char*					GetGamemodeName					( void )			{ return m_szGamemode; };
	static void						SetGamemodeName					( const char* sz )	{ strncpy( m_szGamemode, sz, MAX_GAMEMODE_LEN ); };

	static bool						GetFriendlyFire					( void )			{ return m_bFriendlyFire; };

	static unsigned char			GetShowOnRadar					( void )			{ return m_ucShowOnRadar; };

	static unsigned short			GetRconPort						( void )			{ return m_usRconPort; };

	static unsigned short			GetRconMaxUsers					( void )			{ return m_usRconMaxUsers; };

	static unsigned char			GetWeather						( void )			{ return m_ucWeather; };
	static void						SetWeather						( unsigned char uc )	{ m_ucWeather = uc; }

	static unsigned char			GetHour							( void )			{ return m_ucHour; };
	static void						SetHour							( unsigned char uc )	{ m_ucHour = uc; }

	static unsigned char			GetMinute						( void )			{ return m_ucMinute; };
	static void						SetMinute						( unsigned char uc )	{ m_ucMinute = uc; }

	static unsigned char			GetTimeRate						( void )			{ return m_ucRate; };
	static void						SetTimeRate						( unsigned char uc )	{ m_ucRate = uc; }

	static unsigned int				GetWeatherRate					( void )			{ return m_uiWeatherRate; };
	static void						SetWeatherRate					( unsigned int ui )	{ m_uiWeatherRate = ui; }

	static bool						GetWeatherLock					( void )			{ return m_bWeatherLock; };
	static void						SetWeatherLock					( bool b )			{ m_bWeatherLock = b; }

	static unsigned int				GetPickupRespawnTime			( void )			{ return m_uiPickupRespawnTime; }
	static void						SetPickupRespawnTime			( unsigned int ui ) { m_uiPickupRespawnTime = ui; }

	static bool						GetOutputConsole				( void )			{ return m_bOutputConsole; }
	static bool						GetDeathmatchScoreB				( void )			{ return m_bScore; }
	static void						SetDeathmatchScoreB				( bool bScore )		{ m_bScore = bScore; }

	static bool						GetDrivebyEnabled				( void )			{ return m_bDriveby; }
	static void						SetDrivebyEnabled				( bool bDriveby )	{ m_bDriveby = bDriveby; }

	static VECTOR					GetInitPlayerPos				( void )			{ return m_vecInitPlayerPos; }
	static VECTOR					GetInitCameraPos				( void )			{ return m_vecInitCameraPos; }
	static VECTOR					GetInitCameraLook				( void )			{ return m_vecInitCameraLook; }

	static float					GetWorldBounds					( unsigned char uc ) { return m_WorldBounds[ uc ]; }

private:
	static char*					m_szConfig;
	static TiXmlDocument*			m_xmlDoc;

	static unsigned char			m_ucMaxPlayers;
	static unsigned short			m_usListenPort;
	static char						m_szServerName[ MAX_SERVNAME_LEN ];
	static unsigned char			m_ucMap;

	static char						m_szPassword[ MAX_PASS_LEN ];
	static char						m_szAdminPassword[ MAX_PASS_LEN ];
	static char						m_szGamemode[ MAX_GAMEMODE_LEN ];

	static bool						m_bFriendlyFire;
	static unsigned char			m_ucShowOnRadar;

	static unsigned short			m_usRconPort;
	static unsigned short			m_usRconMaxUsers;

	static unsigned char			m_ucWeather;
	static unsigned char			m_ucHour;
	static unsigned char			m_ucMinute;
	static unsigned char			m_ucRate;
	static unsigned int				m_uiWeatherRate;
	static bool						m_bWeatherLock;

	static unsigned int				m_uiPickupRespawnTime;

	static bool						m_bOutputConsole;
	static bool						m_bScore;
	static bool						m_bDriveby;

	static VECTOR					m_vecInitPlayerPos;
	static VECTOR					m_vecInitCameraPos;
	static VECTOR					m_vecInitCameraLook;

	static float					m_WorldBounds[ 4 ];
};

#endif
