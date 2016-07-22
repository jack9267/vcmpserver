/*
	MiscSendPackets.cpp
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

#include "MiscSendPackets.h"
#include "Pools.h"

void CMiscSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_MISC_PACKET );
	bs.WriteCompressed( uc );
}

void CMiscSendPackets::VersionReject( unsigned char uc, unsigned int ui )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_JOINDECLINED, bs );

		bs.WriteCompressed( ui );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CMiscSendPackets::AdminChat( unsigned char uc, char* szText )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_RCONCHAT, bs );

		bs.WriteCompressed( uc );

		stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CMiscSendPackets::AdminPM( unsigned char uc1, unsigned char uc2, char* szText )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc1 );
	if ( pRcon )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc2 );
		if ( pPlayer )
		{
			RakNet::BitStream bs;

			PreparePacket( ID_MISC_RCONMSG, bs );

			bs.WriteCompressed( uc1 );

			bs.WriteCompressed( uc2 );

			stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

			RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}

void CMiscSendPackets::Announce( unsigned char uc1, unsigned char uc2, char* szText )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc1 );
	if ( pRcon )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc2 );
		if ( pPlayer )
		{
			RakNet::BitStream bs;

			PreparePacket( ID_MISC_RCONANN, bs );

			bs.WriteCompressed( uc1 );

			bs.WriteCompressed( uc2 );

			stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

			RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}

void CMiscSendPackets::AnnounceAll( unsigned char uc, char* szText )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_RCONANNALL, bs );

		bs.WriteCompressed( uc );

		stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CMiscSendPackets::ServerData( unsigned char uc )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_SERVERDATA, bs );

		bs.WriteCompressed( CConfigLoader::GetWeatherRate() );
		bs.WriteCompressed( CConfigLoader::GetTimeRate() );
		bs.WriteCompressed( CConfigLoader::GetWeather() );
		bs.WriteCompressed( CConfigLoader::GetHour() );
		bs.WriteCompressed( CConfigLoader::GetMinute() );
		bs.WriteCompressed( CConfigLoader::GetMaxPlayers() );
		bs.Write( CConfigLoader::GetDeathmatchScoreB() );
		bs.Write( CConfigLoader::GetDrivebyEnabled() );

		stringCompressor->EncodeString( CConfigLoader::GetPassword(), MAX_PASS_LEN, &bs );
		stringCompressor->EncodeString( CConfigLoader::GetServerName(), MAX_SERVNAME_LEN, &bs );
		stringCompressor->EncodeString( CConfigLoader::GetGamemodeName(), MAX_GAMEMODE_LEN, &bs );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CMiscSendPackets::RconList( unsigned char uc )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_RCONLIST, bs );

		bs.WriteCompressed( (unsigned char)CRconPoolManager::Count() );

		unsigned char uc = 0, uc1 = 0;
		while( (uc < MAX_PLAYERS) && (uc1 < CRconPoolManager::Count()) )
		{
			CRconPool* pRcon = CRconPoolManager::Find( uc );
			if ( pRcon )
			{
				bs.WriteCompressed( uc );
				stringCompressor->EncodeString( pRcon->GetNick(), MAX_NAME_LEN, &bs );
				stringCompressor->EncodeString( pRcon->GetIP(), MAX_IP_LEN, &bs );

				uc1++;
			}
			uc++;
		}

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CMiscSendPackets::PlayerList( unsigned char uc )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_PLAYERLIST, bs );

		bs.WriteCompressed( (unsigned char)CPlayerPoolManager::Count() );

		unsigned char uc = 0, uc1 = 0;
		while( (uc < MAX_PLAYERS) && (uc1 < CPlayerPoolManager::Count()) )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				bs.WriteCompressed( uc );
				stringCompressor->EncodeString( pPlayer->GetNick(), MAX_NAME_LEN, &bs );
				stringCompressor->EncodeString( pPlayer->GetIP(), MAX_IP_LEN, &bs );

				bs.WriteVector( pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetZ() );
				bs.Write( pPlayer->GetRotation() );

				bs.WriteCompressed( pPlayer->GetHealth() );
				bs.WriteCompressed( pPlayer->GetArmour() );
				bs.WriteCompressed( pPlayer->GetWeapon() );
				bs.WriteCompressed( pPlayer->GetCash() );
				bs.Write( pPlayer->GetFrozen() );
				bs.Write( pPlayer->GetMuted() );
				bs.WriteCompressed( pPlayer->GetScore() );
				bs.Write( pPlayer->GetSpawned() );
				bs.WriteCompressed( pPlayer->GetClassID() );
				CSpawnPool* pSpawn = CSpawnPoolManager::Find( pPlayer->GetClassID() );
				if ( pSpawn )
				{
					bs.WriteCompressed( pSpawn->GetTeam() );
					bs.WriteCompressed( pSpawn->GetSkin() );
				}

				uc1++;
			}
			uc++;
		}

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CMiscSendPackets::VehicleList( unsigned char uc )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_VEHICLELIST, bs );

		bs.WriteCompressed( (unsigned char)CVehiclePoolManager::Count() );

		unsigned char uc = 0, uc1 = 0;
		while ( ( uc < MAX_VEHICLES ) && ( uc1 < CVehiclePoolManager::Count() ) )
		{
			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc );
			if ( pVehicle )
			{
				bs.WriteCompressed( uc );
				bs.WriteCompressed( pVehicle->GetModel() );
				bs.WriteVector( pVehicle->GetX(), pVehicle->GetY(), pVehicle->GetZ() );
				bs.Write( pVehicle->GetHealth() );
				bs.Write( pVehicle->GetLocked() );

				uc1++;
			}
			uc++;
		}

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CMiscSendPackets::PickupList( unsigned char uc )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_PICKUPLIST, bs );

		bs.Write( (unsigned int)CPickupPoolManager::Count() );
		
		unsigned int ui = 0, ui1 = 0;
		
		while ( ( ui < MAX_PICKUPS ) && ( ui1 < CPickupPoolManager::Count() ) )
		{
			CPickupPool* pPickup = CPickupPoolManager::Find( ui );
			if ( pPickup )
			{
				bs.Write( ui );
				bs.Write( pPickup->GetModel() );
				bs.WriteVector( pPickup->GetX(), pPickup->GetY(), pPickup->GetZ() );
				ui1++;
			}
			ui++;
		}
		
		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CMiscSendPackets::AmmuList( unsigned char uc )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_MISC_AMMULIST, bs );

		bs.WriteCompressed( (unsigned char)CAmmuPoolManager::Count() );
		
		unsigned char uc = 0, uc2 = 0;
		
		while ( ( uc < MAX_SPAWNS ) && ( uc2 < CAmmuPoolManager::Count() ) )
		{
			CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc );
			if ( pAmmu )
			{
				bs.WriteCompressed( uc );
				bs.WriteCompressed( pAmmu->GetWeapon( 0 ) );
				bs.WriteCompressed( pAmmu->GetWeapon( 1 ) );
				bs.WriteCompressed( pAmmu->GetWeapon( 2 ) );
				bs.WriteCompressed( pAmmu->GetWeapon( 3 ) );
				bs.WriteCompressed( pAmmu->GetWeaponCost( 0 ) );
				bs.WriteCompressed( pAmmu->GetWeaponCost( 1 ) );
				bs.WriteCompressed( pAmmu->GetWeaponCost( 2 ) );
				bs.WriteCompressed( pAmmu->GetWeaponCost( 3 ) );
				bs.Write( pAmmu->GetWeaponEnabled( 0 ) );
				bs.Write( pAmmu->GetWeaponEnabled( 1 ) );
				bs.Write( pAmmu->GetWeaponEnabled( 2 ) );
				bs.Write( pAmmu->GetWeaponEnabled( 3 ) );
				uc2++;
			}
			uc++;
		}
		
		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}