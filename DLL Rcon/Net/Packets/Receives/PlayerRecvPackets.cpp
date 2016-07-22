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
		case ID_PLAYER_JOIN:
			Join();
			break;

		case ID_PLAYER_PART:
			Part();
			break;

		case ID_PLAYER_SPAWN:
			Spawn();
			break;

		case ID_PLAYER_CHAT:
			Chat();
			break;

		case ID_PLAYER_ACTION:
			Action();
			break;

		case ID_PLAYER_MSG:
			PM();
			break;

		case ID_PLAYER_TEAMCHAT:
			TeamChat();
			break;

		case ID_PLAYER_KILL:
			Kill();
			break;

		case ID_PLAYER_TEAMKILL:
			TeamKill();
			break;

		case ID_PLAYER_DEATH:
			Death();
			break;

		case ID_PLAYER_COMMAND:
			Command();
			break;

		case ID_PLAYER_STARTSPEC:
			StartSpectating();
			break;

		case ID_PLAYER_STOPSPEC:
			StopSpectating();
			break;

		case ID_PLAYER_SYNCDATA:
			SyncData();
			break;

		case ID_PLAYER_VEHICLEENTER:
			VehicleEnter();
			break;

		case ID_PLAYER_VEHICLEEXIT:
			VehicleExit();
			break;
		}
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::Join(void)
{
	unsigned char uc = 0;
	char szNick[ MAX_NAME_LEN ] = { NULL };
	char szIP[ MAX_IP_LEN ] = { NULL };

	bs.ReadCompressed(uc);
	stringCompressor->DecodeString(szNick, MAX_NAME_LEN, &bs);
	stringCompressor->DecodeString(szIP, MAX_IP_LEN, &bs);

	CPlayerPool* pPlayer = CPlayerPoolManager::New( uc );

	if ( pPlayer )
	{
		pPlayer->SetNick(szNick);
		pPlayer->SetIP(szIP);
		CallSignal("vcmp.player.join", "%i %s %s", uc, szNick, szIP);
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::Part(void)
{
	unsigned char uc1 = 0, uc2 = 0;

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CallSignal("vcmp.player.part", "%i %s %i %s", uc1, pPlayer->GetNick(), uc2, pPlayer->GetIP());
		CPlayerPoolManager::Remove( pPlayer );
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::Spawn(void)
{
	unsigned char uc1 = 0, uc2 = 0, uc3 = 0;

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);
	bs.ReadCompressed(uc3);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		pPlayer->SetSkin( uc2 );
		pPlayer->SetTeam( uc3 );
		CallSignal("vcmp.player.spawn", "%i %s %i %i", uc1, pPlayer->GetNick(), uc2, uc3);
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::Chat(void)
{
	unsigned char uc = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed(uc);
	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		if ((szText[0] == '!') && (strlen(szText) > 1))
		{
			strncpy(szText, szText + 1, strlen(szText));
			CallSignal( "vcmp.player.command", "%i %s %s", uc, pPlayer->GetNick(), szText);
		}
		else CallSignal("vcmp.player.chat", "%i %s %s", uc, pPlayer->GetNick(), szText);
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::Action(void)
{
	unsigned char uc = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed(uc);
	
	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer ) 
	{
		CallSignal("vcmp.player.action", "%i %s %s", uc, pPlayer->GetNick(), szText);
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::PM(void)
{
	unsigned char uc1 = 0, uc2 = 0, uc3 = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed( uc1 );
	bs.ReadCompressed( uc2 );
	bs.ReadCompressed( uc3 );

	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	if (uc1 == 0)
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc2 );
		if ( pPlayer )
		{
			CPlayerPool* pPlayerTo = CPlayerPoolManager::Find( uc3 );
			if ( pPlayerTo )
			{
				CallSignal("vcmp.player.msg", "%i %s %i %s %s", uc2, pPlayer->GetNick(), uc3, pPlayerTo->GetNick(), szText);
			}
		}
	}
	else
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc2 );
		if ( pPlayer )
		{
			CallSignal("vcmp.player.msg", "%i %s %i Unknown %s", uc2, pPlayer->GetNick(), uc3, szText );
		}
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::TeamChat(void)
{
	unsigned char uc1 = 0, uc2 = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);
	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CallSignal("vcmp.player.teamchat", "%i %s %i %s", uc1, pPlayer->GetNick(), uc2, szText);
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::Kill(void)
{
	unsigned char uc1 = 0, uc2 = 0, uc3 = 0, uc4 = 0, uc5 = 0;

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);
	bs.ReadCompressed(uc3);
	bs.ReadCompressed(uc4);
	bs.ReadCompressed(uc5);

	CPlayerPool* pKiller = CPlayerPoolManager::Find( uc1 );
	if ( pKiller )
	{
		CPlayerPool* pKilled = CPlayerPoolManager::Find( uc3 );
		if ( pKilled )
		{
			CallSignal("vcmp.player.kill", "%i %s %i %i %s %i %i", uc1, pKiller->GetNick(), uc2, uc3, pKilled->GetNick(), uc4, uc5);
		}
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::TeamKill(void)
{
	unsigned char uc1 = 0, uc2 = 0, uc3 = 0, uc4 = 0, uc5 = 0;

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);
	bs.ReadCompressed(uc3);
	bs.ReadCompressed(uc4);
	bs.ReadCompressed(uc5);

	CPlayerPool* pKiller = CPlayerPoolManager::Find( uc1 );
	if ( pKiller )
	{
		CPlayerPool* pKilled = CPlayerPoolManager::Find( uc3 );
		if ( pKilled )
		{
			CallSignal("vcmp.player.teamkill", "%i %s %i %i %s %i %i", uc1, pKiller->GetNick(), uc2, uc3, pKilled->GetNick(), uc4, uc5);
		}
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::Death(void)
{
	unsigned char uc1 = 0, uc2 = 0;

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CallSignal("vcmp.player.death", "%i %s %i", uc1, pPlayer->GetNick(), uc2);
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::Command(void)
{
	unsigned char uc = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed(uc);
	
	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer ) 
	{
		CallSignal("vcmp.player.command2", "%i %s %s", uc, pPlayer->GetNick(), szText);
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::StartSpectating(void)
{
	unsigned char uc = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed(uc);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer ) 
	{
		pPlayer->SetSpectating( true );
		CallSignal("vcmp.player.startspectating", "%i %s", uc, pPlayer->GetNick());
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::StopSpectating(void)
{
	unsigned char uc = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed(uc);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer ) 
	{
		pPlayer->SetSpectating( false );
		CallSignal("vcmp.player.stopspectating", "%i %s", uc, pPlayer->GetNick());
	}
}

void CPlayerRecvPackets::VehicleEnter(void)
{
	unsigned char uc1 = 0, uc2 = 0, uc3 = 0, uc4 = 0;
	float fHealth = 0.0;

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);
	bs.ReadCompressed(uc3);
	bs.Read( fHealth );
	bs.ReadCompressed(uc4);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc2 );
		if ( pVehicle )
		{
			if (uc4 == 1) CallSignal("vcmp.vehicle.passenger.enter", "%i %s %i %i %f", uc1, pPlayer->GetNick(), uc2, uc3, fHealth);
			else 
			{
				pVehicle->SetDriver( uc1 );
				pPlayer->SetVehicle( uc2 );
				CallSignal("vcmp.vehicle.driver.enter", "%i %s %i %i %f", uc1, pPlayer->GetNick(), uc2, uc3, fHealth);
			}
		}
	}
}

void CPlayerRecvPackets::VehicleExit(void)
{
	unsigned char uc1 = 0, uc2 = 0, uc3 = 0;
	float fHealth = 0.0;

	bs.ReadCompressed(uc1);
	bs.ReadCompressed(uc2);
	bs.ReadCompressed(uc3);
	bs.Read( fHealth );

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc1 );
	if ( pPlayer )
	{
		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc2 );
		if ( pVehicle )
		{
			pPlayer->SetVehicle( -1 );
			pVehicle->SetDriver( INVALID_PLAYER_ID );
			CallSignal("vcmp.vehicle.exit", "%i %s %i %i %f", uc1, pPlayer->GetNick(), uc2, uc3, fHealth);
		}
	}
}

//-------------------------------------------------------------------------------//

void CPlayerRecvPackets::SyncData(void)
{
	unsigned char uc = 0;
	float fX = 0.0, fY = 0.0, fZ = 0.0, fRot = 0.0;
	BYTE bHealth = 0, bArmour = 0, bWeapon = 0;

	bs.ReadCompressed(uc);
	bs.ReadVector(fX,fY,fZ);
	bs.Read(fRot);
	bs.ReadCompressed(bHealth);
	bs.ReadCompressed(bArmour);
	bs.ReadCompressed(bWeapon);

	CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
	if ( pPlayer )
	{
		if ((pPlayer->GetX() != fX) && (pPlayer->GetY() != fY) && (pPlayer->GetZ() != fZ))
		{
			CallSignal("vcmp.player.move", "%i %s %f %f %f %f %f %f", uc, pPlayer->GetNick(), pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetZ(), fX, fY, fZ );
		}

		if (pPlayer->GetHealth() != bHealth) CallSignal( "vcmp.player.healthchange", "%i %s %i %i", uc, pPlayer->GetNick(), pPlayer->GetHealth(), bHealth );
		if (pPlayer->GetArmour() != bArmour) CallSignal( "vcmp.player.armourchange", "%i %s %i %i", uc, pPlayer->GetNick(), pPlayer->GetHealth(), bArmour );
		if (pPlayer->GetWeapon() != bWeapon) CallSignal( "vcmp.player.weaponchange", "%i %s %i %i", uc, pPlayer->GetNick(), pPlayer->GetWeapon(), bWeapon );

		float fAngle = (fRot>0 ? fRot*180/PI : (2*PI+fRot)*180/PI);
		pPlayer->SetX(fX);
		pPlayer->SetY(fY);
		pPlayer->SetZ(fZ);
		pPlayer->SetRotation(fAngle);
		pPlayer->SetHealth(bHealth);
		pPlayer->SetArmour(bArmour);
		pPlayer->SetWeapon(bWeapon);
		pPlayer->SetVehicle( -1 );
	}
}