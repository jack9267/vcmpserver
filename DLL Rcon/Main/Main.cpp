#include "Main.h"
#include "ConfigLoader.h"
#include "Net.h"
#include "Utils.h"

#ifndef _VCMP_DLL
#define _VCMP_DLL(x) extern "C" int WINAPI x( HWND mWnd, HWND aWnd, char* data, char*, BOOL, BOOL )
#endif

HANDLE hFileMap = NULL;
LPSTR mData;
HWND _mWnd, _aWnd;
HANDLE hThread = NULL;

unsigned __stdcall NetThread( void* args );

bool bRunning = false;

char CConfig::m_szIP [ MAX_IP_LEN ] = { "127.0.0.1" };
unsigned short CConfig::m_usPort = 5315;
char CConfig::m_szPassword[ MAX_PASS_LEN ] = { "none" };
char CConfig::m_szNick[ MAX_NAME_LEN ] = { "Unknown" };

//--------------------------------------------------------------------------------//

_VCMP_DLL( Connect )
{
	char szData [ 512 ] = { 0 };
	strcpy(szData, data);
	strcpy(data, "0");

	if (szData && Utils::NumTok(szData, " ") == 4)
	{
		char szIP [ 128 ] = { 0 };
		char szPass[ MAX_PASS_LEN ] = { 0 };
		char szNick[ MAX_NAME_LEN ] = { 0 };

		strcpy(szIP, strtok(szData, " "));
		unsigned short usPort = static_cast< unsigned short >(atoi(strtok(0, " ")));
		strcpy(szPass, strtok(0, " "));
		strcpy(szNick, strtok(0, " "));

		if ((CNet::GetInterface()) && (!CNet::IsConnected())) bRunning = false;

		if (!bRunning)
		{
			CConfig::SetIP( szIP );
			CConfig::SetPort( usPort );
			CConfig::SetPassword( szPass );
			CConfig::SetNickname( szNick );

			bRunning = true;

			unsigned int ThreadId = 0;
			hThread = (HANDLE)_beginthreadex(NULL, 0, NetThread, NULL, 0, &ThreadId);

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( Disconnect )
{
	char szData [ 512 ] = { 0 };
	strcpy(szData, data);
	strcpy(data, "0");

	if ((CNet::GetInterface()) && (CNet::IsConnected()))
	{
		bRunning = false;
		CallSignal("vcmp.server.disconnected", "0");

		CRconPoolManager::RemoveAll();
		CPlayerPoolManager::RemoveAll();
		CPickupPoolManager::RemoveAll();
		CVehiclePoolManager::RemoveAll();
		CAmmuPoolManager::RemoveAll();

		strcpy(data, "1");
	}

	return 3;
}

_VCMP_DLL( Connected )
{
	char szData [ 512 ] = { 0 };
	strcpy(szData, data);
	strcpy(data, "0");

	if ((CNet::GetInterface()) && (CNet::IsConnected()))
	{
		strcpy(data, "1");
	}

	return 3;
}

//--------------------------------------------------------------------------------//
// Misc Stuff

_VCMP_DLL( Message )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") >= 1)
	{
		if (CNet::GetInterface())
		{
			CMiscSendPackets::AdminChat(szData);

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( MessageAllExcept )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer [ 8 ] = { 0 };
	char szMessage [ 512 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") >= 2)
	{
		strcpy(szMessage, szData);
		strcpy(szPlayer, strtok(szData, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) )
		{
			BYTE bPlayer = static_cast< BYTE >( atoi( szPlayer ) );
			strncpy( szMessage, szMessage + strlen( szPlayer ) + 1, strlen( szMessage ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( bPlayer );
			if ( pPlayer )
			{
				CMiscSendPackets::MessageAllExcept(bPlayer, szMessage);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( MessagePlayer )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer [ 8 ] = { 0 };
	char szMessage [ 512 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") >= 2)
	{
		strcpy(szMessage, szData);
		strcpy(szPlayer, strtok(szData, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) )
		{
			BYTE bPlayer = static_cast< BYTE >( atoi( szPlayer ) );
			strncpy( szMessage, szMessage + strlen( szPlayer ) + 1, strlen( szMessage ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( bPlayer );
			if ( pPlayer )
			{
				CMiscSendPackets::MessagePlayer(bPlayer, szMessage);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( PrivMessage )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer [ 8 ] = { 0 };
	char szMessage [ 512 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") >= 2)
	{
		strcpy(szMessage, szData);
		strcpy(szPlayer, strtok(szData, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) )
		{
			BYTE bPlayer = static_cast< BYTE >( atoi( szPlayer ) );
			strncpy( szMessage, szMessage + strlen( szPlayer ) + 1, strlen( szMessage ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( bPlayer );
			if ( pPlayer )
			{
				CMiscSendPackets::AdminPM(bPlayer, szMessage);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( Announce )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer [ 8 ] = { 0 };
	char szMessage [ 512 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") >= 2)
	{
		strcpy(szMessage, szData);
		strcpy(szPlayer, strtok(szData, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) )
		{
			BYTE bPlayer = static_cast< BYTE >( atoi( szPlayer ) );
			strncpy( szMessage, szMessage + strlen( szPlayer ) + 1, strlen( szMessage ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( bPlayer );
			if ( pPlayer )
			{
				CMiscSendPackets::Announce(bPlayer, szMessage);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( AnnounceAll )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") >= 1)
	{
		if (CNet::GetInterface())
		{
			CMiscSendPackets::AnnounceAll(szData);

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( KickPlayer )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CMiscSendPackets::KickPlayer( ucPlayer );
				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( BanPlayer )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CMiscSendPackets::BanPlayer( ucPlayer );
				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( BanIP )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (strlen(szData) < MAX_IP_LEN)
		{
			CMiscSendPackets::BanIP( szData );
		}
	}

	return 3;
}

_VCMP_DLL( UnbanIP )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (strlen(szData) < MAX_IP_LEN)
		{
			CMiscSendPackets::UnbanIP( szData );
		}
	}

	return 3;
}

_VCMP_DLL( GetPassword )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%s", CGameSettings::GetPassword());

	return 3;
}

_VCMP_DLL( SetPassword )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		CGameSettings::SetPassword( szData );
		CMiscSendPackets::SetPassword( szData );
		strcpy(data, "1");
	}

	return 3;
}

_VCMP_DLL( GetServerName )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%s", CGameSettings::GetServerName());

	return 3;
}

_VCMP_DLL( SetServerName )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		CGameSettings::SetServerName( szData );
		CMiscSendPackets::SetServerName( szData );
		strcpy(data, "1");
	}

	return 3;
}

_VCMP_DLL( GetGamemodeName )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%s", CGameSettings::GetServerMode());

	return 3;
}

_VCMP_DLL( SetGamemodeName )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		CGameSettings::SetServerMode( szData );
		CMiscSendPackets::SetGamemodeName( szData );
		strcpy(data, "1");
	}

	return 3;
}

_VCMP_DLL( GetMaxPlayers )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%i", CGameSettings::GetMaxPlayers());

	return 3;
}

_VCMP_DLL( SetMaxPlayers )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CGameSettings::SetMaxPlayers( uc );
			CMiscSendPackets::SetMaxPlayers( uc );
			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayers )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if ( (CNet::GetInterface()) && (CNet::IsConnected()) )
	{
		sprintf(data, "%i", CPlayerPoolManager::Count());
	}

	return 3;
}

_VCMP_DLL( GetDeathmatchScoreboard )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if ( (CNet::GetInterface()) && (CNet::IsConnected()) )
	{
		if ( CGameSettings::GetScoreBoard() ) strcpy(data, "1");
	}

	return 3;
}

_VCMP_DLL( SetDeathmatchScoreboard )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			if ((!strcmp(szData, "1")) || (!strcmp(szData, "yes")) || (!strcmp(szData, "true")) || (!strcmp(szData, "on"))) CGameSettings::SetScoreBoard(true);
			else CGameSettings::SetScoreBoard(false);

			CMiscSendPackets::SetScoreBoard( CGameSettings::GetScoreBoard() );

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( GetDrivebyEnabled )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if ( (CNet::GetInterface()) && (CNet::IsConnected()) )
	{
		if ( CGameSettings::GetDrivebyEnabled() ) strcpy(data, "1");
	}

	return 3;
}

_VCMP_DLL( SetDrivebyEnabled )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			if ((!strcmp(szData, "1")) || (!strcmp(szData, "yes")) || (!strcmp(szData, "true")) || (!strcmp(szData, "on"))) CGameSettings::SetDrivebyEnabled( true );
			else CGameSettings::SetDrivebyEnabled( false );

			CMiscSendPackets::SetDriveBy( CGameSettings::GetDrivebyEnabled() );

			strcpy(data, "1");
		}
	}

	return 3;
}

//--------------------------------------------------------------------------------//
// Player Stuff

_VCMP_DLL( PlayerConnected )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerID )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "255" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( szData );
		if ( pPlayer )
		{
			sprintf(data, "%i", pPlayer->GetID());
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerName )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "Unknown" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%s", pPlayer->GetNick());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerIP )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%s", pPlayer->GetIP());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerPosition )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%f %f %f", pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetZ());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerAngle )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%f", pPlayer->GetRotation());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerHealth )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%i", pPlayer->GetHealth());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerArmour )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%i", pPlayer->GetArmour());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerControls )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				if ( pPlayer->GetFrozen() ) strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerWeapon )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%i", pPlayer->GetWeapon());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerCash )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%i", pPlayer->GetCash());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerMuted )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				if ( pPlayer->GetMuted() ) strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerIsIgnoredBy )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szIgnore[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szIgnore, strtok(0, " "));
		if ((Utils::IsNum(szPlayer)) && (Utils::IsNum(szIgnore)))
		{
			unsigned char uc1 = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char uc2 = static_cast< unsigned char >( atoi( szIgnore ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
			if ( pPlayer )
			{
				CPlayerPool* pIgnore = CPlayerPoolManager::Find( uc2 );
				if ( pIgnore )
				{
					if ( pPlayer->GetIgnored( uc2 ) ) strcpy(data, "1");
				}
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerScore )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				sprintf(data, "%i", pPlayer->GetScore());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerClass )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( szData );
		if ( pPlayer )
		{
			sprintf(data, "%i", pPlayer->GetClassID());
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerTeam )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( szData );
		if ( pPlayer )
		{
			sprintf(data, "%i", pPlayer->GetTeam());
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerSkin )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( szData );
		if ( pPlayer )
		{
			sprintf(data, "%i", pPlayer->GetSkin());
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerSpawned )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( szData );
		if ( pPlayer )
		{
			if ( pPlayer->GetSpawned() ) strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerSpectating )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( Utils::IsNum(szData) )
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				if ( pPlayer->GetSpectating() ) strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPlayerVehicle )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( Utils::IsNum(szData) )
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				if ( pPlayer->GetVehicle() != -1) sprintf(data, "%i", pPlayer->GetVehicle());
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerPosition )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };

	BYTE bToks = Utils::NumTok( szData, " " );
	if (szData && bToks >= 4)
	{
		strcpy(szPlayer, strtok(szData, " "));

		if ((CNet::GetInterface()) && (Utils::IsNum(szPlayer)))
		{

			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			float fX = (float)atof(strtok( 0, " " ));
			float fY = (float)atof(strtok( 0, " " ));
			float fZ = (float)atof(strtok( 0, " " ));

			unsigned char ucInterior = 0;
			if (bToks == 5) ucInterior = static_cast< unsigned char >( atoi( strtok( 0, " " ) ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->SetX(fX);
				pPlayer->SetY(fY);
				pPlayer->SetZ(fZ);
				CPlayerSendPackets::SetPlayerPosition(ucPlayer, fX, fY, fZ, ucInterior);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerHealth )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szHealth[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szHealth, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szHealth)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucHealth = static_cast< unsigned char >( atoi( szHealth ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->SetHealth( ucHealth );
				CPlayerSendPackets::SetPlayerHealth( ucPlayer, ucHealth );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerArmour )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szArmour[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szArmour, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szArmour)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucArmour = static_cast< unsigned char >( atoi( szArmour ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->SetArmour( ucArmour );
				CPlayerSendPackets::SetPlayerArmour( ucPlayer, ucArmour );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerControls )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szFrozen[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szFrozen, strtok(0, " "));

		if ((CNet::GetInterface()) && (Utils::IsNum(szPlayer)))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				if ((!strcmp( szFrozen, "1") ) || (!strcmp(szFrozen, "yes")) || (!strcmp(szFrozen, "true")) || (!strcmp(szFrozen, "on"))) pPlayer->SetFrozen(true);
				else pPlayer->SetFrozen(false);
				CPlayerSendPackets::SetPlayerFrozen( ucPlayer );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerWeapon )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szWeapon[ 8 ] = { 0 };

	BYTE bToks = Utils::NumTok( szData, " " );
	if (szData && bToks >= 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szWeapon, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szWeapon)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucWeapon = static_cast< unsigned char >( atoi( szWeapon ) );

			unsigned int uiAmmo = 500;
			if (bToks == 3) uiAmmo = static_cast< unsigned int >( atoi( strtok(0, " ") ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->SetWeapon( ucWeapon );
				CPlayerSendPackets::SetPlayerWeapon( ucPlayer, ucWeapon, uiAmmo );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerCash )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			int iCash = atoi( strtok( 0, " " ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->SetCash( iCash );
				CPlayerSendPackets::SetPlayerCash( ucPlayer );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( GivePlayerCash )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			int iCash = atoi( strtok( 0, " " ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->SetCash( pPlayer->GetCash() + iCash );
				CPlayerSendPackets::SetPlayerCash( ucPlayer );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( TakePlayerCash )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			int iCash = atoi( strtok( 0, " " ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->SetCash( pPlayer->GetCash() - iCash );
				CPlayerSendPackets::SetPlayerCash( ucPlayer );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerMuted )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szMute[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szMute, strtok(0, " "));

		if ((CNet::GetInterface()) && (Utils::IsNum(szPlayer)))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				if ((!strcmp( szMute, "1") ) || (!strcmp(szMute, "yes")) || (!strcmp(szMute, "true")) || (!strcmp(szMute, "on"))) pPlayer->SetMuted(true);
				else pPlayer->SetMuted(false);
				CPlayerSendPackets::SetPlayerMute( ucPlayer );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerIgnore )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szIgnore[ 8 ] = { 0 };
	char szMute[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 3)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szIgnore, strtok(0, " "));
		strcpy(szMute, strtok(0, " "));

		if ((CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szIgnore)))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucIgnore = static_cast< unsigned char >( atoi( szIgnore ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CPlayerPool* pIgnore = CPlayerPoolManager::Find( ucIgnore );
				if ( pIgnore )
				{
					if ((!strcmp( szMute, "1") ) || (!strcmp(szMute, "yes")) || (!strcmp(szMute, "true")) || (!strcmp(szMute, "on"))) pPlayer->SetIgnored( ucIgnore, true);
					else pPlayer->SetIgnored( ucIgnore, false);
					CPlayerSendPackets::SetPlayerIgnore( ucPlayer, ucIgnore );

					strcpy(data, "1");
				}
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerScore )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			int iScore = atoi( strtok( 0, " " ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->SetScore( iScore );
				CPlayerSendPackets::SetPlayerScore( ucPlayer, iScore );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( IncPlayerScore )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->IncScore();
				CPlayerSendPackets::IncPlayerScore(ucPlayer);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( DecPlayerScore )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				pPlayer->DecScore();
				CPlayerSendPackets::DecPlayerScore(ucPlayer);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerAnim )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szAnim[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szAnim, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szAnim)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucAnim = static_cast< unsigned char >( atoi( szAnim ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CPlayerSendPackets::SetPlayerAnim( ucPlayer, ucAnim );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerMarker )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szMarker[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szMarker, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szMarker)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucMarker = static_cast< unsigned char >( atoi( szMarker ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CPlayerSendPackets::SetPlayerMarker( ucPlayer, ucMarker );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( RemovePlayerMarker )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szData ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CPlayerSendPackets::RemovePlayerMarker( ucPlayer );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPlayerLocalMarker )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szMarker[ 8 ] = { 0 };
	char szPlayerTo[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 3)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szMarker, strtok(0, " "));
		strcpy(szPlayerTo, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szMarker)) && (Utils::IsNum(szPlayerTo)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucMarker = static_cast< unsigned char >( atoi( szMarker ) );
			unsigned char ucPlayerTo = static_cast< unsigned char >( atoi( szPlayerTo ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( ucPlayerTo );
				if ( pPlayerTo )
				{
					CPlayerSendPackets::SetLocalMarker( ucPlayer, ucPlayerTo, ucMarker );

					strcpy(data, "1");
				}
			}
		}
	}

	return 3;
}

_VCMP_DLL( RemovePlayerLocalMarker )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szPlayerTo[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 3)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szPlayerTo, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szPlayerTo)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucPlayerTo = static_cast< unsigned char >( atoi( szPlayerTo ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( ucPlayerTo );
				if ( pPlayerTo )
				{
					CPlayerSendPackets::RemoveLocalMarker( ucPlayer, ucPlayerTo );

					strcpy(data, "1");
				}
			}
		}
	}

	return 3;
}

_VCMP_DLL( PutPlayerInVehicle )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szVehicle[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szVehicle, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szVehicle)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucVehicle = static_cast< unsigned char >( atoi( szVehicle ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CVehiclePool* pVehicle = CVehiclePoolManager::Find( ucVehicle );
				if ( pVehicle )
				{
					CPlayerSendPackets::PutInVehicle( ucPlayer, ucVehicle );

					strcpy(data, "1");
				}
			}
		}
	}

	return 3;
}

//--------------------------------------------------------------------------------//
// Vehicle Stuff

_VCMP_DLL( GetVehiclePosition )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );
			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				sprintf(data, "%f %f %f", pVehicle->GetX(), pVehicle->GetY(), pVehicle->GetZ());
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetVehiclePosition )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szVehicle[ 8 ] = { 0 };

	if (szData && Utils::NumTok( szData, " " ) == 4)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szVehicle)))
		{
			strcpy(szVehicle, strtok(szData, " "));

			unsigned char ucVehicle = static_cast< unsigned char >( atoi( szVehicle ) );
			float fX = (float)atof(strtok( 0, " " ));
			float fY = (float)atof(strtok( 0, " " ));
			float fZ = (float)atof(strtok( 0, " " ));

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( ucVehicle );
			if ( pVehicle )
			{
				pVehicle->SetX(fX);
				pVehicle->SetY(fY);
				pVehicle->SetZ(fZ);
				CVehicleSendPackets::SetPosition(ucVehicle, fX, fY, fZ);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetVehicleLocked )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );
			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				if (pVehicle->GetLocked()) strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetVehicleLocked )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szVehicle [ 8 ] = { 0 };
	char szStatus [ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szVehicle, strtok(szData, " "));
		strcpy(szStatus, strtok(0, " "));

		if ((CNet::GetInterface()) && (Utils::IsNum(szVehicle)))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szVehicle ) );

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				if ((!strcmp(szStatus, "1")) || (!strcmp(szStatus, "yes")) || (!strcmp(szStatus, "true")) || (!strcmp(szStatus, "on"))) pVehicle->SetLocked(true);
				else pVehicle->SetLocked(false);
				CVehicleSendPackets::SetDoorStatus(uc);

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( KillVehicleEngine )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				CVehicleSendPackets::KillEngine( uc );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetVehicleModel )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				sprintf(data, "%i", pVehicle->GetModel());
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetVehicleHealth )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				sprintf(data, "%f", pVehicle->GetHealth());
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetVehicleHealth )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szVehicle[ 8 ] = { 0 };

	if (szData && Utils::NumTok( szData, " " ) == 2)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szVehicle)))
		{
			strcpy(szVehicle, strtok(szData, " "));

			unsigned char uc = static_cast< unsigned char >( atoi( szVehicle ) );
			float fHealth = (float)atof(strtok( 0, " " ));

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				pVehicle->SetHealth( fHealth );
				CVehicleSendPackets::SetHealth( uc );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( BlowVehicle )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szVehicle[ 8 ] = { 0 };

	if (szData && Utils::NumTok( szData, " " ) == 1)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szVehicle)))
		{
			strcpy(szVehicle, strtok(szData, " "));

			unsigned char uc = static_cast< unsigned char >( atoi( szVehicle ) );

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				pVehicle->SetHealth( 100000.0f );
				CVehicleSendPackets::SetHealth( uc );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetIndestructible )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szVehicle[ 8 ] = { 0 };

	if (szData && Utils::NumTok( szData, " " ) == 1)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szVehicle)))
		{
			strcpy(szVehicle, strtok(szData, " "));

			unsigned char uc = static_cast< unsigned char >( atoi( szVehicle ) );

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				pVehicle->SetHealth( 1000.0f );
				CVehicleSendPackets::SetHealth( uc );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetVehicleDriver )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				sprintf(data, "%i", pVehicle->GetDriver()); // TODO: recieve
			}
		}
	}

	return 3;
}

//--------------------------------------------------------------------------------//
// Pickup Sutff

_VCMP_DLL( CreatePickup )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPickup[ 8 ] = { 0 };

	if (szData && Utils::NumTok( szData, " " ) == 4)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szPickup)))
		{
			strcpy(szPickup, strtok(szData, " "));

			unsigned int uiPickup = static_cast< unsigned int >( atoi( szPickup ) );
			float fX = (float)atof(strtok( 0, " " ));
			float fY = (float)atof(strtok( 0, " " ));
			float fZ = (float)atof(strtok( 0, " " ));

			CPickupSendPackets::CreatePickup(uiPickup, fX, fY, fZ);

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( RemovePickup )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned int ui = static_cast< unsigned int >( atoi( szData ) );

			CPickupPool* pPickup = CPickupPoolManager::Find( ui );
			if ( pPickup )
			{
				CPickupSendPackets::DeletePickup( ui );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( RespawnPickup )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned int ui = static_cast< unsigned int >( atoi( szData ) );

			CPickupPool* pPickup = CPickupPoolManager::Find( ui );
			if ( pPickup )
			{
				CPickupSendPackets::RespawnPickup( ui );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SpawnPickupForPlayer )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer [ 8 ] = { 0 };
	char szPickup [ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szPickup, strtok(0, " "));

		if ((CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szPickup)))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ui = static_cast< unsigned int >( atoi( szPickup ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{

				CPickupPool* pPickup = CPickupPoolManager::Find( ui );
				if ( pPickup )
				{
					CPickupSendPackets::RespawnPickupForPlayer( uc, ui );

					strcpy(data, "1");
				}
			}
		}
	}

	return 3;
}

_VCMP_DLL( GetPickupModel )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned int ui = static_cast< unsigned int >( atoi( szData ) );

			CPickupPool* pPickup = CPickupPoolManager::Find( ui );
			if ( pPickup )
			{
				sprintf(data, "%i", pPickup->GetModel());
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetPickupOnceOnly )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPickup[ 8 ] = { 0 };
	char szParam[ 8 ] = { 0 };

	if (szData && Utils::NumTok( szData, " " ) == 4)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szPickup)))
		{
			strcpy(szPickup, strtok(szData, " "));
			strcpy(szParam, strtok(szData, " "));

			unsigned int uiPickup = static_cast< unsigned int >( atoi( szPickup ) );
			if ((!strcmp( szParam, "1") ) || (!strcmp(szParam, "yes")) || (!strcmp(szParam, "true")) || (!strcmp(szParam, "on")))
			{
				CPickupSendPackets::SetOnlyOnce(uiPickup, true);
			}
			else CPickupSendPackets::SetOnlyOnce(uiPickup, false);

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( SetPickupRespawnTime )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPickup[ 8 ] = { 0 };
	char szParam[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szPickup, strtok(szData, " "));
		strcpy(szParam, strtok(0, " "));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPickup)) && (Utils::IsNum(szParam)) )
		{
			unsigned int ui = static_cast< unsigned int >( atoi( szPickup ) );
			unsigned char uc = static_cast< unsigned char >( atoi( szParam ) );

			CPickupSendPackets::SetPickupRespawnTime( ui, uc );
			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( SetGlobalPickupRespawnTime )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CPickupSendPackets::SetRespawnTime( uc );
			strcpy(data, "1");
		}
	}

	return 3;
}

//--------------------------------------------------------------------------------//
// Game Stuff

_VCMP_DLL( GetHour )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%i", CGameSettings::GetHour());

	return 3;
}

_VCMP_DLL( GetMinute )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%i", CGameSettings::GetMinute());

	return 3;
}

_VCMP_DLL( GetTime )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%i %i", CGameSettings::GetHour(), CGameSettings::GetMinute());

	return 3;
}

_VCMP_DLL( SetHour )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szData)))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CGameSettings::SetHour( uc );
			CGameSendPackets::SetTime( uc, 255 );

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( SetMinute )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szData)))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CGameSettings::SetMinute( uc );
			CGameSendPackets::SetTime( 255, uc );

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( SetTime )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szHour [ 8 ] = { 0 };
	char szMinute [ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szHour, strtok(szData, " "));
		strcpy(szMinute, strtok(0, " "));

		if ((CNet::GetInterface()) && (Utils::IsNum(szHour)) && (Utils::IsNum(szMinute)))
		{
			unsigned char ucHour = static_cast< unsigned char >( atoi( szHour ) );
			unsigned char ucMinute = static_cast< unsigned char >( atoi( szMinute ) );

			CGameSettings::SetHour( ucHour );
			CGameSettings::SetMinute( ucMinute );

			CGameSendPackets::SetTime(ucHour, ucMinute);

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( GetTimeRate )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%i", CGameSettings::GetTimeRate());

	return 3;
}

_VCMP_DLL( SetTimeRate )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CGameSettings::SetTimeRate( uc );
			CGameSendPackets::SetTimeRate( uc );
			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( GetWeather )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%i", CGameSettings::GetWeather());

	return 3;
}

_VCMP_DLL( SetWeather )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ( (CNet::GetInterface()) && (Utils::IsNum(szData)) )
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CGameSettings::SetWeather( uc );
			CGameSendPackets::SetWeather( uc );

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( GetWeatherRate )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	sprintf(data, "%i", CGameSettings::GetWeatherRate());

	return 3;
}

_VCMP_DLL( SetWeatherRate )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if (Utils::IsNum(szData))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szData ) );

			CGameSettings::SetWeatherRate( uc );
			CGameSendPackets::SetWeatherRate( uc );
			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( GetWeatherLock )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );

	strcpy(data, CGameSettings::GetWeatherRate() ? "0" : "1" );

	return 3;
}

_VCMP_DLL( SetWeatherLock )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	bool bLocked = false;

	if (szData && Utils::NumTok(szData, " ") == 1)
	{
		if ((!strcmp( szData, "1") ) || (!strcmp(szData, "yes")) || (!strcmp(szData, "true")) || (!strcmp(szData, "on"))) bLocked = true;
		else bLocked = false;

		CGameSettings::SetWeatherRate( bLocked ? NULL : 600000 );
		CGameSendPackets::SetWeatherRate( bLocked ? NULL : 600000 );
		strcpy(data, "1");
	}

	return 3;
}

_VCMP_DLL( CreateMarker )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szType[ 8 ] = { 0 };

	if (szData && Utils::NumTok( szData, " " ) == 4)
	{
		if ((CNet::GetInterface()) && (Utils::IsNum(szType)))
		{
			strcpy(szType, strtok(szData, " "));

			unsigned char uc = static_cast< unsigned char >( atoi( szType ) );
			float fX = (float)atof(strtok( 0, " " ));
			float fY = (float)atof(strtok( 0, " " ));
			float fZ = (float)atof(strtok( 0, " " ));

			CGameSendPackets::SetMarker(uc, fX, fY, fZ);

			strcpy(data, "1");
		}
	}

	return 3;
}

_VCMP_DLL( CreateMarkerForPlayer )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szPlayer[ 8 ] = { 0 };
	char szMarker[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 5)
	{
		strcpy(szPlayer, strtok(szData, " "));
		strcpy(szMarker, strtok(0, " "));
		float fX = (float)atof(strtok( 0, " " ));
		float fY = (float)atof(strtok( 0, " " ));
		float fZ = (float)atof(strtok( 0, " " ));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szPlayer)) && (Utils::IsNum(szMarker)) )
		{
			unsigned char ucPlayer = static_cast< unsigned char >( atoi( szPlayer ) );
			unsigned char ucMarker = static_cast< unsigned char >( atoi( szMarker ) );

			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				CGameSendPackets::CreateMarkerForPlayer( ucPlayer, ucMarker, fX, fY, fZ );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetAmmuWeapon )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szShopID[ 8 ] = { 0 };
	char szSlot[ 8 ] = { 0 };
	char szWeaponID[ 8 ] = { 0 };
	char szWeapCost[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 5)
	{
		strcpy(szShopID, strtok(szData, " "));
		strcpy(szSlot, strtok(0, " "));
		strcpy(szWeaponID, strtok( 0, " " ));
		strcpy(szWeapCost, strtok( 0, " " ));

		if ( (CNet::GetInterface()) && (Utils::IsNum(szShopID)) && (Utils::IsNum(szSlot)) && (Utils::IsNum(szWeaponID)) && (Utils::IsNum(szWeapCost)) )
		{
			unsigned char uc1 = static_cast< unsigned char >( atoi( szShopID ) );
			unsigned char uc2 = static_cast< unsigned char >( atoi( szSlot ) );
			unsigned int ui1 = static_cast< unsigned int >( atoi( szWeaponID ) );
			unsigned int ui2 = static_cast< unsigned int >( atoi( szWeapCost ) );

			CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc1 );
			if ( pAmmu )
			{
				pAmmu->SetWeapon( uc2, ui1 );
				pAmmu->SetWeaponCost( uc2, ui2 );

				CGameSendPackets::SetAmmuWeapon( uc1, uc2 );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

_VCMP_DLL( IsAmmuWeaponEnabled )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szShopID[ 8 ] = { 0 };
	char szParam[ 8 ] = { 0 };

	if (szData && Utils::NumTok(szData, " ") == 2)
	{
		strcpy(szShopID, strtok(szData, " "));
		strcpy(szParam, strtok(0, " "));
		if ( (Utils::IsNum(szShopID)) && (Utils::IsNum(szParam)) )
		{
			unsigned char uc1 = static_cast< unsigned char >( atoi( szShopID ) );
			unsigned char uc2 = static_cast< unsigned char >( atoi( szParam ) );

			CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc1 );
			if ( pAmmu )
			{
				sprintf( data, "%i", pAmmu->GetWeaponEnabled( uc2 ) );
			}
		}
	}

	return 3;
}

_VCMP_DLL( SetAmmuWeaponEnabled )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "0" );
	char szShopID[ 8 ] = { 0 };
	char szWeapon[ 8 ] = { 0 };
	char szEnabled[ 8 ] = { 0 };
	bool bEnabled = false;

	if (szData && Utils::NumTok(szData, " ") == 3)
	{
		strcpy(szShopID, strtok(szData, " "));
		strcpy(szWeapon, strtok(0, " "));
		strcpy(szEnabled, strtok(0, " "));

		if ((CNet::GetInterface()) && (Utils::IsNum(szShopID)) && (Utils::IsNum(szWeapon)))
		{
			unsigned char uc = static_cast< unsigned char >( atoi( szShopID ) );
			unsigned char uc2 = static_cast< unsigned char >( atoi( szWeapon ) );

			CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc );
			if ( pAmmu )
			{
				if ((!strcmp( szEnabled, "1") ) || (!strcmp(szEnabled, "yes")) || (!strcmp(szEnabled, "true")) || (!strcmp(szEnabled, "on"))) pAmmu->SetWeaponEnabled( uc2, true );
				else pAmmu->SetWeaponEnabled( uc2, false );
				CGameSendPackets::EnableWeaponAtAmmu( uc, uc2 );

				strcpy(data, "1");
			}
		}
	}

	return 3;
}

//--------------------------------------------------------------------------------//
// Other Functions

_VCMP_DLL ( GetWeaponName )
{
	char szData [ 512 ] = { 0 };
	char szNick[ 32 ] = { NULL };
	strcpy ( szData, data );
	strcpy ( data, "Unknown" );

	if ( szData && Utils::NumTok( szData, " " ) == 1 )
	{
		if ( Utils::IsNum( szData ) )
		{
			BYTE bReason = static_cast< BYTE >( atoi( szData ) );
			strcpy( data, Utils::GetDeathReason( bReason ) );
		}
	}
	return 3;
}

_VCMP_DLL ( GetWeaponID )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "-1" );

	if ( szData && Utils::NumTok( szData, " " ) >= 1 )
	{
		sprintf( data, "%i", Utils::GetWeaponID( szData ) );
	}
	return 3;
}

_VCMP_DLL ( GetDistrictName )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "Vice City" );

	if ( szData && Utils::NumTok( szData, " " ) == 2 )
	{
		float fX = (float)atof( strtok( szData, " " ) );
		float fY = (float)atof( strtok( 0, " " ) );

		strcpy( data, Utils::GetAreaName( fX, fY ) );
	}
	return 3;
}

_VCMP_DLL ( GetVehicleNameFromModel )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "Unknown" );

	if ( szData && Utils::NumTok( szData, " " ) >= 1 )
	{
		if ( Utils::IsNum( szData ) )
		{
			BYTE bModel = static_cast< BYTE >( atoi( szData ) );
			strcpy( data, Utils::GetVehicleName( bModel ) );
		}
	}
	return 3;
}

_VCMP_DLL ( GetVehicleModelFromName )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "Unknown" );

	if ( szData && Utils::NumTok( szData, " " ) >= 1 )
	{
		sprintf( data, "%i", Utils::GetVehicleModel( szData ) );
	}
	return 3;
}

_VCMP_DLL ( GetPartReason )
{
	char szData [ 512 ] = { 0 };
	strcpy ( szData, data );
	strcpy ( data, "Unknown" );

	if ( szData && Utils::NumTok( szData, " " ) == 1 )
	{
		if ( Utils::IsNum( szData ) )
		{
			BYTE bReason = static_cast< BYTE >( atoi( szData ) );
			strcpy( data, Utils::GetPartReason( bReason ) );
		}
	}
	return 3;
}

//--------------------------------------------------------------------------------//
// Net

unsigned __stdcall NetThread( void* args )
{
	CNet::Start();

	while (bRunning)
	{
		if (CNet::GetInterface()) CNet::Listen();

		CGameSettings::TimeUpdate();

		Sleep(5);
	}

	CNet::Stop();

	return 1;
}

//--------------------------------------------------------------------------------//

void CallSignal(char* szAlias, char* szParams, ...)
{
	if (szParams)
	{
		char sz [ 500 ] = { 0 };
		va_list va_alist;
		va_start(va_alist, szParams);
		_vsnprintf(sz, sizeof(sz), szParams, va_alist);
		va_end(va_alist);

		wsprintf(mData, "/.signal %s %s", szAlias, sz);
	}
	else wsprintf(mData, "/.signal %s %s", szAlias, szParams);
	SendMessage(_mWnd, WM_USER + 200, 0, 0);
}

void CallAlias(char* szAlias, char* szParams, ...)
{
	if (szParams)
	{
		char sz [ 500 ] = { 0 };
		va_list va_alist;
		va_start(va_alist, szParams);
		_vsnprintf(sz, sizeof(sz), szParams, va_alist);
		va_end(va_alist);

		wsprintf(mData, "$%s(%s)", szAlias, sz);
	}
	else wsprintf(mData, "$%s(%s)", szAlias, szParams);
	SendMessage(_mWnd, WM_USER + 201, 0, 0);
}

void CallFunc(char* szAlias, char* szParams, ...)
{
	if (szParams)
	{
		char sz [ 500 ] = { 0 };
		va_list va_alist;
		va_start(va_alist, szParams);
		_vsnprintf(sz, sizeof(sz), szParams, va_alist);
		va_end(va_alist);

		strcpy(sz,strtok(sz, "\n"));

		wsprintf(mData, "/%s %s", szAlias, sz);
	}
	else wsprintf(mData, "/%s %s", szAlias, szParams);
	SendMessage(_mWnd, WM_USER + 200, 1, 0);
}

//--------------------------------------------------------------------------------//

typedef struct
{
	DWORD  mVersion;
	HWND   mHwnd;
	BOOL   mKeep;
} LOADINFO;

void WINAPI LoadDll(LOADINFO *li)
{
	_mWnd = li->mHwnd;

	hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE,0,PAGE_READWRITE,0,4096,"mIRC");	
	mData = (LPSTR)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS,0,0,0);
}

int WINAPI UnloadDll(int timeout)
{
	if (( !timeout ) || ( timeout == 2 ))
	{
		bRunning = false;

		Sleep( 100 );

		CRconPoolManager::RemoveAll();
		CPlayerPoolManager::RemoveAll();
		CPickupPoolManager::RemoveAll();
		CVehiclePoolManager::RemoveAll();
		CAmmuPoolManager::RemoveAll();

		if ( hThread ) CloseHandle( hThread );

		if ( mData ) UnmapViewOfFile(mData);
		if ( hFileMap ) CloseHandle(hFileMap);

		return 1;
	}
	return 0;
}

//--------------------------------------------------------------------------------//