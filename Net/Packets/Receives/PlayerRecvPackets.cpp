/*
	PlayerRecvPackets.cpp
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

#include "PlayerRecvPackets.h"

RakNet::BitStream CPlayerRecvPackets::bs;
Packet* CPlayerRecvPackets::m_pPacket = NULL;

void CPlayerRecvPackets::Parse( Packet* p )
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
		case ID_PLAYER_SETPOSITION:
			SetPosition();
			break;
		case ID_PLAYER_SETHEALTH:
			SetHealth();
			break;
		case ID_PLAYER_SETARMOUR:
			SetArmour();
			break;
		case ID_PLAYER_SETFROZEN:
			SetFrozen();
			break;
		case ID_PLAYER_SETWEAPON:
			SetWeapon();
			break;
		case ID_PLAYER_SETCASH:
			SetCash();
			break;
		case ID_PLAYER_REMOVEMARKER:
			RemoveMarker();
			break;
		case ID_PLAYER_PUTINVEHICLE:
			PutInVehicle();
			break;
		case ID_PLAYER_SETMARKER:
			SetPlayerMarker();
			break;
		case ID_PLAYER_DOANIM:
			DoAnim();
			break;
		case ID_PLAYER_SETSCORE:
			SetScore();
			break;
		case ID_PLAYER_INCSCORE:
			IncScore();
			break;
		case ID_PLAYER_DECSCORE:
			DecScore();
			break;
		case ID_PLAYER_CREATEPICKUP:
			CreatePickup();
			break;
		case ID_PLAYER_RESPAWNPICKUP:
			RespawnPickup();
			break;
		case ID_PLAYER_DELETEPICKUP:
			DeletePickup();
			break;
		case ID_PLAYER_SETMUTE:
			SetMute();
			break;
		case ID_PLAYER_SETIGNORE:
			SetIgnore();
			break;
		case ID_PLAYER_SETLMARKER:
			SetLocalMarker();
			break;
		case ID_PLAYER_REMLMARKER:
			RemoveLocalMarker();
			break;
		}
	}
}

void CPlayerRecvPackets::SetPosition( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;
		float fX = 0.0, fY = 0.0, fZ = 0.0;

		bs.ReadCompressed(uc1);
		bs.ReadVector(fX,fY,fZ);
		bs.ReadCompressed(uc2);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			pPlayer->SetX(fX);
			pPlayer->SetY(fY);
			pPlayer->SetZ(fZ);
			CNetSends::SetPlayerPos( uc1, uc2 );
		}
	}
}

void CPlayerRecvPackets::SetHealth( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;

		bs.ReadCompressed(uc1);
		bs.ReadCompressed(uc2);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			pPlayer->SetHealth( uc2 );
			CNetSends::SetPlayerHealth( uc1 );
		}
	}
}

void CPlayerRecvPackets::SetArmour( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;

		bs.ReadCompressed(uc1);
		bs.ReadCompressed(uc2);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			pPlayer->SetArmour( uc2 );
			CNetSends::SetPlayerArmour( uc1 );
		}
	}
}

void CPlayerRecvPackets::SetFrozen( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		bool bFrozen = false;

		bs.ReadCompressed(uc);
		bs.Read(bFrozen);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			pPlayer->SetFrozen( bFrozen );
			CNetSends::SetPlayerFrozen( uc );
		}
	}
}

void CPlayerRecvPackets::SetWeapon( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;
		unsigned int ui = 0;

		bs.ReadCompressed(uc1);
		bs.ReadCompressed(uc2);
		bs.ReadCompressed(ui);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			pPlayer->SetWeapon( uc2 );
			CNetSends::SetPlayerWeapon( uc1, uc2, ui );
		}
	}
}

void CPlayerRecvPackets::SetCash( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		int iCash = 0;

		bs.ReadCompressed(uc);
		bs.ReadCompressed(iCash);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			pPlayer->SetCash( iCash );
			CNetSends::SetPlayerCash( uc );
		}
	}
}

void CPlayerRecvPackets::RemoveMarker( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed(uc);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CNetSends::RemovePlayerMarker( uc );
		}
	}
}

void CPlayerRecvPackets::PutInVehicle( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;

		bs.ReadCompressed(uc1);
		bs.ReadCompressed(uc2);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc2 );
			if ( pVehicle )
			{
				CNetSends::PutPlayerInVehicle( uc1, uc2 );
			}
		}
	}
}

void CPlayerRecvPackets::SetPlayerMarker( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;

		bs.ReadCompressed(uc1);
		bs.ReadCompressed(uc2);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			CNetSends::SetPlayerMarker( uc1, uc2 );
		}
	}
}

void CPlayerRecvPackets::DoAnim( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc1 = 0, uc2 = 0;

		bs.ReadCompressed(uc1);
		bs.ReadCompressed(uc2);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
		if ( pPlayer )
		{
			CNetSends::DoAnim( uc1, uc2 );
		}
	}
}

void CPlayerRecvPackets::SetScore( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		int i = 0;

		bs.ReadCompressed(uc);
		bs.ReadCompressed(i);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			pPlayer->SetScore(i);
		}
	}
}

void CPlayerRecvPackets::IncScore( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed(uc);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			pPlayer->IncScore();
		}
	}
}

void CPlayerRecvPackets::DecScore( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;

		bs.ReadCompressed(uc);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			pPlayer->DecScore();
		}
	}
}

void CPlayerRecvPackets::CreatePickup( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		/*unsigned char uc = 0;
		unsigned int uiPickup = CPickupPoolManager::Count() + 1, uiModel = 0;
		float fX = 0.0, fY = 0.0, fZ = 0.0;

		bs.ReadCompressed(uc);
		bs.Read(uiModel);
		bs.ReadVector(fX,fY,fZ);

		CPickupPool* pPickup = CPickupPoolManager::Find( uiPickup );
		if ( !pPickup )
		{
			pPickup = CPickupPoolManager::New( uiPickup );
			if (pPickup)
			{
				pPickup->SetModel(uiModel);
				pPickup->SetX(fX);
				pPickup->SetY(fY);
				pPickup->SetZ(fZ);

				CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
				if ( pPlayer )
				{
					CNetSends::SpawnPickupForPlayer( uc, uiPickup, uc );
				}
			}
		}*/
	}
}

void CPlayerRecvPackets::RespawnPickup( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		unsigned int uiPickup = 0;

		bs.ReadCompressed(uc);
		bs.Read(uiPickup);

		CPickupPool* pPickup = CPickupPoolManager::Find( uiPickup );
		if ( pPickup )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				CNetSends::SpawnPickupForPlayer( uc, uiPickup, uc );
			}
		}
	}
}

void CPlayerRecvPackets::DeletePickup( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		/*unsigned char uc = 0;
		unsigned int uiPickup = 0;

		bs.ReadCompressed(uc);
		bs.Read(uiPickup);

		CPickupPool* pPickup = CPickupPoolManager::Find( uiPickup );
		if ( pPickup )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				CNetSends::DeletePickup( uc, uiPickup );
			}
		}*/
	}
}

void CPlayerRecvPackets::SetMute( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0;
		bool bMute = false;

		bs.ReadCompressed( uc );
		bs.Read( bMute );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			pPlayer->SetMuted( bMute );
		} 
	}
}

void CPlayerRecvPackets::SetIgnore( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0, uc2 = 0;
		bool bMute = false;

		bs.ReadCompressed( uc );
		bs.ReadCompressed( uc2 );
		bs.Read( bMute );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CPlayerPool* pIgnore = CPlayerPoolManager::Find( uc2 );
			if ( pIgnore )
			{
				pPlayer->SetIgnored( uc2, bMute );
			}
		} 
	}
}

void CPlayerRecvPackets::SetLocalMarker( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0, uc2 = 0, uc3 = 0;

		bs.ReadCompressed( uc );
		bs.ReadCompressed( uc2 );
		bs.ReadCompressed( uc3 );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc2 );
			if ( pPlayerTo )
			{
				CNetSends::SetPlayerLocalMarker( uc, uc2, uc3 );
			}
		} 
	}
}

void CPlayerRecvPackets::RemoveLocalMarker( void )
{
	CRconPool* pRcon = CRconPoolManager::Find( (unsigned char)m_pPacket->playerIndex );
	if ( pRcon )
	{
		unsigned char uc = 0, uc2 = 0;

		bs.ReadCompressed( uc );
		bs.ReadCompressed( uc2 );

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
		if ( pPlayer )
		{
			CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc2 );
			if ( pPlayerTo )
			{
				CNetSends::RemovePlayerLocalMarker( uc, uc2 );
			}
		}
	}
}