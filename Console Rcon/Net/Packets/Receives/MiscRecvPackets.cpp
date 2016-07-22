#include "MiscRecvPackets.h"
#include "Pools.h"

RakNet::BitStream CMiscRecvPackets::bs;
Packet* CMiscRecvPackets::m_pPacket = NULL;

void CMiscRecvPackets::Parse( Packet* pPacket )
{
	if ( pPacket )
	{
		unsigned char ucPacket1 = 0, ucPacket2 = 0;

		m_pPacket = pPacket;

		bs = RakNet::BitStream( (char*)pPacket->data, pPacket->bitSize, false );

		bs.Read( ucPacket1 );
		bs.ReadCompressed( ucPacket2 );

		printf( "Misc Packet: %i %i\n", ucPacket1, ucPacket2 );

		switch ( ucPacket2 )
		{
		case ID_MISC_PLAYERLIST:
			PlayerList();
			break;
		}
	}
}

void CMiscRecvPackets::PlayerList( void )
{
	BYTE bytePlayers = 0;
	unsigned char uc = 0, ucPlayerID = 0;
	char szNick[ MAX_NAME_LEN ] = { NULL };
	float fX = 0.0f, fY = 0.0f, fZ = 0.0f, fAngle = 0.0f;

	bs.Read( bytePlayers );

	while ( uc < bytePlayers )
	{
		bs.ReadCompressed( ucPlayerID );

		stringCompressor->DecodeString( szNick, MAX_NAME_LEN, &bs );

		bs.ReadVector( fX, fY, fZ );
		bs.Read( fAngle );

		CPlayerPool* pPlayer = CPlayerPoolManager::New( ucPlayerID );
		if ( pPlayer )
		{
			pPlayer->SetNick( szNick );
		}

		uc++;
	}
}