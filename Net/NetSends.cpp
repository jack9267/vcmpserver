/*
	NetSends.cpp
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

#include "NetSends.h"
#include "Pools.h"
#include "ConfigLoader.h"
#include "RangedSends.h"
#include "BanList.h"

void CNetSends::InitGame( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		// Send this back as the 'InitGame' RPC
		RakNet::BitStream bsInitGame;

		VECTOR vInitPlayerPos = CConfigLoader::GetInitPlayerPos();
		VECTOR vInitCameraPos = CConfigLoader::GetInitCameraPos();
		VECTOR vInitCameraLook = CConfigLoader::GetInitCameraLook();

		bsInitGame.Write( (float)vInitPlayerPos.X );
		bsInitGame.Write( (float)vInitPlayerPos.Y );
		bsInitGame.Write( (float)vInitPlayerPos.Z );
		bsInitGame.Write( (float)vInitCameraPos.X );
		bsInitGame.Write( (float)vInitCameraPos.Y );
		bsInitGame.Write( (float)vInitCameraPos.Z );
		bsInitGame.Write( (float)vInitCameraLook.X );
		bsInitGame.Write( (float)vInitCameraLook.Y );
		bsInitGame.Write( (float)vInitCameraLook.Z );
		bsInitGame.Write( (float)CConfigLoader::GetWorldBounds( 0 ) );
		bsInitGame.Write( (float)CConfigLoader::GetWorldBounds( 1 ) );
		bsInitGame.Write( (float)CConfigLoader::GetWorldBounds( 2 ) );
		bsInitGame.Write( (float)CConfigLoader::GetWorldBounds( 3 ) );
		bsInitGame.Write( (int)CSpawnPoolManager::Count() ); // Spawns Available

		if ( CConfigLoader::GetFriendlyFire() ) bsInitGame.Write( (BYTE)1 );
		else bsInitGame.Write((BYTE)0);

		bsInitGame.Write( (unsigned char)CConfigLoader::GetShowOnRadar() );

		bsInitGame.Write( uc );

		bsInitGame.Write( (unsigned char)CAmmuPoolManager::Count() );
		CNet::GetInterface()->RPC( "InitGame", &bsInitGame, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
	}
}

void CNetSends::PlayerJoin( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		bsSend.Write( uc );
		bsSend.Write( (int)strlen( pPlayer->GetNick() ) );
		bsSend.Write( pPlayer->GetNick(), strlen( pPlayer->GetNick() ) );
		CNet::GetInterface()->RPC( "ServerJoin", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
	}
}

void CNetSends::PlayerPart( unsigned char uc, unsigned char ucReason )
{
	RakNet::BitStream bsSend;
	bsSend.Write( uc );
	bsSend.Write( ucReason );
	NetInterface->RPC( "ServerQuit", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
}

void CNetSends::Chat( unsigned char ucPlayer, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
	if ( pPlayer )
	{
		if ( !pPlayer->GetMuted() )
		{
			BYTE byteTextLen = strlen( szText );

			RakNet::BitStream bsSend;
			bsSend.Write( ucPlayer );
			bsSend.Write( byteTextLen );
			bsSend.Write( szText, byteTextLen );

			//NetInterface->RPC( "Chat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
			unsigned char uc = 0, uc1 = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pSendTo = CPlayerPoolManager::Find( uc );
				if ( pSendTo )
				{
					if ( pSendTo != pPlayer )
					{
						// If the player hasn't ignored the player talking, send the message
						if ( !pSendTo->GetIgnored( ucPlayer ) ) NetInterface->RPC( "Chat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pSendTo->GetPlayerID(), FALSE, FALSE );
					}
					uc1++;
				}
				uc++;
			}
		}
	}
}

void CNetSends::ChatToPlayer( unsigned char ucPlayerTo, unsigned char ucPlayer, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
	CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( ucPlayerTo );

	if ( ( pPlayer ) && ( pPlayerTo ) )
	{
		BYTE byteTextLen = strlen( szText );

		RakNet::BitStream bsSend;
		bsSend.Write( ucPlayer );
		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		unsigned char uc = 0, uc1 = 0;

		if ( ucPlayerTo != ucPlayer )
		{
			// If the player hasn't ignored the player talking, send the message
			if ( !pPlayer->GetIgnored( ucPlayerTo ) ) NetInterface->RPC( "Chat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayerTo->GetPlayerID(), FALSE, FALSE );
		}
	}
}

void CNetSends::Action( unsigned char ucPlayer, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
	if ( pPlayer )
	{
		if ( !pPlayer->GetMuted() )
		{
			BYTE byteTextLen = strlen( szText );

			RakNet::BitStream bsSend;
			bsSend.Write( ucPlayer );
			bsSend.Write( byteTextLen );
			bsSend.Write( szText, byteTextLen );

			//NetInterface->RPC( "ME", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
			unsigned char uc = 0, uc1 = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pSendTo = CPlayerPoolManager::Find( uc );
				if ( pSendTo )
				{
					if ( pSendTo != pPlayer )
					{
						// If the player hasn't ignored the player talking, send the message
						if ( !pSendTo->GetIgnored( ucPlayer ) ) NetInterface->RPC( "ME", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pSendTo->GetPlayerID(), FALSE, FALSE );
					}
					uc1++;
				}
				uc++;
			}
		}
	}
}

void CNetSends::PM( unsigned char ucPlayer, unsigned char ucPlayerTo, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
	if ( pPlayer )
	{
		CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( ucPlayerTo );
		if ( pPlayerTo )
		{
			if ( !pPlayerTo->GetIgnored( ucPlayer ) )
			{
				char szBlah[ MAX_TEXT_LEN ] = { 0 };
				strcpy( szBlah, "PM" );
				strcat( szBlah, szText );

				BYTE byteTextLen = strlen( szBlah );

				RakNet::BitStream bsSend;
				bsSend.Write( ucPlayer );
				bsSend.Write( ucPlayerTo );
				bsSend.Write( byteTextLen );
				bsSend.Write( szBlah, byteTextLen );

				NetInterface->RPC( "MsgChat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayerTo->GetPlayerID(), FALSE, FALSE );
			}
		}
	}
}

void CNetSends::TeamChat( unsigned char ucPlayer, unsigned char ucTeam, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
	if ( pPlayer )
	{
		if ( !pPlayer->GetMuted() )
		{
			BYTE byteTextLen = strlen( szText );

			RakNet::BitStream bsSend;
			bsSend.Write( ucPlayer );
			bsSend.Write( ucTeam );
			bsSend.Write( byteTextLen );
			bsSend.Write( szText, byteTextLen );

			//NetInterface->RPC( "TeamChat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
			unsigned char uc = 0, uc1 = 0;
			while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
			{
				CPlayerPool* pSendTo = CPlayerPoolManager::Find( uc );
				if ( pSendTo )
				{
					if ( pSendTo != pPlayer )
					{
						// If the player hasn't ignored the player talking, send the message
						if ( !pSendTo->GetIgnored( ucPlayer ) ) NetInterface->RPC( "TeamChat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pSendTo->GetPlayerID(), FALSE, FALSE );
					}
					uc1++;
				}
				uc++;
			}
		}
	}
}

void CNetSends::SpawnForWorld( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		if ( !pPlayer->GetSyncBlocked() )
		{
			RakNet::BitStream bsSend;
			CSpawnPool* pSpawn = CSpawnPoolManager::Find( pPlayer->GetClassID() );
			if ( pSpawn )
			{
				bsSend.Write( uc );
				bsSend.Write( pSpawn->GetTeam() );
				bsSend.Write( pSpawn->GetSkin() );
				bsSend.Write( pSpawn->GetX() );
				bsSend.Write( pSpawn->GetY() );
				bsSend.Write( pSpawn->GetZ() );
				bsSend.Write( pSpawn->GetAngle() );
				bsSend.Write( pSpawn->GetWeapon( 0 ) );
				bsSend.Write( pSpawn->GetWeaponAmmo( 0 ) );
				bsSend.Write( pSpawn->GetWeapon( 1 ) );
				bsSend.Write( pSpawn->GetWeaponAmmo( 1 ) );
				bsSend.Write( pSpawn->GetWeapon( 2 ) );
				bsSend.Write( pSpawn->GetWeaponAmmo( 2 ) );

				NetInterface->RPC( "Spawn", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
			}
		}
	}
}

void CNetSends::SpawnForPlayer( unsigned char uc, unsigned char ucPlayer )
{
	CPlayerPool* pPlayerToSend = CPlayerPoolManager::Find( uc );
	if ( pPlayerToSend )
	{
		if ( !pPlayerToSend->GetSyncBlocked() )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( ucPlayer );
			if ( pPlayer )
			{
				RakNet::BitStream bsSend;
				CSpawnPool* pSpawn = CSpawnPoolManager::Find( pPlayer->GetClassID() );
				if ( pSpawn )
				{
					bsSend.Write( ucPlayer );
					bsSend.Write( pSpawn->GetTeam() );
					bsSend.Write( pSpawn->GetSkin() );
					bsSend.Write( pSpawn->GetX() );
					bsSend.Write( pSpawn->GetY() );
					bsSend.Write( pSpawn->GetZ() );
					bsSend.Write( pSpawn->GetAngle() );
					bsSend.Write( pSpawn->GetWeapon( 0 ) );
					bsSend.Write( pSpawn->GetWeaponAmmo( 0 ) );
					bsSend.Write( pSpawn->GetWeapon( 1 ) );
					bsSend.Write( pSpawn->GetWeaponAmmo( 1 ) );
					bsSend.Write( pSpawn->GetWeapon( 2 ) );
					bsSend.Write( pSpawn->GetWeaponAmmo( 2 ) );

					NetInterface->RPC( "Spawn", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPlayerToSend->GetPlayerID(), FALSE, FALSE );
				}
			}
		}
	}
}

void CNetSends::SpawnVehicleForPlayer( unsigned char uc, unsigned char ucVehicle )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( ucVehicle );
		if ( pVehicle )
		{
			RakNet::BitStream bsSend;

			bsSend.Write( ucVehicle );
			bsSend.Write( pVehicle->GetModel() );
			bsSend.Write( pVehicle->GetX() );
			bsSend.Write( pVehicle->GetY() );
			bsSend.Write( pVehicle->GetZ() );
			bsSend.Write( pVehicle->GetRotation() );
			bsSend.Write( (int)pVehicle->GetColour1() );
			bsSend.Write( (int)pVehicle->GetColour2() );
			bsSend.Write( pVehicle->GetHealth() );

			// Spawn stuff
			bsSend.Write( pVehicle->GetSpawnX() );
			bsSend.Write( pVehicle->GetSpawnY() );
			bsSend.Write( pVehicle->GetSpawnZ() );
			bsSend.Write( pVehicle->GetSpawnRotation() );

			NetInterface->RPC( "VehicleSpawn", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
		}
	}
}

void CNetSends::SpawnPickupForPlayer( unsigned char uc, unsigned int uiPickup, unsigned char ucOwner )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CPickupPool* pPickup = CPickupPoolManager::Find( uiPickup );
		if ( pPickup )
		{
			RakNet::BitStream bsSend;

			bsSend.Write( ucOwner );
			bsSend.Write( uiPickup );
			bsSend.Write( pPickup->GetModel() );
			bsSend.Write( pPickup->GetX() );
			bsSend.Write( pPickup->GetY() );
			bsSend.Write( pPickup->GetZ() );
			if ( pPickup->GetSpawned() ) bsSend.Write( (BYTE)1 );
			else bsSend.Write( (BYTE)0 );

			NetInterface->RPC( "PickupSpawn", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 1, pPlayer->GetPlayerID(), FALSE, FALSE );
		}
	}
}

void CNetSends::Death( unsigned char uc, unsigned char ucKiller, unsigned char ucReason )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		bsSend.Write( uc );
		bsSend.Write( ucReason );
		bsSend.Write( ucKiller );

		if ( ( ucKiller != INVALID_PLAYER_ID ) && ( ucKiller < MAX_PLAYERS ) )
		{
			CPlayerPool* pKiller = CPlayerPoolManager::Find( ucKiller );
			if ( pKiller )
			{
				if ( ( pKiller->GetTeam() == INVALID_TEAM_ID ) || ( ( pKiller->GetTeam() != INVALID_TEAM_ID ) && ( pKiller->GetTeam() != pPlayer->GetTeam() ) ) )
				{
					if (CConfigLoader::GetDeathmatchScoreB()) pKiller->IncScore();

					bsSend.Write( (unsigned char)VALID_KILL );

					CScripting::PushFunction( "onPlayerKill" );
					CScripting::PushPlayerPointer( pKiller );
					CScripting::PushPlayerPointer( pPlayer );
					CScripting::PushInteger( ucReason );
					CScripting::CallFunction();

					CPlayerSendPackets::Kill( ucKiller, pKiller->GetTeam(), uc, pPlayer->GetTeam(), ucReason );
				}
				else
				{
					if (CConfigLoader::GetDeathmatchScoreB()) pKiller->DecScore();

					bsSend.Write( (unsigned char)TEAM_KILL );

					CScripting::PushFunction( "onPlayerTeamKill" );
					CScripting::PushPlayerPointer( pKiller );
					CScripting::PushPlayerPointer( pPlayer );
					CScripting::PushInteger( ucReason );
					CScripting::CallFunction();

					CPlayerSendPackets::TeamKill( ucKiller, pKiller->GetTeam(), uc, pPlayer->GetTeam(), ucReason );
				}
			}
			else
			{
				bsSend.Write( (unsigned char)SELF_KILL );

				CScripting::PushFunction( "onPlayerDeath" );
				CScripting::PushPlayerPointer( pPlayer );
				CScripting::PushInteger( ucReason );
				CScripting::CallFunction();

				CPlayerSendPackets::Death( uc, ucReason );
			}
		}
		else
		{
			bsSend.Write( (unsigned char)SELF_KILL );

			CScripting::PushFunction( "onPlayerDeath" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushInteger( ucReason );
			CScripting::CallFunction();

			CPlayerSendPackets::Death( uc, ucReason );
		}

		// Broadcast it
		NetInterface->RPC( "Death", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), TRUE, FALSE );

		// Now let the player who died know aswell.
		NetInterface->RPC( "OwnDeath", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
	}
}

void CNetSends::SetHour( unsigned char ucHour )
{
	RakNet::BitStream bsSend;

	bsSend.Write( ucHour );

	NetInterface->RPC( "SetHour", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
}

void CNetSends::SetMinute( unsigned char ucMinute )
{
	RakNet::BitStream bsSend;

	bsSend.Write( ucMinute );

	NetInterface->RPC( "SetMinute", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
}

void CNetSends::SetWeather( unsigned char ucWeather )
{
	RakNet::BitStream bsSend;

	bsSend.Write( ucWeather );

	NetInterface->RPC( "SetWeather", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
}

void CNetSends::Set( char* sz )
{
	RakNet::BitStream bsSend;

	BYTE byteTextLen = strlen( sz );

	bsSend.Write( byteTextLen );
	bsSend.Write( sz, byteTextLen );

	NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
}

void CNetSends::EnterVehicle( unsigned char uc, unsigned char ucVehicle )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		if ( !pPlayer->GetSyncBlocked() )
		{
			RakNet::BitStream bsSend;
			bsSend.Write( uc );
			bsSend.Write( ucVehicle );
			bsSend.Write( pPlayer->GetVehiclePassenger() );

			NetInterface->RPC( "EnterVehicle", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
		}
	}
}

void CNetSends::ExitVehicle( unsigned char uc, unsigned char ucVehicle )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		if ( !pPlayer->GetSyncBlocked() )
		{
			RakNet::BitStream bsSend;
			bsSend.Write( uc );
			bsSend.Write( ucVehicle );

			NetInterface->RPC( "ExitVehicle", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
		}
	}
}

void CNetSends::UpdateScoreAndPing( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		unsigned char uc = 0, uc1 = 0;
		while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				bsSend.Write( uc );
				bsSend.Write( (int)pPlayer->GetScore() );
				bsSend.Write( (int)pPlayer->GetPing() );

				if ( pPlayer->IsAdmin() )
				{
					bsSend.Write( (unsigned long)pPlayer->GetPlayerID().binaryAddress );
				}
				else bsSend.Write((unsigned long)0UL);

				uc1++;
			}
			uc++;
		}

		NetInterface->RPC( "UpdateScPing", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
	}
}

void CNetSends::SetAdmin( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		bsSend.Write( uc );

		NetInterface->RPC( "IsAdmin", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::NotAdmin( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		bsSend.Write( uc );

		NetInterface->RPC( "NotAdmin", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::PickedUpPickup( unsigned char uc, unsigned int ui )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CPickupPool* pPickup = CPickupPoolManager::Find( ui );
		if ( pPickup )
		{
			RakNet::BitStream bsSend;

			bsSend.Write( ui );

			CPickupSendPackets::PickedUpPickup( uc, ui, pPickup->GetModel() );

			NetInterface->RPC( "PickedUp", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
		}
	}
}

void CNetSends::DeletePickup( unsigned char uc, unsigned int ui )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CPickupPool* pPickup = CPickupPoolManager::Find( ui );
		if ( pPickup )
		{
			RakNet::BitStream bsSend;

			bsSend.Write( ui );

			NetInterface->RPC( "PickedUp", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
		}
	}
}

void CNetSends::DeletePickupForPlayer( unsigned char uc, unsigned int ui )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CPickupPool* pPickup = CPickupPoolManager::Find( ui );
		if ( pPickup )
		{
			RakNet::BitStream bsSend;

			bsSend.Write( ui );

			NetInterface->RPC( "PickedUp", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
		}
	}
}

void CNetSends::DeletePickupForAll( unsigned int ui )
{
	CPickupPool* pPickup = CPickupPoolManager::Find( ui );
	if ( pPickup )
	{
		RakNet::BitStream bsSend;

		bsSend.Write( ui );

		NetInterface->RPC( "PickedUp", &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );

		CPickupPoolManager::Remove( pPickup );
	}
}

void CNetSends::DoAnim( unsigned char uc, unsigned char ucAnim )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		if ( !pPlayer->GetSyncBlocked() )
		{
			RakNet::BitStream bsSend;

			bsSend.Write( uc );
			bsSend.Write( ucAnim );

			NetInterface->RPC( "DoAnim", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
		}
	}
}

void CNetSends::SpecOn( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		NetInterface->RPC( "SpecOn", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
	}
}

void CNetSends::AdminChat( char* szText )
{
	BYTE byteTextLen = strlen( szText );

	RakNet::BitStream bsSend;
	bsSend.Write( byteTextLen );
	bsSend.Write( szText, byteTextLen );

	unsigned char uc = 0, uc1 = 0;
	while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
	{
		CPlayerPool* pSendTo = CPlayerPoolManager::Find( uc );
		if ( pSendTo )
		{
			NetInterface->RPC( "AdminChat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pSendTo->GetPlayerID(), FALSE, FALSE );

			uc1++;
		}
		uc++;
	}
}

void CNetSends::AdminChatToPlayer( PlayerID playerID, char* szText )
{
	BYTE byteTextLen = strlen( szText );

	RakNet::BitStream bsSend;
	bsSend.Write( byteTextLen );
	bsSend.Write( szText, byteTextLen );

	NetInterface->RPC( "AdminChat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, playerID, FALSE, FALSE );
}

void CNetSends::AdminChatAllExcept( PlayerID playerID, char* szText )
{
	BYTE byteTextLen = strlen( szText );

	RakNet::BitStream bsSend;
	bsSend.Write( byteTextLen );
	bsSend.Write( szText, byteTextLen );

	NetInterface->RPC( "AdminChat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, playerID, TRUE, FALSE );
}

void CNetSends::AdminMsg( unsigned char uc, char* sz )
{
	CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc );
	if ( pPlayerTo )
	{
		char szBlah[ MAX_TEXT_LEN ] = { 0 };
		strcpy( szBlah, "PM" );
		strcat( szBlah, sz );

		unsigned char ucAdmin = 200;
		unsigned char byteTextLen = strlen( szBlah );

		RakNet::BitStream bsSend;
		bsSend.Write( ucAdmin );
		bsSend.Write( uc );
		bsSend.Write( byteTextLen );
		bsSend.Write( szBlah, byteTextLen );

		NetInterface->RPC( "MsgChat", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayerTo->GetPlayerID(), FALSE, FALSE );
	}
}

void CNetSends::RequestAmmu( unsigned char uc, unsigned char ucAmmu )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CAmmuPool* pAmmu = CAmmuPoolManager::Find( ucAmmu );
		if ( pAmmu )
		{
			unsigned char ucThing = 1;
			RakNet::BitStream bsSend;

			bsSend.Write( ucThing );
			bsSend.Write( ucAmmu );
			bsSend.Write( pAmmu->GetX() );
			bsSend.Write( pAmmu->GetY() );
			bsSend.Write( pAmmu->GetZ() );
			bsSend.Write( pAmmu->GetCameraAngleX() );
			bsSend.Write( pAmmu->GetCameraAngleY() );
			bsSend.Write( pAmmu->GetCameraAngleZ() );
			bsSend.Write( pAmmu->GetCameraX() );
			bsSend.Write( pAmmu->GetCameraY() );
			bsSend.Write( pAmmu->GetCameraZ() );

			NetInterface->RPC( "RequestAmmu", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
		}
	}
}

void CNetSends::RequestWep( unsigned char uc, unsigned char ucAmmu, unsigned int iWeapon )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CAmmuPool* pAmmu = CAmmuPoolManager::Find( ucAmmu );
		if ( pAmmu )
		{
			unsigned char ucEnabled = 0;
			if ( pAmmu->GetWeaponEnabled( iWeapon ) ) ucEnabled = 1;

			RakNet::BitStream bsSend;

			bsSend.Write( ucEnabled );
			bsSend.Write( iWeapon );
			bsSend.Write( ucAmmu );

			bsSend.Write( pAmmu->GetWeaponX() );
			bsSend.Write( pAmmu->GetWeaponY() );
			bsSend.Write( pAmmu->GetWeaponZ() );
			bsSend.Write( pAmmu->GetWeapon( iWeapon ) );
			bsSend.Write( pAmmu->GetWeaponCost( iWeapon ) );

			NetInterface->RPC( "RequestWep", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
		}
	}
}

void CNetSends::PlayerSync( unsigned char uc, ONFOOT_SYNC_DATA ofSync )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		bsSend.Write( (unsigned char)ID_PLAYER_SYNC );
		bsSend.Write( (unsigned char)uc );

		bsSend.Write( (PCHAR)&ofSync, sizeof( ONFOOT_SYNC_DATA ) );

		CRangedSends::MPUR( pPlayer, &bsSend );
	}
}

void CNetSends::AimSync( unsigned char uc, ONFOOT_SYNC_DATA ofSync, AIM_SYNC_DATA aimSync )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		bsSend.Write( (unsigned char)ID_AIM_SYNC );
		bsSend.Write( (unsigned char)uc );

		bsSend.Write( (PCHAR)&ofSync, sizeof( ONFOOT_SYNC_DATA ) );
		bsSend.Write( (PCHAR)&aimSync, sizeof( AIM_SYNC_DATA ) );

		CRangedSends::LPUR( pPlayer, &bsSend );
	}
}

void CNetSends::VehicleSync( unsigned char uc, unsigned char ucVehicle, INCAR_SYNC_DATA icSync )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		bsSend.Write( (unsigned char)ID_VEHICLE_SYNC );
		bsSend.Write( (unsigned char)uc );
		bsSend.Write( (unsigned char)ucVehicle );

		bsSend.Write( (PCHAR)&icSync, sizeof( INCAR_SYNC_DATA ) );

		CRangedSends::MPUR( pPlayer, &bsSend );
	}
}

void CNetSends::PassengerSync( unsigned char uc, unsigned char ucVehicle, unsigned int uiPassengerSeat )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;

		//bsSend.Write( (unsigned char)ID_PASSENGER_SYNC );
		bsSend.Write( (unsigned char)uc );
		bsSend.Write( (unsigned char)ucVehicle );
		bsSend.Write( uiPassengerSeat );

		NetInterface->RPC( "Passenger", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), TRUE, FALSE );
	}
}

// Rcon stuff
void CNetSends::SetPlayerPos( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "1 %i %f %f %f %i", uc1, pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetZ(), uc2 );

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetPlayerHealth( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "10 %i %i", uc, pPlayer->GetHealth() );

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetPlayerArmour( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "12 %i %i", uc, pPlayer->GetArmour() );

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetPlayerFrozen( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "6 %i %i", uc, (int)pPlayer->GetFrozen() );

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetPlayerWeapon( unsigned char uc, unsigned char ucWeapon, unsigned int uiAmmo )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "5 %i %i %i", uc, ucWeapon, uiAmmo );

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetPlayerCash( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf(szText, "4 %i %i", uc, pPlayer->GetCash());

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::RemovePlayerMarker( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf(szText, "7 %i", uc);

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::RemovePlayerLocalMarker( unsigned char uc, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc2 );

	if ( ( pPlayer ) && ( pPlayerTo ) )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf(szText, "7 %i", uc);

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayerTo->GetPlayerID(), FALSE, FALSE );
	}
}

void CNetSends::PutPlayerInVehicle( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc2 );
		if ( pVehicle )
		{
			RakNet::BitStream bsSend;
			char szText[ 256 ] = { NULL };
			BYTE byteTextLen = 0;

			sprintf(szText, "9 %i %i", uc1, uc2);

			byteTextLen = strlen( szText );

			bsSend.Write( byteTextLen );
			bsSend.Write( szText, byteTextLen );

			pPlayer->SetEnterVehicleTime( RakNet::GetTime() );
			pPlayer->SetVehicle( uc2 );
			pPlayer->SetVehiclePassenger( 18 );

			CPlayerSendPackets::EnterVehicle( pPlayer->GetID(), pVehicle->GetID(), pVehicle->GetModel(), pVehicle->GetHealth(), 0 );
				
			CScripting::PushFunction( "onPlayerEnterVehicle" );
			CScripting::PushPlayerPointer( pPlayer );
			CScripting::PushVehiclePointer( pVehicle );
			CScripting::PushBool( false );
			CScripting::CallFunction();

			pVehicle->SetLastUsedTime( RakNet::GetTime() );
			pVehicle->SetBeenUsed( true );
			pVehicle->SetDriver( uc1 );

			NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
		}
	}
}

void CNetSends::SetPlayerMarker( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf(szText, "11 %i %i", uc1, uc2);

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetPlayerLocalMarker( unsigned char uc1, unsigned char uc2, unsigned char uc3 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc3 );

	if ( ( pPlayer ) && ( pPlayerTo ) )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf(szText, "11 %i %i", uc1, uc2);

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayerTo->GetPlayerID(), FALSE, FALSE );
	}
}

void CNetSends::Announce( unsigned char uc, char* sz )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "13 %i %s", uc, sz );

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetMarker( unsigned char uc, float fX, float fY, float fZ )
{
	RakNet::BitStream bsSend;
	char szText[ 256 ] = { NULL };
	BYTE byteTextLen = 0;

	sprintf( szText, "16 %i %f %f %f", uc, fX, fY, fZ);

	byteTextLen = strlen( szText );

	bsSend.Write( byteTextLen );
	bsSend.Write( szText, byteTextLen );

	NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
}

void CNetSends::SetMarkerForPlayer( unsigned char uc, unsigned char uc2, float fX, float fY, float fZ )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "16 %i %f %f %f", uc2, fX, fY, fZ);

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, pPlayer->GetPlayerID(), FALSE, FALSE );
	}
}

void CNetSends::SetVehicleDoorStatus( unsigned char uc )
{
	CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
	if ( pVehicle )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		if (pVehicle->GetLocked()) sprintf( szText, "3 %i 4", uc);
		else sprintf( szText, "3 %i 1", uc );

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetVehiclePosition( unsigned char uc )
{
	CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
	if ( pVehicle )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "8 %i %f %f %f", uc, pVehicle->GetX(), pVehicle->GetY(), pVehicle->GetZ());

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::KillVehicleEngine( unsigned char uc )
{
	CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
	if ( pVehicle )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "18 %i", uc);

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::SetVehicleHealth( unsigned char uc )
{
	CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
	if ( pVehicle )
	{
		RakNet::BitStream bsSend;
		char szText[ 256 ] = { NULL };
		BYTE byteTextLen = 0;

		sprintf( szText, "2 %i %f", uc, pVehicle->GetHealth() );

		byteTextLen = strlen( szText );

		bsSend.Write( byteTextLen );
		bsSend.Write( szText, byteTextLen );

		NetInterface->RPC( "Set", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );
	}
}

void CNetSends::KickPlayer( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		bsSend.Write( uc );

		NetInterface->RPC( "Kicked", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );

		CConsole::PrintC( "CONNECTION", COLOUR_RED, "Kicked [%s] | Index [%i]", pPlayer->GetNick(), uc );

		PlayerPart( uc, PARTREASON_KICKED );
		CPlayerSendPackets::Part( uc, PARTREASON_KICKED );

		//NetInterface->Kick( pPlayer->GetPlayerID() );

		CScripting::PushFunction( "onPlayerPart" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( PARTREASON_KICKED );
		CScripting::CallFunction();

		CPlayerPoolManager::Remove( pPlayer );
	}
}

void CNetSends::BanPlayer( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bsSend;
		bsSend.Write( uc );

		NetInterface->RPC( "Banned", &bsSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, TRUE, FALSE );

		//NetInterface->Kick( pPlayer->GetPlayerID() ); 

		CConsole::PrintC( "CONNECTION", COLOUR_RED, "Banned [%s] | IP [%s] | Index [%i]", pPlayer->GetNick(), pPlayer->GetIP(), uc );

		PlayerPart( uc, PARTREASON_KICKED );
		CPlayerSendPackets::Part( uc, PARTREASON_BANNED );

		CScripting::PushFunction( "onPlayerPart" );
		CScripting::PushPlayerPointer( pPlayer );
		CScripting::PushInteger( PARTREASON_BANNED );
		CScripting::CallFunction();

		CBanList::AddToBanList( pPlayer->GetIP() );

		CPlayerPoolManager::Remove( pPlayer );
	}
}