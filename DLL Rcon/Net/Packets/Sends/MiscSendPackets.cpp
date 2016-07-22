#include "MiscSendPackets.h"

void CMiscSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_MISC_PACKET );
	bs.WriteCompressed( uc );
}

void CMiscSendPackets::RconJoin(char* szNick)
{
	unsigned short usVersion = RCON_VERSION;
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_RCONJOIN, bs);

	bs.Write(usVersion);
	stringCompressor->EncodeString(szNick, MAX_NAME_LEN, &bs);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::AdminChat(char* szText)
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_RCONCHAT, bs);

	stringCompressor->EncodeString(szText, MAX_TEXT_LEN, &bs);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::AdminPM(unsigned char uc, char* szText)
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_MISC_RCONMSG, bs);

		bs.WriteCompressed(uc);
		stringCompressor->EncodeString(szText, MAX_TEXT_LEN, &bs);

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CMiscSendPackets::Announce(unsigned char uc, char* szText)
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_MISC_RCONANN, bs);

		bs.WriteCompressed(uc);
		stringCompressor->EncodeString(szText, MAX_TEXT_LEN, &bs);

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CMiscSendPackets::AnnounceAll(char* szText)
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_RCONANNALL, bs);

	stringCompressor->EncodeString(szText, MAX_TEXT_LEN, &bs);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::KickPlayer( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_MISC_KICK, bs);

		bs.WriteCompressed( uc );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CMiscSendPackets::BanPlayer( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_MISC_BANID, bs);

		bs.WriteCompressed( uc );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CMiscSendPackets::BanIP( char* szIP )
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_BANIP, bs);

	stringCompressor->EncodeString( szIP, MAX_IP_LEN, &bs );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::UnbanIP( char* szIP )
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_UNBAN, bs);

	stringCompressor->EncodeString( szIP, MAX_IP_LEN, &bs );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::SetPassword(char* szPassword)
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_SETPASSWORD, bs);

	stringCompressor->EncodeString(szPassword, MAX_PASS_LEN, &bs);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::SetMaxPlayers( unsigned char uc )
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_SETMAXPLAYERS, bs);

	bs.WriteCompressed( uc );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::SetScoreBoard( bool b )
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_SETSCOREBOARD, bs);

	bs.Write( b );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::SetDriveBy( bool b )
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_SETDRIVEBY, bs);

	bs.Write( b );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::MessageAllExcept(unsigned char uc, char* szText)
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_MISC_CHATEXCEPT, bs);

		bs.WriteCompressed( uc );
		stringCompressor->EncodeString(szText, MAX_TEXT_LEN, &bs);

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CMiscSendPackets::MessagePlayer(unsigned char uc, char* szText)
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_MISC_CHATPLAYER, bs);

		bs.WriteCompressed( uc );
		stringCompressor->EncodeString(szText, MAX_TEXT_LEN, &bs);

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CMiscSendPackets::SendPlayerMessage(unsigned char uc1, unsigned char uc2, char* szText)
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc2 );
		if ( pPlayerTo )
		{
			RakNet::BitStream bs;

			PreparePacket(ID_MISC_CHATTOPLAYER, bs);

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );
			stringCompressor->EncodeString(szText, MAX_TEXT_LEN, &bs);

			CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
		}
	}
}

void CMiscSendPackets::SetServerName( char* szName )
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_SETSERVNAME, bs);

	stringCompressor->EncodeString(szName, MAX_SERVNAME_LEN, &bs);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CMiscSendPackets::SetGamemodeName( char* szMode )
{
	RakNet::BitStream bs;

	PreparePacket(ID_MISC_SETGAMEMODE, bs);

	stringCompressor->EncodeString(szMode, MAX_GAMEMODE_LEN, &bs);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}