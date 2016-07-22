/*
	GameRecvPackets.cpp
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

#include "GameRecvPackets.h"
#include "ConfigLoader.h"

RakNet::BitStream CGameRecvPackets::bs;
Packet* CGameRecvPackets::m_pPacket = NULL;

void CGameRecvPackets::Parse( Packet* p )
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
		case ID_GAME_SETWEATHER:
			RecvWeather();
			break;
		case ID_GAME_SETTIME:
			RecvTime();
			break;
		case ID_GAME_SETMARKER:
			SetMarker();
			break;
		case ID_GAME_AMMUWEAPON:
			EnableWeaponAtAmmu();
			break;
		case ID_GAME_SETTIMERATE:
			SetTimeRate();
			break;
		case ID_GAME_SETWEATHERRATE:
			SetWeatherRate();
			break;
		case ID_GAME_SETMARKER4PLAYER:
			CreateMarkerForPlayer();
			break;
		case ID_GAME_SETMAMMUWEAPON:
			SetAmmuWeapon();
			break;
		}
	}
}

void CGameRecvPackets::RecvWeather( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed(uc);

		CScripting::PushFunction( "onWeatherChange" );
		CScripting::PushInteger( CConfigLoader::GetWeather() );
		CScripting::PushInteger( uc );
		CScripting::CallFunction();

		CConfigLoader::SetWeather( uc );

		CGameSendPackets::SetWeather( (unsigned char)m_pPacket->playerIndex, uc );
		CNetSends::SetWeather( uc );
	}
}

void CGameRecvPackets::RecvTime( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;

		bs.ReadCompressed(uc1);
		bs.ReadCompressed(uc2);

		if (uc1 != 255)
		{
			CConfigLoader::SetHour( uc1 );
			CNetSends::SetHour( uc1 );
		}
		else if (uc2 != 255)
		{
			CConfigLoader::SetMinute( uc2 );
			CNetSends::SetMinute( uc2 );
		}

		CScripting::PushFunction( "onTimeChange" );
		CScripting::PushInteger( uc1 );
		CScripting::PushInteger( uc2 );
		CScripting::CallFunction();

		CGameSendPackets::SetTime( (unsigned char)m_pPacket->playerIndex, uc1, uc2 );
	}
}

void CGameRecvPackets::SetMarker( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		float fX = 0.0, fY = 0.0, fZ = 0.0;

		bs.ReadCompressed(uc);
		bs.ReadVector(fX,fY,fZ);

		CNetSends::SetMarker( uc, fX, fY, fZ );
	}
}

void CGameRecvPackets::EnableWeaponAtAmmu( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0, uc2 = 0;
		bool bEnabled = false;

		bs.ReadCompressed( uc );
		bs.ReadCompressed( uc2 );
		bs.Read( bEnabled );

		CAmmuPool* pAmmunation = CAmmuPoolManager::Find( uc );
		if (pAmmunation)
		{
			pAmmunation->SetWeaponEnabled( uc2, bEnabled );
		}
	}
}

void CGameRecvPackets::SetTimeRate( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed( uc );

		CConfigLoader::SetTimeRate( uc );
	}
}

void CGameRecvPackets::SetWeatherRate( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned int ui1 = 0;
		unsigned int ui2 = 0;

		bs.Read( ui1 );

		ui2 = ui1 *= 1000; // Converts from seconds to milliseconds

		CConfigLoader::SetWeatherRate( ui2 );
	}
}

void CGameRecvPackets::CreateMarkerForPlayer( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;
		float fX = 0.0, fY = 0.0, fZ = 0.0;

		bs.ReadCompressed( uc1 );
		bs.ReadCompressed( uc2 );
		bs.ReadVector(fX,fY,fZ);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			CNetSends::SetMarkerForPlayer( uc1, uc2, fX, fY, fZ );
		}
	}
}

void CGameRecvPackets::SetAmmuWeapon( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;
		unsigned int ui1 = 0, ui2 = 0;

		bs.ReadCompressed( uc1 );
		bs.ReadCompressed( uc2 );
		bs.ReadCompressed( ui1 );
		bs.ReadCompressed( ui2 );

		CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc1 );
		if ( pAmmu )
		{
			pAmmu->SetWeapon( uc2, ui1 );
			pAmmu->SetWeaponCost( uc2, ui2 );
		}
	}
}