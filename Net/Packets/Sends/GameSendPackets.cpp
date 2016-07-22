/*
	GameSendPackets.cpp
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

#include "GameSendPackets.h"

void CGameSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_GAME_PACKET );
	bs.WriteCompressed( uc );
}

void CGameSendPackets::SetWeather( unsigned char uc1, unsigned char uc2 )
{
	CRconPool* pRcon = CRconPoolManager::Find( uc1 );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_GAME_SETWEATHER, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
	else if ( uc1 == 255 )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_GAME_SETWEATHER, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CGameSendPackets::SetTime( unsigned char uc1, unsigned char uc2, unsigned char uc3 )
{
	if (uc2 == 255) uc2 = CConfigLoader::GetHour();
	else if (uc3 == 255) uc3 = CConfigLoader::GetMinute();

	CRconPool* pRcon = CRconPoolManager::Find( uc1 );
	if ( pRcon )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_GAME_SETTIME, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );
		bs.WriteCompressed( uc3 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
	else if ( uc1 == 255 )
	{
		RakNet::BitStream bs;

		PreparePacket( ID_GAME_SETTIME, bs );

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );
		bs.WriteCompressed( uc3 );

		RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
	}
}

void CGameSendPackets::UpdateTimeRate( unsigned char uc )
{
	RakNet::BitStream bs;

	PreparePacket(ID_GAME_UPDATETIMERATE, bs );

	bs.WriteCompressed( uc );

	RconInterface->Send( &bs, LOW_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true );
}