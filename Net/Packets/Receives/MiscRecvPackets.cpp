/*
	MiscRecvPackets.cpp
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

	File Author: Knight2k9

	--------------------------------------------------------------------
*/

#include "MiscRecvPackets.h"
#include "BanList.h"

RakNet::BitStream CMiscRecvPackets::bs;
Packet* CMiscRecvPackets::m_pPacket = NULL;

extern SERVER_INFO pServerInfo;

void CMiscRecvPackets::Parse( Packet* p )
{
	if ( p )
	{
		unsigned char ucPacket1 = 0, ucPacket2 = 0;

		m_pPacket = p;

		bs = RakNet::BitStream( (char*)p->data, p->bitSize, false );

		bs.Read( ucPacket1 );
		bs.ReadCompressed( ucPacket2 );

		switch ( ucPacket2 )
		{
		case ID_MISC_RCONCHAT:
			AdminChat();
			break;
		case ID_MISC_RCONJOIN:
			RconJoin();
			break;
		case ID_MISC_RCONMSG:
			AdminPM();
			break;
		case ID_MISC_RCONANN:
			Announce();
			break;
		case ID_MISC_RCONANNALL:
			AnnounceAll();
			break;
		case ID_MISC_SETPASSWORD:
			SetPassword();
			break;
		case ID_MISC_SETMAXPLAYERS:
			SetMaxPlayers();
			break;
		case ID_MISC_SETSCOREBOARD:
			SetScoreBoard();
			break;
		case ID_MISC_SETDRIVEBY:
			SetDriveBy();
			break;
		case ID_MISC_KICK:
			KickPlayer();
			break;
		case ID_MISC_BANID:
			BanPlayer();
			break;
		case ID_MISC_BANIP:
			BanIP();
			break;
		case ID_MISC_UNBAN:
			Unban();
			break;
		case ID_MISC_CHATEXCEPT:
			MessageAllExcept();
			break;
		case ID_MISC_CHATPLAYER:
			MessagePlayer();
			break;
		case ID_MISC_CHATTOPLAYER:
			SendPlayerMessage();
			break;
		case ID_MISC_SETSERVNAME:
			SetServerName();
			break;
		case ID_MISC_SETGAMEMODE:
			SetGamemodeName();
			break;
		}
	}
}

void CMiscRecvPackets::AdminChat( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		char szText[ MAX_TEXT_LEN ] = { NULL };

		stringCompressor->DecodeString( szText, MAX_TEXT_LEN, &bs );

		char szOutput[ 256 ] = { 0 };
		strcpy( szOutput, pRcon->GetNick() );
		strcat( szOutput, ": " );
		strncat( szOutput, szText, 256 );

		CConsole::PrintC2( "RCON CHAT", COLOUR_YELLOW, szOutput );

		CMiscSendPackets::AdminChat( (unsigned char)m_pPacket->playerIndex, szText );
		CNetSends::AdminChat( szText );
	}
}

void CMiscRecvPackets::RconJoin(void)
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		char szNick[ MAX_NAME_LEN ] = { NULL };
		unsigned short usVersion = 0;

		bs.Read(usVersion);

		if (usVersion != RCON_VERSION)
		{
			CMiscSendPackets::VersionReject( pRcon->GetID(), REJECT_REASON_BAD_VERSION );

			return;
		}

		stringCompressor->DecodeString( szNick, MAX_NAME_LEN, &bs );

		unsigned int uiNickLen = strlen(szNick);

		if ((uiNickLen == 0) || (uiNickLen > 16))
		{
			CMiscSendPackets::VersionReject( pRcon->GetID(), REJECT_REASON_BAD_NICKNAME );

			return;
		}

		pRcon->SetNick( szNick );

		CMiscSendPackets::ServerData( pRcon->GetID() );
		CMiscSendPackets::RconList( pRcon->GetID() );
		CMiscSendPackets::PlayerList( pRcon->GetID() );
		CMiscSendPackets::VehicleList( pRcon->GetID() );
		CMiscSendPackets::PickupList( pRcon->GetID() );
		CMiscSendPackets::AmmuList( pRcon->GetID() );
	}
}

void CMiscRecvPackets::AdminPM( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		char szText[ MAX_TEXT_LEN ] = { NULL };

		bs.ReadCompressed(uc);
		stringCompressor->DecodeString( szText, MAX_TEXT_LEN, &bs );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			char szOutput[ 256 ] = { 0 };
			strcpy( szOutput, pRcon->GetNick() );
			strcat( szOutput, ": To: " );
			strcat( szOutput, pPlayer->GetNick() );
			strcat( szOutput, " Message: " );
			strncat( szOutput, szText, 256 );

			CConsole::PrintC2( "RCON PM", COLOUR_YELLOW, szOutput );

			CMiscSendPackets::AdminPM( (unsigned char)m_pPacket->playerIndex, uc, szText );
			CNetSends::AdminMsg( uc, szText );
		}
	}
}

void CMiscRecvPackets::Announce( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		char szText[ MAX_TEXT_LEN ] = { NULL };

		bs.ReadCompressed(uc);
		stringCompressor->DecodeString( szText, MAX_TEXT_LEN, &bs );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			char szOutput[ 256 ] = { 0 };
			strcpy( szOutput, pRcon->GetNick() );
			strcat( szOutput, ": To: " );
			strcat( szOutput, pPlayer->GetNick() );
			strcat( szOutput, " Message: " );
			strncat( szOutput, szText, 256 );

			CConsole::PrintC2( "RCON ANNOUNCE", COLOUR_YELLOW, szOutput );

			CMiscSendPackets::Announce( (unsigned char)m_pPacket->playerIndex, uc, szText );
			CNetSends::Announce( uc, szText );
		}
	}
}

void CMiscRecvPackets::AnnounceAll( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		char szText[ MAX_TEXT_LEN ] = { NULL };

		stringCompressor->DecodeString( szText, MAX_TEXT_LEN, &bs );

		CMiscSendPackets::AnnounceAll( (unsigned char)m_pPacket->playerIndex, szText );

		unsigned char uc = 0;
		while ( ( uc < MAX_PLAYERS ) && ( uc < CPlayerPoolManager::Count() ) )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				CNetSends::Announce( uc, szText );
			}
			uc++;
		}
	}
}

void CMiscRecvPackets::SetPassword( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		char szPassword[ MAX_PASS_LEN ] = { NULL };

		stringCompressor->DecodeString(szPassword, MAX_PASS_LEN, &bs);

		CConfigLoader::SetPassword( szPassword );
		CNet::GetInterface()->SetPassword( szPassword );
		pServerInfo.bLocked = true;
	}
}

void CMiscRecvPackets::SetMaxPlayers( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed( uc );

		if ( uc > MAX_PLAYERS ) uc = MAX_PLAYERS;

		pServerInfo.ucMaxPlayers = uc;
		CConfigLoader::SetMaxPlayers( uc );
		NetInterface->SetAllowedPlayers( uc );
	}
}

void CMiscRecvPackets::SetScoreBoard( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		bool b = true;

		bs.Read(b);

		CConfigLoader::SetDeathmatchScoreB( b );
	}
}

void CMiscRecvPackets::SetDriveBy( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		bool b = true;

		bs.Read( b );

		CConfigLoader::SetDrivebyEnabled( b );
	}
}

void CMiscRecvPackets::KickPlayer( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed( uc );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CNetSends::KickPlayer( uc );
		}
	}
}

void CMiscRecvPackets::BanPlayer( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed( uc );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CNetSends::BanPlayer( uc );
		}
	}
}

void CMiscRecvPackets::BanIP( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		char szIP[ MAX_IP_LEN ] = { NULL };

		stringCompressor->DecodeString( szIP, MAX_IP_LEN, &bs );

		CBanList::AddToBanList( szIP );
	}
}

void CMiscRecvPackets::Unban( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		char szIP[ MAX_IP_LEN ] = { NULL };

		stringCompressor->DecodeString( szIP, MAX_IP_LEN, &bs );

		CBanList::RemoveFromBanList( szIP );
	}
}

void CMiscRecvPackets::MessageAllExcept( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		char szText[ MAX_TEXT_LEN ] = { NULL };

		bs.ReadCompressed( uc );
		stringCompressor->DecodeString( szText, MAX_TEXT_LEN, &bs );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CNetSends::AdminChatAllExcept( pPlayer->GetPlayerID(), szText );
		}
	}
}

void CMiscRecvPackets::MessagePlayer( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		char szText[ MAX_TEXT_LEN ] = { NULL };

		bs.ReadCompressed( uc );
		stringCompressor->DecodeString( szText, MAX_TEXT_LEN, &bs );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CNetSends::AdminChatToPlayer( pPlayer->GetPlayerID(), szText );
		}
	}
}

void CMiscRecvPackets::SendPlayerMessage( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;
		char szText[ MAX_TEXT_LEN ] = { NULL };

		bs.ReadCompressed( uc1 );
		bs.ReadCompressed( uc2 );
		stringCompressor->DecodeString( szText, MAX_TEXT_LEN, &bs );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc2 );
			if ( pPlayerTo )
			{
				CNetSends::ChatToPlayer( pPlayer->GetID(), pPlayerTo->GetID(), szText );
			}
		}
	}
}

void CMiscRecvPackets::SetServerName( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		char szName[ MAX_SERVNAME_LEN ] = { NULL };

		stringCompressor->DecodeString(szName, MAX_SERVNAME_LEN, &bs);

		pServerInfo.ucServerNameLen = strlen( szName );
		strcpy( pServerInfo.szServerName, szName );
		CConfigLoader::SetServerName( szName );
	}
}

void CMiscRecvPackets::SetGamemodeName( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		char szMode[ MAX_GAMEMODE_LEN ] = { NULL };

		stringCompressor->DecodeString(szMode, MAX_GAMEMODE_LEN, &bs);
		strcpy( pServerInfo.szMode, szMode );
		CConfigLoader::SetGamemodeName( szMode );
	}
}