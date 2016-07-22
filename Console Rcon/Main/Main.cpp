#include "Main.h"
#include "Net.h"
#include <time.h>

GAME_SETTINGS pSettings;

bool bRunning = true;
char szBuffer[ 256 ] = { NULL };

void ParseInput( char* sz )
{
	if ( !strcmp( sz, "quit" ) ) bRunning = false;
}

int main( void )
{
	memset( &pSettings, 0, sizeof( GAME_SETTINGS ) );

	srand( time( 0 ) );

	char szIP[ 256 ];
	puts( "Enter IP to connect to: " );
	gets( szIP );

	if ( szIP[ 0 ] == 0 ) strcpy( pSettings.szIP, "127.0.0.1" );
	else strcpy( pSettings.szIP, szIP );

	puts ( "Enter port: " );
	char szPort[ 256 ];
	gets( szPort );

	if ( szPort[0] == 0 ) pSettings.usPort = 5315;
	else pSettings.usPort = atoi( szPort );

	puts( "Enter your name: " );
	gets( pSettings.szNick );

	puts( "Enter the password for the server: " );
	gets( pSettings.szPass );

	if ( pSettings.szPass[0] == 0 ) strcpy( pSettings.szPass, "Blah" );

	CNet::Start();

	while ( bRunning )
	{
		gets( szBuffer );
		ParseInput( szBuffer );
	}

	CNet::Stop();

	return 1;
}