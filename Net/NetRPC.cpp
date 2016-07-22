/*
	NetRPC.cpp
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

#include "NetRPC.h"
#include "NetSends.h"
#include <math.h>
#include "Pools.h"
#include "BanList.h"
#include "../Misc/KeyBinds.h"
#include "Utils.h"

RakServerInterface* pRak = 0;

extern SERVER_INFO pServerInfo;

void ClientJoin(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	RakNet::BitStream bsReject;

	char szPlayerName[ MAX_NAME_LEN ] = { 0 };
	BYTE bytePlayerID = 0;
	BYTE bytePlayerNameLen = 0;
	unsigned int iVer = 0;

	bsData.Read( iVer );

	if ( iVer != NETGAME_VERSION )
	{
		bsReject.Write( (BYTE)REJECT_REASON_BAD_VERSION );
		pRak->RPC( "ConnectionRejected", &bsReject, HIGH_PRIORITY, RELIABLE, 0, sender, FALSE, FALSE );
		return;
	}

	bsData.Read( bytePlayerNameLen );
	bsData.Read( szPlayerName, bytePlayerNameLen );
	szPlayerName[ bytePlayerNameLen ] = '\0';

	FilterInvalidNickChars( szPlayerName );
	bytePlayerNameLen = strlen( szPlayerName );

	if ( ( bytePlayerNameLen == 0 ) || ( bytePlayerNameLen > 32 ) || ( CPlayerPoolManager::FindExact( szPlayerName ) ) )
	{
		bsReject.Write( (BYTE)REJECT_REASON_BAD_NICKNAME );
		pRak->RPC( "ConnectionRejected", &bsReject, HIGH_PRIORITY, RELIABLE, 0, sender, FALSE, FALSE );
		return;
	}

	bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		pPlayer->SetNick( szPlayerName );
		CConsole::PrintC( "CONNECTION", COLOUR_GREEN, "Accepted [%s] | Index [%i] | Name [%s]", pPlayer->GetIP(), bytePlayerID, szPlayerName );
	}

	CNetSends::InitGame( bytePlayerID );
	CNetSends::PlayerJoin( bytePlayerID );

	CScripting::PushFunction( "onPlayerJoin" );
	CScripting::PushPlayerPointer( pPlayer );
	CScripting::CallFunction();

	CPlayerSendPackets::Join( bytePlayerID, szPlayerName, pPlayer->GetIP() );

	// Send the player list
	unsigned char uc = 0, uc1 = 0;
	pPlayer = NULL;
	RakNet::BitStream bsSend;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
	{
		pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			if ( uc != bytePlayerID )
			{
				unsigned int ucNickLen = strlen( pPlayer->GetNick() );
				bsSend.Write( uc );

				bsSend.Write( ucNickLen );
				bsSend.Write( pPlayer->GetNick(), ucNickLen );

				pRak->RPC( "ServerJoin", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, FALSE, FALSE );

				if ( pPlayer->GetSpawned() ) CNetSends::SpawnForPlayer( bytePlayerID, uc );

				bsSend.Reset();
			}
			uc1++;
		}
		uc++;
	}

	uc = 1;
	uc1 = 0;
	CVehiclePool* pVehicle = NULL;
	while ( ( uc < MAX_VEHICLES ) && ( uc1 < CVehiclePoolManager::Count() ) )
	{
		pVehicle = CVehiclePoolManager::Find( uc );
		if ( pVehicle )
		{
			if ( pVehicle->GetSpawned() ) CNetSends::SpawnVehicleForPlayer( bytePlayerID, uc );

			uc1++;
		}
		uc++;
	}

	unsigned int ui = 0, ui1 = 0;
	CPickupPool* pPickup = NULL;
	while ( ( ui < MAX_PICKUPS ) && ( ui1 < CPickupPoolManager::Count() ) )
	{
		pPickup = CPickupPoolManager::Find( ui );
		if ( pPickup )
		{
			if ( pPickup->GetSpawned() ) CNetSends::SpawnPickupForPlayer( bytePlayerID, ui, bytePlayerID );

			ui1++;
		}
		ui++;
	}

	CNetSends::SetWeather( CConfigLoader::GetWeather() );
	CNetSends::SetHour( CConfigLoader::GetHour() );
	CNetSends::SetMinute( CConfigLoader::GetMinute() );
}

void Quit(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE byteQuitReason = 0;

	bsData.Read( byteQuitReason );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CConsole::PrintC( "CONNECTION", COLOUR_RED, "Closed [%s] | Index [%i]", pPlayer->GetIP(), bytePlayerID );

		CNetSends::PlayerPart( pPlayer->GetID(), PARTREASON_CLOSED );
		CPlayerSendPackets::Part( pPlayer->GetID(), PARTREASON_CLOSED );

		CScripting::PushFunction( "onPlayerPart" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( PARTREASON_CLOSED );
		CScripting::CallFunction();

		CPlayerPoolManager::Remove( pPlayer );
	}
}

void Chat(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteTextLen = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( byteTextLen );
	bsData.Read( szText, byteTextLen );
	szText[ byteTextLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( CConfigLoader::GetOutputConsole() ) 
		{
			char szOutput[ 256 ] = { 0 };
			strcpy( szOutput, pPlayer->GetNick() );
			strcat( szOutput, ": " );
			strncat( szOutput, szText, 256 );
			CConsole::PrintC2( "CHAT", COLOUR_YELLOW, szOutput );
		}

		CScripting::PushFunction( "onPlayerChat" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushString( szText );
		CScripting::CallFunction();

		CNetSends::Chat( bytePlayerID, szText );
		CPlayerSendPackets::Chat( bytePlayerID, szText );
	}
}

void ME(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteTextLen = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( byteTextLen );
	bsData.Read( szText, byteTextLen );
	szText[ byteTextLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( CConfigLoader::GetOutputConsole() ) 
		{
			char szOutput[ 256 ] = { 0 };
			strcpy( szOutput, "* " );
			strcat( szOutput, pPlayer->GetNick() );
			strcat( szOutput, " " );
			strncat( szOutput, szText, 256 );
			CConsole::PrintC2( "ACTION", COLOUR_YELLOW, szOutput );
		}

		CScripting::PushFunction( "onPlayerAction" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushString( szText );
		CScripting::CallFunction();

		CNetSends::Action( bytePlayerID, szText );
		CPlayerSendPackets::Action( bytePlayerID, szText );
	}
}

// Private Message
void MsgChat(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteTextLen = 0;
	BYTE bytePlayerTo = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( bytePlayerTo );

	bsData.Read( byteTextLen );
	bsData.Read( szText, byteTextLen );
	szText[ byteTextLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( bytePlayerTo );
		if ( pPlayerTo )
		{
			if ( CConfigLoader::GetOutputConsole() ) 
			{
				char szOutput[ 256 ] = { 0 };
				strcpy( szOutput, pPlayer->GetNick() );
				strcat( szOutput, " -> " );
				strcat( szOutput, pPlayerTo->GetNick() );
				strcat( szOutput, ": " );
				strncat( szOutput, szText, 256 );
				CConsole::PrintC2( "PM", COLOUR_YELLOW, szOutput );
			}

			CScripting::PushFunction( "onPlayerPM" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushPlayerPointer( pPlayerTo );
			CScripting::PushString( szText );
			CScripting::CallFunction();

			CNetSends::PM( bytePlayerID, bytePlayerTo, szText );
			CPlayerSendPackets::PM( 0, bytePlayerID, bytePlayerTo, szText );
		}
		else
		{
			if ( CConfigLoader::GetOutputConsole() )
			{
				char szOutput[ 256 ] = { 0 };
				strcpy( szOutput, pPlayer->GetNick() );
				strcat( szOutput, " -> " );
				sprintf( szOutput, "%s%i: ", szOutput, bytePlayerTo );
				strncat( szOutput, szText, 256 );
				CConsole::PrintC2( "FAILED PM", COLOUR_YELLOW, szOutput );
			}


			CScripting::PushFunction( "onPlayerFailedPM" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( bytePlayerTo );
			CScripting::PushString( szText );
			CScripting::CallFunction();

			CPlayerSendPackets::PM( 1, bytePlayerID, bytePlayerTo, szText );
		}
	}
}

//Team Message
void TeamChat(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteTextLen = 0;
	BYTE byteTeamTo = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( byteTeamTo );

	bsData.Read( byteTextLen );
	bsData.Read( szText, byteTextLen );
	szText[ byteTextLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CSpawnPool* pSpawn = CSpawnPoolManager::FindTeam( byteTeamTo );
		if ( ( pSpawn ) || ( byteTeamTo == INVALID_TEAM_ID ) )
		{
			if ( CConfigLoader::GetOutputConsole() ) 
			{
				char szOutput[ 256 ] = { 0 };
				sprintf( szOutput, "[%i] %s: ", byteTeamTo, pPlayer->GetNick() );
				strncat( szOutput, szText, 256 );

				CConsole::PrintC2( "TEAMCHAT", COLOUR_YELLOW, szOutput );
			}

			CScripting::PushFunction( "onPlayerTeamChat" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( byteTeamTo );
			CScripting::PushString( szText );
			CScripting::CallFunction();

			CNetSends::TeamChat( bytePlayerID, byteTeamTo, szText );
			CPlayerSendPackets::TeamChat( bytePlayerID, byteTeamTo, szText );
		}
	}
}

void RequestClass(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteRequestedClass = 0;
	BYTE byteRequestOutcome = 0;

	bsData.Read( byteRequestedClass );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->GetSpectating() != INVALID_PLAYER_ID )
		{
			CScripting::PushFunction( "onPlayerExitSpectating" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::CallFunction();

			CPlayerSendPackets::StopSpectating( bytePlayerID );
		}
		pPlayer->SetSpectating( INVALID_PLAYER_ID );

		CSpawnPool* pSpawn = CSpawnPoolManager::Find( byteRequestedClass );
		if ( !pSpawn )
		{
			byteRequestOutcome = 0;
			pSpawn = CSpawnPoolManager::Find( pPlayer->GetClassID() );
		}
		else byteRequestOutcome = 1;

		if ( pSpawn )
		{
			RakNet::BitStream bsSpawnRequestReply;

			bsSpawnRequestReply.Write( byteRequestOutcome );
			bsSpawnRequestReply.Write( pSpawn->GetTeam() );
			bsSpawnRequestReply.Write( pSpawn->GetSkin() );
			bsSpawnRequestReply.Write( (float)pSpawn->GetX() );
			bsSpawnRequestReply.Write( (float)pSpawn->GetY());
			bsSpawnRequestReply.Write( (float)pSpawn->GetZ());
			bsSpawnRequestReply.Write( (float)pSpawn->GetAngle());
			bsSpawnRequestReply.Write( pSpawn->GetWeapon( 0 ) );
			bsSpawnRequestReply.Write( pSpawn->GetWeaponAmmo( 0 ) );
			bsSpawnRequestReply.Write( pSpawn->GetWeapon( 1 ) );
			bsSpawnRequestReply.Write( pSpawn->GetWeaponAmmo( 1 ) );
			bsSpawnRequestReply.Write( pSpawn->GetWeapon( 2 ) );
			bsSpawnRequestReply.Write( pSpawn->GetWeaponAmmo( 2 ) );

			CScripting::PushFunction( "onPlayerRequestClass" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( byteRequestedClass );
			CScripting::PushInteger( pSpawn->GetTeam() );
			CScripting::PushInteger( pSpawn->GetSkin() );
			CScripting::CallFunction();

			CPlayerSendPackets::RequestClass( bytePlayerID, byteRequestedClass, pSpawn->GetTeam(), pSpawn->GetSkin() );

			NetInterface->RPC( "RequestClass", &bsSpawnRequestReply, HIGH_PRIORITY, RELIABLE, 0, sender, FALSE, FALSE );

			pPlayer->SetClassID( byteRequestedClass );
		}
	}
}

void Spawn(PCHAR Data, int iBitLength, PlayerID sender)
{
	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->GetSpectating() != INVALID_PLAYER_ID )
		{
			CScripting::PushFunction( "onPlayerExitSpectating" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::CallFunction();

			CPlayerSendPackets::StopSpectating( bytePlayerID );
		}
		pPlayer->SetSpectating( INVALID_PLAYER_ID );
		pPlayer->SetSpawned( true );

		CSpawnPool* pSpawn = CSpawnPoolManager::Find( pPlayer->GetClassID() );
		if ( pSpawn )
		{
			pPlayer->SetX( pSpawn->GetX() );
			pPlayer->SetY( pSpawn->GetY() );
			pPlayer->SetZ( pSpawn->GetZ() );
			pPlayer->SetRotation( pSpawn->GetAngle() );

			CNetSends::SpawnForWorld( bytePlayerID );
			CPlayerSendPackets::Spawn( bytePlayerID, pSpawn->GetSkin(), pSpawn->GetTeam() );

			CScripting::PushFunction( "onPlayerSpawn" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::CallFunction();
		}
	}
}

void Death(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteDeathReason = 0;
	BYTE byteWhoWasResponsible = 0;

	bsData.Read( byteDeathReason );
	bsData.Read( byteWhoWasResponsible );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		pPlayer->SetSpawned( false );

		if ( byteWhoWasResponsible == bytePlayerID )
		{
			if ( CConfigLoader::GetOutputConsole() ) CConsole::PrintC( "DEATH", COLOUR_RED, "%s killed himself.", pPlayer->GetNick() );
		}
		else if ( byteWhoWasResponsible == INVALID_PLAYER_ID )
		{
			if ( CConfigLoader::GetOutputConsole() ) CConsole::PrintC( "DEATH", COLOUR_RED, "%s died.", pPlayer->GetNick() );
		}
		else
		{
			CPlayerPool* pKiller = CPlayerPoolManager::Find( byteWhoWasResponsible );
			if ( pKiller )
			{
				if ( CConfigLoader::GetOutputConsole() ) CConsole::PrintC( "DEATH", COLOUR_RED, "%s killed %s.", pKiller->GetNick(), pPlayer->GetNick() );
			}
			else
			{
				if ( CConfigLoader::GetOutputConsole() ) CConsole::PrintC( "DEATH", COLOUR_RED, "%s died.", pPlayer->GetNick() );
			}
		}

		CNetSends::Death( bytePlayerID, byteWhoWasResponsible, byteDeathReason );
	}
}

void EnterVehicle(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteVehicleID = 0;
	BYTE bytePassenger = 0;

	bsData.Read( byteVehicleID );
	bsData.Read( bytePassenger );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( byteVehicleID );
		if ( pVehicle )
		{
			if (!pVehicle->GetLocked())
			{
				pPlayer->SetEnterVehicleTime( RakNet::GetTime() );
				pPlayer->SetVehicle( byteVehicleID );
				pPlayer->SetVehiclePassenger( bytePassenger );

				if ( bytePassenger == 18 )
				{
					pVehicle->SetDriver( bytePlayerID );
					CPlayerSendPackets::EnterVehicle( bytePlayerID, pVehicle->GetID(), pVehicle->GetModel(), pVehicle->GetHealth(), 0 );
				}
				else
				{
					CPlayerSendPackets::EnterVehicle( bytePlayerID, pVehicle->GetID(), pVehicle->GetModel(), pVehicle->GetHealth(), 1 );
				}
				
				CScripting::PushFunction( "onPlayerEnterVehicle" );
				CScripting::PushPlayerPointer( pPlayer );
				CScripting::PushVehiclePointer( pVehicle );
				CScripting::PushBool( bytePassenger != 18 ? true : false );
				CScripting::CallFunction();

				pVehicle->SetLastUsedTime( RakNet::GetTime() );
				pVehicle->SetBeenUsed( true );
			}
		}

		CNetSends::EnterVehicle( bytePlayerID, byteVehicleID );
	}
}

void ExitVehicle(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteVehicleID = 0;

	bsData.Read( byteVehicleID );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( pPlayer->GetVehicle() );
		if ( pVehicle ) 
		{
			if (pVehicle->GetDriver() == bytePlayerID) CPlayerSendPackets::ExitVehicle( bytePlayerID, pVehicle->GetID(), pVehicle->GetModel(), pVehicle->GetHealth() );

			pVehicle->SetLastUsedTime( RakNet::GetTime() );
			pVehicle->SetBeenUsed( true );

			pVehicle->SetDriver( INVALID_PLAYER_ID );

			CScripting::PushFunction( "onPlayerExitVehicle" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushVehiclePointer( pVehicle );
			CScripting::PushBool( pPlayer->GetVehiclePassenger() != 18 ? true : false );
			CScripting::CallFunction();
		}

		pPlayer->SetVehicle( -1 );
		pPlayer->SetVehiclePassenger( 0 );
		pPlayer->SetVehicleSeat( 255 );

		CNetSends::ExitVehicle( bytePlayerID, byteVehicleID );
	}
}

void UpdateScoreAndPing(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CNetSends::UpdateScoreAndPing( bytePlayerID );
}

void Admin(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szPass[ MAX_PASS_LEN ] = { NULL };
	int iPassLen = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( iPassLen );
	bsData.Read( szPass, iPassLen );
	szPass[ iPassLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( !strcmp( szPass, CConfigLoader::GetRconPassword() ) )
		{
			CConsole::PrintC( "ADMIN", COLOUR_GREEN, "%s (IP=%s) Logged In.", pPlayer->GetNick(), pPlayer->GetIP() );
			pPlayer->SetAdmin( true );

			CScripting::PushFunction( "onPlayerRconLogin" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::CallFunction();

			CNetSends::SetAdmin( bytePlayerID );
		}
		else
		{
			CScripting::PushFunction( "onPlayerRconLoginAttempt" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushString( szPass );
			CScripting::CallFunction();

			char szOutput[ 256 ] = { 0 };
			strcpy( szOutput, pPlayer->GetNick() );
			strcat( szOutput, " (IP=" );
			strcat( szOutput, pPlayer->GetIP() );
			strcat( szOutput, ") failed to login (Password=" );
			strcat( szOutput, szPass );
			strncat( szOutput, ").", 256 );

			CConsole::PrintC2( "ADMIN", COLOUR_RED, szOutput );
			CNetSends::NotAdmin( bytePlayerID );
		}
	}
}

void KickPlayer(PCHAR Data, int iBitLength, PlayerID sender)
{
	BYTE bytePlayer = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( bytePlayer );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->IsAdmin() )
		{
			CPlayerPool* pKicked = CPlayerPoolManager::Find( bytePlayer );
			if ( pKicked ) CNetSends::KickPlayer( bytePlayer );
		}
		else CNetSends::NotAdmin( bytePlayerID );
	}
}

void Ban(PCHAR Data, int iBitLength, PlayerID sender)
{
	BYTE bytePlayer = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( bytePlayer );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->IsAdmin() )
		{
			CPlayerPool* pKicked = CPlayerPoolManager::Find( bytePlayer );
			if ( pKicked ) CNetSends::BanPlayer( bytePlayer );
		}
		else CNetSends::NotAdmin( bytePlayerID );
	}
}

void BanIPAddress(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szIP[ MAX_PASS_LEN ] = { NULL };
	int iIPLen = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( iIPLen );
	bsData.Read( szIP, iIPLen );
	szIP[ iIPLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->IsAdmin() )
		{
			CBanList::AddToBanList( szIP );

			CConsole::PrintC( "ADMIN", COLOUR_RED, "%s Banned IP: %s", pPlayer->GetNick(), szIP );
		}
		else CNetSends::NotAdmin( bytePlayerID );
	}
}

void PickedUp(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	unsigned int uiPickup = 0;

	bsData.Read( uiPickup );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CPickupPool* pPickup = CPickupPoolManager::Find( uiPickup );
		if ( pPickup )
		{
			pPickup->SetSpawned( false ); // This is really set picked up in a way
			pPickup->SetPickedUpTime( RakNet::GetTime() );

			CNetSends::PickedUpPickup( bytePlayerID, uiPickup );

			CScripting::PushFunction( "onPickupPickedUp" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushPickupPointer( pPickup );
			CScripting::CallFunction();

			if ( pPickup->GetOneTime() )
			{
				// Send to rcon
				CPickupSendPackets::DelPickup( uiPickup );

				CScripting::PushFunction( "onPickupDestroy" );
				CScripting::PushPickupPointer( pPickup );
				CScripting::CallFunction();

				// Remove from pools
				CPickupPoolManager::Remove( pPickup );
			}
		}
	}
}

void DoAnim(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteAnimation = 0;

	bsData.Read( byteAnimation );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->GetSpawned() )
		{
			CNetSends::DoAnim( bytePlayerID, byteAnimation );

			CPlayerSendPackets::DoAnim( bytePlayerID, byteAnimation );
		}
	}
}

void Spec(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE bytePlayer = 0;

	bsData.Read( bytePlayer );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( !pPlayer->GetSpawned() )
		{
			CPlayerPool* pSpecPlayer = CPlayerPoolManager::Find( bytePlayer );
			if ( pSpecPlayer )
			{
				if ( pSpecPlayer->GetSpawned() )
				{
					CScripting::PushFunction( "onPlayerStartSpectating" );
					CScripting::PushPlayerPointer( pPlayer );
					CScripting::PushPlayerPointer( CPlayerPoolManager::Find( bytePlayer ) );
					CScripting::CallFunction();

					pPlayer->SetSpectating( bytePlayer );
					CNetSends::SpecOn( bytePlayerID );

					CPlayerSendPackets::StartSpectating( bytePlayerID );
				}
			}
		}
	}
}

void Commanda(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteTextLen = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( byteTextLen );
	bsData.Read( szText, byteTextLen );
	szText[ byteTextLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( CConfigLoader::GetOutputConsole() )
		{
			char szOutput[ 256 ] = { 0 };
			strcpy( szOutput, pPlayer->GetNick() );
			strcat( szOutput, ": " );
			strncat( szOutput, szText, 256 );
			
			CConsole::PrintC2( "COMMAND", COLOUR_YELLOW, szOutput );
		}

		char szCmd[ 32 ] = { NULL };
		char sz[ MAX_TEXT_LEN ] = { NULL };
		char szTmp[ MAX_TEXT_LEN ] = { NULL };
		strncpy( szTmp, szText, MAX_TEXT_LEN );
		BYTE bTokens = CUtils::NumTok( szTmp, " " );
		if ( bTokens > 1 )
		{
			strncpy( szCmd, strtok( szTmp, " " ), 32 );
			strncpy( sz, strtok( NULL, "\n" ), MAX_TEXT_LEN );
		}
		else
		{
			strncpy( szCmd, strtok( szTmp, " " ), 32 );
			strcpy( sz, "" );
		}

		CScripting::PushFunction( "onPlayerCommand" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushString( szCmd );
		CScripting::PushString( sz );
		CScripting::CallFunction();

		CPlayerSendPackets::Command( bytePlayerID, szText );
	}
}

void SetHr(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteHour = 0;

	bsData.Read( byteHour );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->IsAdmin() ) CNetSends::SetHour( byteHour );
		else CNetSends::NotAdmin( bytePlayerID );
	}
}

void SetMin(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteMinute = 0;

	bsData.Read( byteMinute );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->IsAdmin() ) CNetSends::SetMinute( byteMinute );
		else CNetSends::NotAdmin( bytePlayerID );
	}
}

void SetWeather(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteWeather = 0;

	bsData.Read( byteWeather );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->IsAdmin() ) 
		{
			CConfigLoader::SetWeather( byteWeather );
			CNetSends::SetWeather( byteWeather );
		}
		else CNetSends::NotAdmin( bytePlayerID );
	}
}

void SetTimeRate(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	BYTE byteTimeRate = 0;

	bsData.Read( byteTimeRate );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID(sender);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->IsAdmin() )
		{
			CConfigLoader::SetTimeRate( byteTimeRate );

			CGameSendPackets::UpdateTimeRate( byteTimeRate );
		}
		else CNetSends::NotAdmin( bytePlayerID );
	}
}

void Set(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteTextLen = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( byteTextLen );
	bsData.Read( szText, byteTextLen );
	szText[ byteTextLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CNetSends::Set( szText );
	}
}

void SetA(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteTextLen = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	bsData.Read( byteTextLen );
	bsData.Read( szText, byteTextLen );
	szText[ byteTextLen ] = '\0';

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->IsAdmin() ) CNetSends::Set( szText );
		else CNetSends::NotAdmin( bytePlayerID );
	}
}

void RequestAmmu(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteAmmunation = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	bsData.Read( byteAmmunation );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CAmmuPool* pAmmu = CAmmuPoolManager::Find( byteAmmunation );
		if ( pAmmu )
		{
			// Send the ammu
			CNetSends::RequestAmmu( bytePlayerID, byteAmmunation );

			CScripting::PushFunction( "onPlayerRequestAmmunation" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( byteAmmunation );
			CScripting::CallFunction();

			CPlayerSendPackets::RequestAmmunation( bytePlayerID, byteAmmunation );
		}
	}
}

void RequestWep(PCHAR Data, int iBitLength, PlayerID sender)
{
	char szText[ 256 ] = { NULL };
	BYTE byteAmmunation = 0;
	unsigned int iWeaponID = 0;

	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);

	bsData.Read( byteAmmunation );
	bsData.Read( iWeaponID );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( sender );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		CAmmuPool* pAmmu = CAmmuPoolManager::Find( byteAmmunation );
		if ( pAmmu )
		{
			// Send the wep
			CNetSends::RequestWep( bytePlayerID, byteAmmunation, iWeaponID );

			CScripting::PushFunction( "onPlayerRequestAmmuWeapon" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( byteAmmunation );
			CScripting::PushInteger( iWeaponID );
			CScripting::CallFunction();

			CPlayerSendPackets::RequestAmmuWeapon( bytePlayerID, byteAmmunation, iWeaponID );
		}
	}
}

void RegisterRPCs( RakServerInterface * pRakServer )
{
	pRak = pRakServer;

	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ClientJoin);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Quit);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Chat);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ME);
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, MsgChat );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, TeamChat );
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, RequestClass);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Spawn);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Death);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, EnterVehicle);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ExitVehicle);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, UpdateScoreAndPing);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Admin);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, KickPlayer);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Ban);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, BanIPAddress);
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, PickedUp );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, DoAnim );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, Spec );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, Commanda );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetHr );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetMin );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetWeather );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetTimeRate );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, Set );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetA );
	// Amunation Stuff
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, RequestAmmu );
	REGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, RequestWep );
}

//----------------------------------------------------

void UnRegisterRPCs( RakServerInterface * pRakServer )
{
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ClientJoin);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Quit);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Chat);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ME);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, MsgChat );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, TeamChat );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, RequestClass);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Spawn);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Death);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, EnterVehicle);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, ExitVehicle);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, UpdateScoreAndPing);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Admin);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, KickPlayer);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, Ban);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakServer, BanIPAddress);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, PickedUp );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, DoAnim );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, Commanda );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetHr );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetMin );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetWeather );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetTimeRate );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, Set );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, SetA );
	// Amunation Stuff
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, RequestAmmu );
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL( pRakServer, RequestWep );

	pRak = 0;
}


void PlayerSync( Packet* pPacket )
{
	RakNet::BitStream bsPlayerSync((PCHAR)pPacket->data, ( pPacket->bitSize / 8 ), FALSE);

	BYTE bytePacketID = 0;
	ONFOOT_SYNC_DATA ofSync;

	memset( &ofSync, 0, sizeof( ONFOOT_SYNC_DATA ) );

	bsPlayerSync.Read( bytePacketID );
	bsPlayerSync.Read( (char*)&ofSync, sizeof( ONFOOT_SYNC_DATA ) );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( pPacket->playerId );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->GetKeys() != ofSync.wKeys ) CKeyBinds::PlayerKeySync( pPlayer, (int)pPlayer->GetKeys(), (int)ofSync.wKeys );

		if ( pPlayer->GetHealth() != ofSync.byteHealth )
		{
			CScripting::PushFunction( "onPlayerHealthChange" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( pPlayer->GetHealth() );
			CScripting::PushInteger( ofSync.byteHealth );
			CScripting::CallFunction();
		}

		if ( pPlayer->GetArmour() != ofSync.byteArmour )
		{
			CScripting::PushFunction( "onPlayerArmourChange" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( pPlayer->GetArmour() );
			CScripting::PushInteger( ofSync.byteArmour );
			CScripting::CallFunction();
		}

		if ( pPlayer->GetWeapon() != ofSync.byteCurrentWeapon )
		{
			CScripting::PushFunction( "onPlayerWeaponChange" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( pPlayer->GetWeapon() );
			CScripting::PushInteger( ofSync.byteCurrentWeapon );
			CScripting::CallFunction();
		}

		if ( pPlayer->GetCash() != ofSync.iCash )
		{
			CScripting::PushFunction( "onPlayerCashChange" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( pPlayer->GetCash() );
			CScripting::PushInteger( ofSync.iCash );
			CScripting::CallFunction();
		}

		if ( ( pPlayer->GetVehicle() != -1 ) && ( ( RakNet::GetTime() - pPlayer->GetEnterVehicleTime() ) >= 5000 ) )
		{
			CVehiclePool* pVehicle = CVehiclePoolManager::Find( pPlayer->GetVehicle() );
			if ( pVehicle )
			{
				if ( pVehicle->GetDriver() == pPlayer->GetID() )
				{
					CPlayerSendPackets::ExitVehicle( bytePlayerID, pVehicle->GetID(), pVehicle->GetModel(), pVehicle->GetHealth() );

					CScripting::PushFunction( "onPlayerExitVehicle" );
					CScripting::PushPlayerPointer( pPlayer );
					CScripting::PushVehiclePointer( pVehicle );
					CScripting::PushBool( pPlayer->GetVehiclePassenger() != 18 ? true : false );
					CScripting::CallFunction();

					pVehicle->SetLastUsedTime( RakNet::GetTime() );
					pVehicle->SetBeenUsed( true );

					pVehicle->SetDriver( INVALID_PLAYER_ID );
				}

				pPlayer->SetVehicle( -1 );
				pPlayer->SetVehiclePassenger( 0 );
				pPlayer->SetVehicleSeat( 255 );
			}
		}

		if ( ( pPlayer->GetX() != ofSync.vecPos.X ) || ( pPlayer->GetY() != ofSync.vecPos.Y ) || ( pPlayer->GetZ() != ofSync.vecPos.Z ) )
		{
			CScripting::PushFunction( "onPlayerMove" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushFloat( pPlayer->GetX() );
			CScripting::PushFloat( pPlayer->GetY() );
			CScripting::PushFloat( pPlayer->GetZ() );
			CScripting::PushFloat( ofSync.vecPos.X );
			CScripting::PushFloat( ofSync.vecPos.Y );
			CScripting::PushFloat( ofSync.vecPos.Z );
			CScripting::CallFunction();
		}

		pPlayer->SetKeys( ofSync.wKeys );
		pPlayer->SetX( ofSync.vecPos.X );
		pPlayer->SetY( ofSync.vecPos.Y );
		pPlayer->SetZ( ofSync.vecPos.Z );

		pPlayer->SetRotation( ofSync.fRotation );
		pPlayer->SetHealth( ofSync.byteHealth );
		pPlayer->SetArmour( ofSync.byteArmour );
		pPlayer->SetAction( ofSync.byteAction );

		pPlayer->SetWeapon( ofSync.byteCurrentWeapon );
		pPlayer->SetCash( ofSync.iCash );

		CNetSends::PlayerSync( pPlayer->GetID(), ofSync );
		CPlayerSendPackets::FootSync( bytePlayerID, ofSync );

	}
}

void AimSync( Packet* pPacket )
{
	RakNet::BitStream bsPlayerSync((PCHAR)pPacket->data, ( pPacket->bitSize / 8 ), FALSE);

	BYTE bytePacketID = 0;

	ONFOOT_SYNC_DATA ofSync;
	AIM_SYNC_DATA aimSync;

	memset( &ofSync, 0, sizeof( ONFOOT_SYNC_DATA ) );
	memset( &aimSync, 0, sizeof( AIM_SYNC_DATA ) );

	bsPlayerSync.Read( bytePacketID );
	bsPlayerSync.Read( (char*)&ofSync, sizeof( ONFOOT_SYNC_DATA ) );
	bsPlayerSync.Read( (char*)&aimSync, sizeof( AIM_SYNC_DATA ) );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( pPacket->playerId );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->GetKeys() != ofSync.wKeys ) CKeyBinds::AimKeySync( pPlayer, (int)pPlayer->GetKeys(), (int)ofSync.wKeys );

		if ( pPlayer->GetHealth() != ofSync.byteHealth )
		{
			CScripting::PushFunction( "onPlayerHealthChange" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( pPlayer->GetHealth() );
			CScripting::PushInteger( ofSync.byteHealth );
			CScripting::CallFunction();
		}

		if ( pPlayer->GetArmour() != ofSync.byteArmour )
		{
			CScripting::PushFunction( "onPlayerArmourChange" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( pPlayer->GetArmour() );
			CScripting::PushInteger( ofSync.byteArmour );
			CScripting::CallFunction();
		}

		if ( pPlayer->GetWeapon() != ofSync.byteCurrentWeapon )
		{
			CScripting::PushFunction( "onPlayerWeaponChange" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( pPlayer->GetWeapon() );
			CScripting::PushInteger( ofSync.byteCurrentWeapon );
			CScripting::CallFunction();
		}

		if ( ( pPlayer->GetVehicle() != -1 ) && ( ( RakNet::GetTime() - pPlayer->GetEnterVehicleTime() ) >= 5000 ) )
		{
			CVehiclePool* pVehicle = CVehiclePoolManager::Find( pPlayer->GetVehicle() );
			if ( pVehicle )
			{
				if ( pVehicle->GetDriver() == pPlayer->GetID() )
				{
					CPlayerSendPackets::ExitVehicle( bytePlayerID, pVehicle->GetID(), pVehicle->GetModel(), pVehicle->GetHealth() );

					CScripting::PushFunction( "onPlayerExitVehicle" );
					CScripting::PushPlayerPointer( pPlayer );
					CScripting::PushVehiclePointer( pVehicle );
					CScripting::PushBool( pPlayer->GetVehiclePassenger() != 18 ? true : false );
					CScripting::CallFunction();

					pVehicle->SetLastUsedTime( RakNet::GetTime() );
					pVehicle->SetBeenUsed( true );

					pVehicle->SetDriver( INVALID_PLAYER_ID );
				}

				pPlayer->SetVehicle( -1 );
				pPlayer->SetVehiclePassenger( 0 );
				pPlayer->SetVehicleSeat( 255 );
			}
		}

		if ( ( pPlayer->GetX() != ofSync.vecPos.X ) || ( pPlayer->GetY() != ofSync.vecPos.Y ) || ( pPlayer->GetZ() != ofSync.vecPos.Z ) )
		{
			CScripting::PushFunction( "onPlayerMove" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushFloat( pPlayer->GetX() );
			CScripting::PushFloat( pPlayer->GetY() );
			CScripting::PushFloat( pPlayer->GetZ() );
			CScripting::PushFloat( ofSync.vecPos.X );
			CScripting::PushFloat( ofSync.vecPos.Y );
			CScripting::PushFloat( ofSync.vecPos.Z );
			CScripting::CallFunction();
		}

		pPlayer->SetKeys( ofSync.wKeys );
		pPlayer->SetX( ofSync.vecPos.X );
		pPlayer->SetY( ofSync.vecPos.Y );
		pPlayer->SetZ( ofSync.vecPos.Z );
		pPlayer->SetRotation( ofSync.fRotation );

		pPlayer->SetHealth( ofSync.byteHealth );
		pPlayer->SetArmour( ofSync.byteArmour );
		pPlayer->SetAction( ofSync.byteAction );

		pPlayer->SetWeapon( ofSync.byteCurrentWeapon );

		CNetSends::AimSync( pPlayer->GetID(), ofSync, aimSync );
		CPlayerSendPackets::FootSync( bytePlayerID, ofSync );
	}
}

void DecompressVector1(VECTOR * vec, C_VECTOR1 * c1)
{
	vec->X = (float)c1->X;
	vec->X = (float)((double)vec->X / 10000.0);
	vec->Y = (float)c1->Y;
	vec->Y = (float)((double)vec->Y / 10000.0);
	vec->Z = (float)c1->Z;
	vec->Z = (float)((double)vec->Z / 10000.0);
}

void VehicleSync( Packet* pPacket )
{
	RakNet::BitStream bsVehicleSync((PCHAR)pPacket->data, ( pPacket->bitSize / 8 ), FALSE);
	BYTE bytePacketID = 0, byteVehicleID = 0;

	INCAR_SYNC_DATA icSync;

	memset( &icSync, 0, sizeof( INCAR_SYNC_DATA ) );

	bsVehicleSync.Read( bytePacketID );
	bsVehicleSync.Read( byteVehicleID );

	bsVehicleSync.Read( (char*)&icSync, sizeof( INCAR_SYNC_DATA ) );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( pPacket->playerId );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( pPlayer->GetKeys() != icSync.wKeys ) CKeyBinds::VehicleKeySync( pPlayer, (int)pPlayer->GetKeys(), (int)icSync.wKeys );

		if ( pPlayer->GetHealth() != icSync.bytePlayerHealth )
		{
			CScripting::PushFunction( "onPlayerHealthChange" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( pPlayer->GetHealth() );
			CScripting::PushInteger( icSync.bytePlayerHealth );
			CScripting::CallFunction();
		}

		if ( !CConfigLoader::GetDrivebyEnabled() )
		{
			if ( icSync.wKeys & KEY_INVEHICLE_FIRE ) icSync.wKeys -= KEY_INVEHICLE_FIRE;
		}

		pPlayer->SetKeys( icSync.wKeys );
		pPlayer->SetX( icSync.vecPos.X );
		pPlayer->SetY( icSync.vecPos.Y );
		pPlayer->SetZ( icSync.vecPos.Z );

		pPlayer->SetHealth( icSync.bytePlayerHealth );

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( byteVehicleID );
		if ( pVehicle )
		{
			if ( pVehicle->GetDriver() == INVALID_PLAYER_ID )
			{
				pPlayer->SetEnterVehicleTime( RakNet::GetTime() );
				pPlayer->SetVehicle( byteVehicleID );
				pPlayer->SetVehiclePassenger( 18 );
				pVehicle->SetDriver( bytePlayerID );
				pVehicle->SetLastUsedTime( RakNet::GetTime() );
				pVehicle->SetBeenUsed( true );

				CPlayerSendPackets::EnterVehicle( bytePlayerID, pVehicle->GetID(), pVehicle->GetModel(), pVehicle->GetHealth(), 0 );

				CScripting::PushFunction( "onPlayerEnterVehicle" );
				CScripting::PushPlayerPointer( pPlayer );
				CScripting::PushVehiclePointer( pVehicle );
				CScripting::PushBool( false );
				CScripting::CallFunction();
			}

			if ( pVehicle->GetHealth() != icSync.fHealth )
			{
				CScripting::PushFunction( "onVehicleHealthChange" );
				CScripting::PushVehiclePointer( pVehicle );
				CScripting::PushFloat( pVehicle->GetHealth() );
				CScripting::PushFloat( icSync.fHealth );
				CScripting::CallFunction();
			}

			if ( ( pVehicle->GetX() != icSync.vecPos.X ) || ( pVehicle->GetY() != icSync.vecPos.Y ) || ( pVehicle->GetZ() != icSync.vecPos.Z ) )
			{
				CScripting::PushFunction( "onVehicleMove" );
				CScripting::PushPlayerPointer( pPlayer );
				CScripting::PushVehiclePointer( pVehicle );
				CScripting::PushFloat( pVehicle->GetX() );
				CScripting::PushFloat( pVehicle->GetY() );
				CScripting::PushFloat( pVehicle->GetZ() );
				CScripting::PushFloat( icSync.vecPos.X );
				CScripting::PushFloat( icSync.vecPos.Y );
				CScripting::PushFloat( icSync.vecPos.Z );
				CScripting::CallFunction();
			}

			pVehicle->SetX( icSync.vecPos.X );
			pVehicle->SetY( icSync.vecPos.Y );
			pVehicle->SetZ( icSync.vecPos.Z );

			pVehicle->SetHealth( icSync.fHealth );

			pPlayer->SetVehicleSeat( 0 );

			CNetSends::VehicleSync( bytePlayerID, byteVehicleID, icSync );
			CVehicleSendPackets::VehicleSync( bytePlayerID, byteVehicleID, icSync );
		}

		/*printf( "Starting Output...\n" );
		for ( unsigned char uc = 0; uc < 11; uc++ )
		{
			printf( "Pad %i: %i\n", uc, icSync.bytePadding[ uc ] );
		}*/
	}
}

void PassengerSync( Packet* pPacket )
{
	RakNet::BitStream bsPassengerSync((PCHAR)pPacket->data, ( pPacket->bitSize / 8 ), FALSE);

	BYTE bytePacketID = 0, byteVehicleID = 0;
	unsigned int uiPassengerSeat = 0;
	float fX = 0.0f, fY = 0.0f, fZ = 0.0f;

	bsPassengerSync.Read( bytePacketID );

	bsPassengerSync.Read( byteVehicleID );
	bsPassengerSync.Read( uiPassengerSeat );
	bsPassengerSync.Read( fX );
	bsPassengerSync.Read( fY );
	bsPassengerSync.Read( fZ );

	BYTE bytePlayerID = pRak->GetIndexFromPlayerID( pPacket->playerId );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( bytePlayerID );
	if ( pPlayer )
	{
		if ( ( pPlayer->GetX() != fX ) || ( pPlayer->GetY() != fY ) || ( pPlayer->GetZ() != fZ ) )
		{
			CScripting::PushFunction( "onPlayerMove" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushFloat( pPlayer->GetX() );
			CScripting::PushFloat( pPlayer->GetY() );
			CScripting::PushFloat( pPlayer->GetZ() );
			CScripting::PushFloat( fX );
			CScripting::PushFloat( fY );
			CScripting::PushFloat( fZ );
			CScripting::CallFunction();
		}

		pPlayer->SetX( fX );
		pPlayer->SetY( fY );
		pPlayer->SetZ( fZ );
		pPlayer->SetVehicleSeat( uiPassengerSeat );

		CNetSends::PassengerSync( bytePlayerID, byteVehicleID, uiPassengerSeat );
	}
}
