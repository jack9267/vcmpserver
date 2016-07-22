#include "MiscSendPackets.h"

extern GAME_SETTINGS pSettings;

void CMiscSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_MISC_PACKET );
	bs.WriteCompressed( uc );
}

void CMiscSendPackets::JoinRequest( void )
{
	if ( strlen( pSettings.szNick ) > 0 )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_RCONJOIN, bs );

		bs.Write( (unsigned short)RCON_VERSION );

		stringCompressor->EncodeString( pSettings.szNick, MAX_NAME_LEN, &bs );

		//bs.WriteCompressed( (unsigned char)MOD_GAME );
		//bs.WriteCompressed( (unsigned char)MOD_VERSION );

		NetInterface->Send( &bs, HIGH_PRIORITY, RELIABLE, 0 );
	}
}