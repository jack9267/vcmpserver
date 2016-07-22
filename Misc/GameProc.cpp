/*
	GameProc.cpp
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

#include "GameProc.h"
#include "ConfigLoader.h"
#include "NetSends.h"
#include "SendPackets.h"
#include "CarCols.h"

unsigned int CGameProc::m_uiLastTimeSync = RakNet::GetTime();
unsigned int CGameProc::m_uiLastWeatherSync = RakNet::GetTime();

void CGameProc::PickupRespawn( void )
{
	unsigned int uiTime = RakNet::GetTime();

	unsigned short us = 0, us1 = 0;
	CPickupPool* pPickup = NULL;

	while ( ( us < MAX_PICKUPS ) && ( us1 < CPickupPoolManager::Count() ) )
	{
		pPickup = CPickupPoolManager::Find( us );
		if ( pPickup )
		{
			if ( !pPickup->GetSpawned() )
			{
				if ( ( uiTime - pPickup->GetPickedUpTime() ) >= pPickup->GetRespawnTime() )
				{
					if ( pPickup->GetSpawnForPlayer() != INVALID_PLAYER_ID ) CNetSends::SpawnPickupForPlayer( pPickup->GetSpawnForPlayer(), us );
					else
					{
						unsigned char uc = 0, uc1 = 0;
						while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
						{
							CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
							if ( pPlayer )
							{
								CNetSends::SpawnPickupForPlayer( uc, us );

								uc1++;
							}
							uc++;
						}
					}

					CScripting::PushFunction( "onPickupRespawn" );
					CScripting::PushPickupPointer( pPickup );
					CScripting::CallFunction();

					pPickup->SetSpawned( true );
				}
			}
			us1++;
		}
		us++;
	}
}

void CGameProc::VehicleRespawn( void )
{
	unsigned short us = 1, us1 = 0;
	CVehiclePool* pVehicle = NULL;
	unsigned int iTime = RakNet::GetTime();

	unsigned short usVehicleCount = CVehiclePoolManager::Count();

	while ( ( us < MAX_VEHICLES ) && ( us1 < usVehicleCount ) )
	{
		pVehicle = CVehiclePoolManager::Find( us );
		if ( pVehicle )
		{
			if ( pVehicle->GetDriver() == INVALID_PLAYER_ID )
			{
				if ( pVehicle->GetBeenUsed() )
				{
					if ( ( iTime - pVehicle->GetLastUsedTime() ) >= 210000 )
					{
						pVehicle->SetX( pVehicle->GetSpawnX() );
						pVehicle->SetY( pVehicle->GetSpawnY() );
						pVehicle->SetZ( pVehicle->GetSpawnZ() );
						pVehicle->SetRotation( pVehicle->GetSpawnRotation() );
						pVehicle->SetBeenUsed( false );
						pVehicle->SetLastUsedTime( 0 );

						if ( ( pVehicle->GetRandomColour1() ) || ( pVehicle->GetRandomColour2() ) )
						{
							CCarCols* pCarCols = CCarColManager::Find( pVehicle->GetModel() );
							if ( pCarCols )
							{
								if ( pVehicle->GetRandomColour1() ) pVehicle->SetColour1( pCarCols->RandomColour1() );
								if ( pVehicle->GetRandomColour2() ) pVehicle->SetColour2( pCarCols->RandomColour2() );
							}
						}


						// Tell the players to respawn the vehicle
						unsigned int uc = 0, uc1 = 0;
						while ( ( uc < MAX_PLAYERS ) && ( uc1 < CPlayerPoolManager::Count() ) )
						{
							CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
							if ( pPlayer )
							{
								CNetSends::SpawnVehicleForPlayer( uc, pVehicle->GetID() );
								uc1++;
							}
							uc++;
						}

						CScripting::PushFunction( "onVehicleRespawn" );
						CScripting::PushVehiclePointer( pVehicle );
						CScripting::CallFunction();
					}
				}
			}

			us1++;
		}
		us++;
	}
}

void CGameProc::TimeAndWeatherSync( void )
{
	unsigned int uiTime = RakNet::GetTime();

	if ( CConfigLoader::GetTimeRate() )
	{
		if ( ( uiTime - m_uiLastTimeSync ) >= (unsigned int)( ( 1000 / 8 ) * CConfigLoader::GetTimeRate() ) )
		{
			unsigned char ucHour = CConfigLoader::GetHour(), ucMinute = CConfigLoader::GetMinute();

			if ( ucMinute == 59 )
			{
				if ( ucHour == 23 ) ucHour = 0;
				else ucHour++;

				ucMinute = 0;

				CConfigLoader::SetHour( ucHour );

				CNetSends::SetHour( ucHour );

				// We want to refrain from doing a VC:MP and spamming the shit out of the rcon, so we send to the rcon every "hour" and let it work the rest out
				CGameSendPackets::SetTime( 255, ucHour, ucMinute );
			}
			else ucMinute++;

			CConfigLoader::SetMinute( ucMinute );

			CNetSends::SetMinute( ucMinute );

			CScripting::PushFunction( "onTimeChange" );
			CScripting::PushInteger( ucHour );
			CScripting::PushInteger( ucMinute );
			CScripting::CallFunction();

			m_uiLastTimeSync = uiTime;
		}
	}

	if ( CConfigLoader::GetWeatherRate() )
	{
		if ( ( uiTime - m_uiLastWeatherSync ) >= CConfigLoader::GetWeatherRate() )
		{
			unsigned char ucWeather = rand() % 5;

			CScripting::PushFunction( "onWeatherChange" );
			CScripting::PushInteger( CConfigLoader::GetWeather() );
			CScripting::PushInteger( ucWeather );
			CScripting::CallFunction();

			CConfigLoader::SetWeather( ucWeather );

			CNetSends::SetWeather( ucWeather );

			CGameSendPackets::SetWeather( 255, ucWeather );

			m_uiLastWeatherSync = uiTime;
		}
	}
}