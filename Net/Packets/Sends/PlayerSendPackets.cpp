/*
	PlayerSendPackets.cpp
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

#include "PlayerSendPackets.h"
#include "Pools.h"

void CPlayerSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_PLAYER_PACKET );
	bs.WriteCompressed( uc );
}

void CPlayerSendPackets::Chat( unsigned char uc, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_CHAT, bs );

		bs.WriteCompressed( uc );
		stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::Join( unsigned char uc, char* szNick, char* szIP )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_JOIN, bs );

		bs.WriteCompressed( uc );
		stringCompressor->EncodeString( szNick, MAX_NAME_LEN, &bs );
		stringCompressor->EncodeString( szIP, MAX_IP_LEN, &bs );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::Part( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_PART, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::Action( unsigned char uc, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_ACTION, bs );

		bs.WriteCompressed( uc );
		stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::PM( unsigned char uc1, unsigned char uc2, unsigned char uc3, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc2 );
	if ( pPlayer )
	{
		CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc3 );
		if ( pPlayerTo )
		{
			RakNet::BitStream bs;

			PreparePacket( ID_PLAYER_MSG, bs );

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );
			bs.WriteCompressed( uc3 );
			stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

			RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}

void CPlayerSendPackets::TeamChat( unsigned char uc1, unsigned char uc2, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_TEAMCHAT, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );
		stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::Spawn( unsigned char uc1, unsigned char uc2, unsigned char uc3 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_SPAWN, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );
		bs.WriteCompressed( uc3 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::EnterVehicle( unsigned char uc1, unsigned char uc2, unsigned char uc3, float fHealth, unsigned char uc4 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_VEHICLEENTER, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );
		bs.WriteCompressed( uc3 );
		bs.Write( fHealth );
		bs.WriteCompressed( uc4 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::ExitVehicle( unsigned char uc1, unsigned char uc2, unsigned char uc3, float fHealth )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_VEHICLEEXIT, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );
		bs.WriteCompressed( uc3 );
		bs.Write( fHealth );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::Kill( unsigned char uc1, unsigned char uc2, unsigned char uc3, unsigned char uc4, unsigned char uc5 )
{
	CPlayerPool* pKiller = CPlayerPoolManager::Find( uc1 );
	if ( pKiller )
	{
		CPlayerPool* pKilled = CPlayerPoolManager::Find( uc3 );
		if ( pKilled )
		{
			RakNet::BitStream bs;

			PreparePacket(ID_PLAYER_KILL, bs );

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );
			bs.WriteCompressed( uc3 );
			bs.WriteCompressed( uc4 );
			bs.WriteCompressed( uc5 );

			RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}

void CPlayerSendPackets::TeamKill( unsigned char uc1, unsigned char uc2, unsigned char uc3, unsigned char uc4, unsigned char uc5 )
{
	CPlayerPool* pKiller = CPlayerPoolManager::Find( uc1 );
	if ( pKiller )
	{
		CPlayerPool* pKilled = CPlayerPoolManager::Find( uc3 );
		if ( pKilled )
		{
			RakNet::BitStream bs;

			PreparePacket(ID_PLAYER_TEAMKILL, bs );

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );
			bs.WriteCompressed( uc3 );
			bs.WriteCompressed( uc4 );
			bs.WriteCompressed( uc5 );

			RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}

void CPlayerSendPackets::Death( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_DEATH, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::Command( unsigned char uc, char* szText )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_COMMAND, bs );

		bs.WriteCompressed( uc );
		stringCompressor->EncodeString( szText, MAX_TEXT_LEN, &bs );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::DoAnim( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_DOANIM, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::FootSync( unsigned char uc, ONFOOT_SYNC_DATA ofSync )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_SYNCDATA, bs );

		bs.WriteCompressed( uc );

		bs.WriteVector( ofSync.vecPos.X, ofSync.vecPos.Y, ofSync.vecPos.Z );
		bs.Write( ofSync.fRotation );

		bs.WriteCompressed( ofSync.byteHealth );
		bs.WriteCompressed( ofSync.byteArmour );
		bs.WriteCompressed( ofSync.byteCurrentWeapon );

		RconInterface->Send( &bs, LOW_PRIORITY, UNRELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::RequestAmmuWeapon( unsigned char uc1, unsigned char uc2, unsigned char uc3 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc2 );
		if ( pAmmu )
		{
			RakNet::BitStream bs;

			PreparePacket( ID_PLAYER_REQUESTWEAPON, bs );

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );
			bs.WriteCompressed( uc3 );

			RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}

void CPlayerSendPackets::RequestAmmunation( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc2 );
		if ( pAmmu )
		{
			RakNet::BitStream bs;

			PreparePacket( ID_PLAYER_REQUESTAMMU, bs );

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );

			RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}

void CPlayerSendPackets::RequestClass( unsigned char uc1, unsigned char uc2, unsigned char uc3, unsigned char uc4 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CSpawnPool* pSpawn = CSpawnPoolManager::Find( uc2 );
		if ( pSpawn ) 
		{
			RakNet::BitStream bs;

			PreparePacket( ID_PLAYER_REQUESTCLASS, bs );

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );
			bs.WriteCompressed( uc3 );
			bs.WriteCompressed( uc4 );

			RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
		}
	}
}

void CPlayerSendPackets::StartSpectating( unsigned char uc )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_STARTSPEC, bs );

		bs.WriteCompressed( uc );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CPlayerSendPackets::StopSpectating( unsigned char uc )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_PLAYER_STOPSPEC, bs );

		bs.WriteCompressed( uc );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}