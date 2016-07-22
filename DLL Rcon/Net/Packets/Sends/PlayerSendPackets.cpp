#include "PlayerSendPackets.h"
//#include "Pools.h"

void CPlayerSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_PLAYER_PACKET );
	bs.WriteCompressed( uc );
}

void CPlayerSendPackets::SetPlayerPosition( unsigned char uc1, float fX, float fY, float fZ, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETPOSITION, bs);

		bs.WriteCompressed( uc1 );
		bs.WriteVector( fX, fY, fZ );
		bs.WriteCompressed( uc2 );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerHealth( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETHEALTH, bs);

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerArmour( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETARMOUR, bs);

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerFrozen( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETFROZEN, bs);

		bs.WriteCompressed( uc );
		bs.Write( pPlayer->GetFrozen() );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerWeapon( unsigned char uc1, unsigned char uc2, unsigned int ui )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETWEAPON, bs);

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );
		bs.WriteCompressed( ui );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerCash( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETCASH, bs);

		bs.WriteCompressed( uc );
		bs.WriteCompressed( pPlayer->GetCash() );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerMute( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETMUTE, bs);

		bs.WriteCompressed( uc );
		bs.Write( pPlayer->GetMuted() );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerIgnore( unsigned char uc, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc2 );
		if ( pPlayer )
		{
			RakNet::BitStream bs;

			PreparePacket(ID_PLAYER_SETIGNORE, bs);

			bs.WriteCompressed( uc );
			bs.Write( pPlayer->GetIgnored( uc2 ) );

			CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
		}
	}
}

void CPlayerSendPackets::SetPlayerScore( unsigned char uc, int iScore )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETSCORE, bs);

		bs.WriteCompressed( uc );
		bs.WriteCompressed( iScore );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::IncPlayerScore( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_INCSCORE, bs);

		bs.WriteCompressed( uc );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::DecPlayerScore( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_DECSCORE, bs);

		bs.WriteCompressed( uc );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerAnim( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_DOANIM, bs);

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::SetPlayerMarker( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_SETMARKER, bs);

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::RemovePlayerMarker( unsigned char uc )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_PLAYER_REMOVEMARKER, bs);

		bs.WriteCompressed( uc );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CPlayerSendPackets::PutInVehicle( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc2 );
		if ( pVehicle )
		{
			RakNet::BitStream bs;

			PreparePacket(ID_PLAYER_PUTINVEHICLE, bs);

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );

			CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
		}
	}
}

void CPlayerSendPackets::SetLocalMarker( unsigned char uc1, unsigned char uc2, unsigned char uc3 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc2 );
		if ( pPlayerTo )
		{
			RakNet::BitStream bs;

			PreparePacket(ID_PLAYER_SETLMARKER, bs);

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc3 );
			bs.WriteCompressed( uc2 );

			CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
		}
	}
}

void CPlayerSendPackets::RemoveLocalMarker( unsigned char uc1, unsigned char uc2 )
{
	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc2 );
		if ( pPlayerTo )
		{
			RakNet::BitStream bs;

			PreparePacket(ID_PLAYER_SETLMARKER, bs);

			bs.WriteCompressed( uc1 );
			bs.WriteCompressed( uc2 );

			CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
		}
	}
}