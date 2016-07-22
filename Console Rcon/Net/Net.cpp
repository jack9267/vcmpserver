#include "Net.h"
#include <process.h>

extern GAME_SETTINGS pSettings;

RakClientInterface* CNet::m_pRakClient = NULL;
HANDLE CNet::m_hThread = NULL;
bool CNet::m_bStarted = false;

unsigned __stdcall Recieve( void* args );

void CNet::Start( void )
{
	m_pRakClient = RakNetworkFactory::GetRakClientInterface();

	if ( m_pRakClient )
	{
		m_pRakClient->SetPassword( pSettings.szPass );
		m_pRakClient->Connect( pSettings.szIP, (unsigned short)pSettings.usPort, 0, 0, 5 );
		m_pRakClient->StartOccasionalPing();

		printf( "Connecting to %s:%d...\n", pSettings.szIP, pSettings.usPort );

		m_bStarted = true;

		unsigned int ThreadId = 0;
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, Recieve, NULL, 0, &ThreadId );
	}
}

void CNet::Stop( void )
{
	m_bStarted = false;

	if ( m_hThread ) CloseHandle( m_hThread );

	if ( m_pRakClient )
	{
		m_pRakClient->Disconnect( 500 );
		RakNetworkFactory::DestroyRakClientInterface( m_pRakClient );
	}
}

unsigned __stdcall Recieve( void* args )
{
	bool bRun = true;
	while ( ( bRun ) && ( CNet::GetStarted() ) )
	{
		if ( CNet::GetInterface() )
		{
			Packet* pPacket = NULL;
			while ( ( bRun ) && ( pPacket = CNet::GetInterface()->Receive() ) )
			{
				switch ( (unsigned char)pPacket->data[0] )
				{
				case ID_CONNECTION_REQUEST_ACCEPTED:
					printf( "Connected!\n" );
					CMiscSendPackets::JoinRequest();
					break;

				case ID_DISCONNECTION_NOTIFICATION:
					printf( "Disconnected\n" );
					bRun = false;

					break;

				case ID_CONNECTION_LOST:
					printf( "Lost Connection\n" );
					bRun = false;
					break;

				case ID_CONNECTION_BANNED:
					printf( "You are banned\n" );
					bRun = false;
					break;

				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf( "No free connections\n" );
					bRun = false;
					break;

				case ID_INVALID_PASSWORD:
					printf( "Invalid Password\n" );
					bRun = false;
					break;

				case ID_RECEIVED_STATIC_DATA:
					//printf( "Recieved Static Data.\n" );
					break;

				case ID_MISC_PACKET:
					CMiscRecvPackets::Parse( pPacket );
					break;

				case ID_PLAYER_PACKET:
					break;

				default:
					printf( "Packet %i recieved\n", pPacket->data[0] );
					break;
				}

				CNet::GetInterface()->DeallocatePacket( pPacket );		
			}
		}

		Sleep( 5 );
	}

	return 1;
}