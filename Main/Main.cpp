/*
	Main.cpp
	This file is part of VCMP Server.

    VCMP Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VCMP Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VCMP Server.  If not, see <http://www.gnu.org/licenses/>.

	--------------------------------------------------------------------

    VC:MP Server 
		A server for VC:MP - A Multiplayer Modification For GTA:VC
    Copyright 2009 VRocker, Knight2k9 and Juppi

	File Author: VRocker

	--------------------------------------------------------------------
*/
#include "Main.h"
#include <time.h>
#include "Net.h"
#include "Rcon.h"
#include "GameProc.h"
#include "Utils.h"
#include "BanList.h"
#include "RakThread.h"

bool bRunning = true;
char szInput[ 256 ] = { NULL };
char szConfigFile[ 32 ] = { NULL };
bool bNoInput = false;

HANDLE hThread = NULL;
SERVER_INFO pServerInfo;

/*#ifdef _WIN32
    unsigned __stdcall NetThread( void* args );
#else
    void* NetThread( void* args );
#endif*/

RAK_THREAD_DECLARATION( NetThread );

void DoCleanup()
{
	bRunning = false;
	CConsole::PrintTime( "*** Server shutting down... ***", COLOUR_GREY, 25 );

	// Do some cleanup!
	CSquirrelManager::RemoveAll();

	CPlayerPoolManager::RemoveAll();
	CVehiclePoolManager::RemoveAll();
	CPickupPoolManager::RemoveAll();

#if defined( WIN32 ) || defined( WIN64 )
		Sleep( 500 );
#else
		usleep( 500 * 1000 );
#endif
}

int main ( int argc, char* argv[] )
{
#if defined( WIN32 ) || defined( WIN64 )
	SetConsoleTitle( "VC:MP 0.3z Server" );
#endif

	/*if( argc > 1 ) strncpy( szConfigFile, argv[1], 32 );
	else strcpy( szConfigFile, "./server.conf" );*/

	unsigned int ui = 1;
	while ( ui < argc )
	{
		if ( !_strcmpi( argv[ ui ], "-noinput" ) )
		{
			bNoInput = true;
		}
		else strncpy( szConfigFile, argv[ ui ], 32 );

		ui++;
	}

	if ( strlen( szConfigFile ) == 0 ) strcpy( szConfigFile, "./server.conf" );

	FILE *fileConfig = fopen( szConfigFile, "r" );

	if( !fileConfig )
	{
		CConsole::PrintC( "WARNING", COLOUR_RED, "Config file not found (%s), Starting with default options...", szConfigFile );
#if defined( WIN32 ) || defined( WIN64 )
		Sleep( 1000 );
#else
		usleep( 1000 * 1000 );
#endif
		//return 0;
	}
	else fclose( fileConfig );

	memset( &pServerInfo, 0, sizeof( SERVER_INFO ) );

	srand( time(0) ); // Seeds the randomiser to make it a bit more random :)

	atexit( DoCleanup );
	
	CConfigLoader::Initialize( szConfigFile );
	CConfigLoader::LoadSettings();
	CConfigLoader::LoadClasses();
	CConfigLoader::LoadVehicleColours();
	CConfigLoader::LoadVehicles();
	CConfigLoader::LoadPickups();
	CConfigLoader::LoadAmmunations();
	CConfigLoader::LoadSquirrelScripts();

	pServerInfo.ucServerNameLen = strlen( CConfigLoader::GetServerName() );
	strncpy( pServerInfo.szServerName, CConfigLoader::GetServerName(), 128 );
	pServerInfo.ucMaxPlayers = CConfigLoader::GetMaxPlayers();
	strncpy( pServerInfo.szMode, CConfigLoader::GetGamemodeName(), 64 );
	if ( CConfigLoader::GetMap() != 2 ) strcpy( pServerInfo.szMap, "Vice-City" );
	else strcpy( pServerInfo.szMap, "Liberty-City" );

	CConsole::PrintTime( "*** Server started on port %i ***", COLOUR_GREY, 25, CConfigLoader::GetListenPort() );

	/*unsigned int ThreadId = 0;
#ifdef _WIN32
	hThread = (HANDLE)_beginthreadex(NULL, 0, NetThread, NULL, 0, &ThreadId );
#else
	pthread_t threadHandle;

	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

	pthread_create( &threadHandle, &attr, NetThread, NULL );
	hThread = (HANDLE)threadHandle;
#endif*/
	RakNet::RakThread::Create( NetThread, NULL );

	CConfigLoader::LoadBans();

	CScripting::PushFunction( "onServerStart" );
	CScripting::CallFunction();

	// Start the loop
	while( bRunning )
	{
		if ( !bNoInput )
		{
			// Do the console input stuff
			fgets( szInput, 256, stdin );
			ProcessConsoleInput( szInput );
		}
		else Sleep( 100 );
	}
	// Do cleanup will automatically get called here
}

void ProcessConsoleInput( char* sz )
{
	char szCmd[ 256 ] = { NULL };
	char szText[ 1024 ] = { NULL };

	unsigned char ucTokens = CUtils::NumTok( sz, " " );

	strncpy( szText, sz, 1024 );

	if ( strcmp( sz, "\n" ) )
	{
		if ( ucTokens > 1 )
		{
			strncpy( szCmd, strtok( sz, " " ), 256 );
			strncpy( szText, strtok( 0, "\n" ), 1024 );
		}
		else
		{
			strncpy( szCmd, strtok( sz, "\n" ), 256 );
			strcpy( szText, "" );
		}

		if ( !strcmp( szCmd, "quit" ) ) bRunning = false;
		else if ( !strcmp( szCmd, "say" ) )
		{
			char szMessage[ 256 ] = { NULL };
			sprintf( szMessage, "Server: %s", szText );

			CNetSends::AdminChat( szMessage );
			CMiscSendPackets::AdminChat( 255, szText );
		}
		else if ( !strcmp( szCmd, "msg" ) )
		{
			char szPlayer[ 2 ] = { NULL };
			char szMessage[ 256 ] = { NULL };
			strncpy( szPlayer, strtok( szText, " " ), 2 );
			strncpy( szMessage, strtok( 0, "\n" ), 256 );

			CNetSends::AdminMsg( atoi( szPlayer ), szMessage );
		}
		else if ( !strcmp( szCmd, "unload_script" ) )
		{
			if ( strlen( szText ) > 0 )
			{
				CSquirrel* pScript = CSquirrelManager::Find( szText );
				if ( pScript )
				{
					CSquirrelManager::Remove( pScript );

					CConsole::PrintC( "SCRIPT UNLOAD", COLOUR_RED, "%s has been unloaded successfully", szText );
				}
			}
		}
		else if ( !strcmp( szCmd, "load_script" ) )
		{
			if ( strlen( szText ) > 0 )
			{
				FILE * pFile = fopen( szText, "r" );

				CSquirrel* pScript = NULL;
				if ( pFile ) 
				{
					pScript = CSquirrelManager::New( szText );
					if ( pScript ) 
					{
						CConsole::PrintC( "SCRIPT LOAD", COLOUR_RED, "%s has been loaded successfully", szText );
						pScript->PushFunction("onScriptLoad");
						pScript->CallFunction();
					}

					fclose( pFile );
				}
			}
		}
		else if ( !strcmp( szCmd, "reload_scripts" ) )
		{
			CSquirrelManager::RemoveAll();

			CConfigLoader::LoadSquirrelScripts();
		}
		else if ( !strcmp( szCmd, "clear_bans" ) )
		{
			CBanList::Clear();
		}
		else if ( !strcmp( szCmd, "reload_bans" ) )
		{
			CBanList::Reload();
		}
		else if ( !strcmp( szCmd, "banip" ) )
		{
			if ( strlen( szText ) > 0 )
			{
				CBanList::AddToBanList( szText );
				CConsole::PrintC( "ADMIN", COLOUR_RED, "Server Banned IP: %s", szText );
			}
		}
		else if ( !strcmp( szCmd, "ban" ) )
		{
			if ( strlen( szText ) > 0 )
			{
				if ( CUtils::IsNum( szText ) )
				{
					CPlayerPool* pPlayer = CPlayerPoolManager::Find( atoi( szText ) );
					if ( pPlayer ) CNetSends::BanPlayer( pPlayer->GetID() );
				}
			}
		}
		else if ( !strcmp( szCmd, "kick" ) )
		{
			if ( strlen( szText ) > 0 )
			{
				if ( CUtils::IsNum( szText ) )
				{
					CPlayerPool* pPlayer = CPlayerPoolManager::Find( atoi( szText ) );
					if ( pPlayer ) CNetSends::KickPlayer( pPlayer->GetID() );
				}
			}
		}
		else
		{
			CScripting::PushFunction( "onConsoleInput" );
			CScripting::PushString( szCmd );
			CScripting::PushString( szText );
			CScripting::CallFunction();
		}
	}
}

void FilterInvalidNickChars( char* szString )
{
	while(*szString) {
		if(*szString < ' ' || *szString > 'z' || *szString == '%') {
			*szString = '_';
		}
		szString++;
	}
}

/*#ifdef _WIN32
    unsigned __stdcall NetThread( void* args )
#else
    void* NetThread( void* args )
#endif*/
RAK_THREAD_DECLARATION( NetThread )
{
	CNet::Start();
	CRcon::Start();

	while ( bRunning )
	{
		if ( CNet::GetInterface() ) CNet::Listen();

		if ( CRcon::GetInterface() ) CRcon::Listen();

		// Even though this is the net thread, we do processing here
		CGameProc::TimeAndWeatherSync();
		CGameProc::PickupRespawn();
		CGameProc::VehicleRespawn();

		// Process the squirrel timers
		CSquirrelTimerManager::ProcessTimers();
		// Process the squirrel sockets
		CSquirrelSocketManager::ProcessSockets();

#if defined( WIN32 ) || defined( WIN64 )
		Sleep( 5 );
#else
		usleep( 5 * 1000 );
#endif
	}

	CNet::Stop();
	if ( CRcon::GetInterface() ) CRcon::Stop();

	return 1;
}
