/*
	RangedSends.cpp
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

#include "RangedSends.h"
#include "Utils.h"

void CRangedSends::LPUR( CPlayerPool* pPlayer, RakNet::BitStream* bs )
{
	BYTE b = 0, b1 = 0;
	if ( pPlayer )
	{
		if ( !pPlayer->GetSyncBlocked() )
		{
			float fLocalX = pPlayer->GetX();
			float fLocalY = pPlayer->GetY();
			CPlayerPool* pSendPlayer = NULL;
			unsigned long iTime = RakNet::GetTime();

			while ( ( b < MAX_PLAYERS ) && ( b1 < CPlayerPoolManager::Count() ) )
			{
				pSendPlayer = CPlayerPoolManager::Find( b );
				if ( pSendPlayer )
				{
					if ( pSendPlayer != pPlayer )
					{
						if ( CUtils::DistanceFromPoint( fLocalX, fLocalY, pSendPlayer->GetX(), pSendPlayer->GetY() ) <= 250.0f )
						{
							if ( !pPlayer->GetSyncBlockedTo( b ) ) NetInterface->Send( bs, LOW_PRIORITY, UNRELIABLE, 0, pSendPlayer->GetPlayerID(), false );

							pPlayer->SetLastSyncTime( b, iTime );
						}
						else 
						{
							if ( ( iTime - pPlayer->GetLastSyncTime( b ) ) >= 250 )
							{
								if ( !pPlayer->GetSyncBlockedTo( b ) ) NetInterface->Send( bs, LOW_PRIORITY, UNRELIABLE, 0, pSendPlayer->GetPlayerID(), false );

								pPlayer->SetLastSyncTime( b, iTime );
							}
						}
					}
					b1++;
				}
				b++;
			}
		}
	}
}

void CRangedSends::MPUR( CPlayerPool* pPlayer, RakNet::BitStream* bs )
{
	BYTE b = 0, b1 = 0;
	if ( pPlayer )
	{
		if ( !pPlayer->GetSyncBlocked() )
		{
			float fLocalX = pPlayer->GetX();
			float fLocalY = pPlayer->GetY();
			CPlayerPool* pSendPlayer = NULL;
			unsigned long iTime = RakNet::GetTime();

			while ( ( b < MAX_PLAYERS ) && ( b1 < CPlayerPoolManager::Count() ) )
			{
				pSendPlayer = CPlayerPoolManager::Find( b );
				if ( pSendPlayer )
				{
					if ( pSendPlayer != pPlayer )
					{
						if ( CUtils::DistanceFromPoint( fLocalX, fLocalY, pSendPlayer->GetX(), pSendPlayer->GetY() ) <= 250.0f )
						{
							if ( !pPlayer->GetSyncBlockedTo( b ) ) NetInterface->Send( bs, MEDIUM_PRIORITY, UNRELIABLE, 0, pSendPlayer->GetPlayerID(), false );

							pPlayer->SetLastSyncTime( b, iTime );
						}
						else 
						{
							if ( ( iTime - pPlayer->GetLastSyncTime( b ) ) >= 250 )
							{
								if ( !pPlayer->GetSyncBlockedTo( b ) ) NetInterface->Send( bs, MEDIUM_PRIORITY, UNRELIABLE, 0, pSendPlayer->GetPlayerID(), false );

								pPlayer->SetLastSyncTime( b, iTime );
							}
						}
					}
					b1++;
				}
				b++;
			}
		}
	}
}

void CRangedSends::HPUR( CPlayerPool* pPlayer, RakNet::BitStream* bs )
{
	BYTE b = 0, b1 = 0;
	if ( pPlayer )
	{
		if ( !pPlayer->GetSyncBlocked() )
		{
			float fLocalX = pPlayer->GetX();
			float fLocalY = pPlayer->GetY();
			CPlayerPool* pSendPlayer = NULL;
			unsigned long iTime = RakNet::GetTime();

			while ( ( b < MAX_PLAYERS ) && ( b1 < CPlayerPoolManager::Count() ) )
			{
				pSendPlayer = CPlayerPoolManager::Find( b );
				if ( pSendPlayer )
				{
					if ( pSendPlayer != pPlayer )
					{
						if ( CUtils::DistanceFromPoint( fLocalX, fLocalY, pSendPlayer->GetX(), pSendPlayer->GetY() ) <= 250.0f )
						{
							if ( !pPlayer->GetSyncBlockedTo( b ) ) NetInterface->Send( bs, HIGH_PRIORITY, UNRELIABLE, 0, pSendPlayer->GetPlayerID(), false );

							pPlayer->SetLastSyncTime( b, iTime );
						}
						else 
						{
							if ( ( iTime - pPlayer->GetLastSyncTime( b ) ) >= 250 )
							{
								if ( !pPlayer->GetSyncBlockedTo( b ) ) NetInterface->Send( bs, HIGH_PRIORITY, UNRELIABLE, 0, pSendPlayer->GetPlayerID(), false );

								pPlayer->SetLastSyncTime( b, iTime );
							}
						}
					}
					b1++;
				}
				b++;
			}
		}
	}
}