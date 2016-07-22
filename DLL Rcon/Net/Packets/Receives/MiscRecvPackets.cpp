#include "MiscRecvPackets.h"

RakNet::BitStream CMiscRecvPackets::bs;
Packet* CMiscRecvPackets::m_pPacket = NULL;

unsigned char CGameSettings::m_ucWeather = 0;
unsigned char CGameSettings::m_ucHour = 0;
unsigned char CGameSettings::m_ucMinute = 0;
unsigned char CGameSettings::m_ucRate = 0;
unsigned char CGameSettings::m_ucWeatherRate = 0;
bool CGameSettings::m_bScoreBoard = true;
bool CGameSettings::m_bDriveby = true;
char CGameSettings::m_szPass[ MAX_PASS_LEN ] = { "none set" };
char CGameSettings::m_szServerName[ MAX_SERVNAME_LEN ] = { "none set" };
char CGameSettings::m_szServerMode[ MAX_GAMEMODE_LEN ] = { "none set" };
unsigned char CGameSettings::m_ucMaxPlayers = 0;

extern bool bRunning;

void CMiscRecvPackets::Parse( Packet* p )
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
		case ID_MISC_JOINDECLINED:
			RconReject();
			break;

		case ID_MISC_RCONLIST:
			RconList();
			break;

		case ID_MISC_PLAYERLIST:
			PlayerList();
			break;

		case ID_MISC_VEHICLELIST:
			VehicleList();
			break;

		case ID_MISC_PICKUPLIST:
			PickupList();
			break;

		case ID_MISC_AMMULIST:
			AmmuList();
			break;

		case ID_MISC_SERVERDATA:
			ServerData();
			break;

		case ID_MISC_RCONCHAT:
			AdminChat();
			break;

		case ID_MISC_RCONMSG:
			AdminPM();
			break;

		case ID_MISC_RCONANN:
			Announce();
			break;

		case ID_MISC_RCONANNALL:
			AnnounceAll();
			break;
		}
	}
}

void CMiscRecvPackets::RconReject(void)
{
	unsigned int ui = 0;

	bs.ReadCompressed( ui );

	if (ui == REJECT_REASON_BAD_VERSION)
	{
		CallSignal( "vcmp.server.disconnected", "5" );
	}
	else if (ui == REJECT_REASON_BAD_NICKNAME)
	{
		CallSignal( "vcmp.server.disconnected", "6" );
	}

	bRunning = false;
}

void CMiscRecvPackets::RconList(void)
{
	unsigned char uc = 0, uc2 = 0, uc3 = 0;
	char szNick[ MAX_NAME_LEN ] = { NULL };
	char szIP[ MAX_IP_LEN ] = { NULL };

	bs.ReadCompressed(uc);

	while(uc2 < uc)
	{
		bs.ReadCompressed( uc3 );
		stringCompressor->DecodeString(szNick, MAX_NAME_LEN, &bs);
		stringCompressor->DecodeString(szIP, MAX_IP_LEN, &bs);

		CRconPool* pRcon = CRconPoolManager::Find( uc3 );
		if ( !pRcon )
		{
			pRcon = CRconPoolManager::New( uc3 );
			if ( pRcon ) 
			{
				pRcon->SetID( uc3 );
			}
		}

		if ( pRcon )
		{
				pRcon->SetNick( szNick );
				pRcon->SetIP( szIP );
		}
		uc2++;
	}
}

void CMiscRecvPackets::PlayerList(void)
{
	unsigned char uc = 0, uc2 = 0, uc3 = 0;
	char szNick[ MAX_NAME_LEN ] = { NULL };
	char szIP[ MAX_IP_LEN ] = { NULL };
	float fX = 0.0, fY = 0.0, fZ = 0.0, fRot = 0.0;
	unsigned char ucHealth = 0, ucArmour = 0, ucWeapon = 0, ucClass = 0, ucTeam = 0, ucSkin = 0;
	int iCash = 0, iScore;
	bool bFrozen = false, bMuted = false, bSpawned = false;

	bs.ReadCompressed(uc);

	while(uc2 < uc)
	{
		bs.ReadCompressed( uc3 );
		stringCompressor->DecodeString(szNick, MAX_NAME_LEN, &bs);
		stringCompressor->DecodeString(szIP, MAX_IP_LEN, &bs);
		bs.ReadVector(fX,fY,fZ);
		bs.Read(fRot);
		bs.ReadCompressed(ucHealth);
		bs.ReadCompressed(ucArmour);
		bs.ReadCompressed(ucWeapon);
		bs.ReadCompressed(iCash);
		bs.Read(bFrozen);
		bs.Read(bMuted);
		bs.ReadCompressed(iScore);
		bs.Read(bSpawned);
		bs.ReadCompressed(ucClass);
		bs.ReadCompressed(ucTeam);
		bs.ReadCompressed(ucSkin);

		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc3 );
		if ( !pPlayer )
		{
			pPlayer = CPlayerPoolManager::New( uc3 );
			if ( pPlayer ) 
			{
				pPlayer->SetNick(szNick);
				pPlayer->SetIP(szIP);

				CallSignal("vcmp.player.join", "%i %s %s", uc3, szNick, szIP);
			}
		}

		if ( pPlayer )
		{
			float fAngle = (fRot>0 ? fRot*180/PI : (2*PI+fRot)*180/PI);

			pPlayer->SetX(fX);
			pPlayer->SetY(fY);
			pPlayer->SetZ(fZ);
			pPlayer->SetRotation(fAngle);
			pPlayer->SetHealth(ucHealth);
			pPlayer->SetArmour(ucArmour);
			pPlayer->SetWeapon(ucWeapon);
			pPlayer->SetCash(iCash);
			pPlayer->SetFrozen(bFrozen);
			pPlayer->SetMuted(bMuted);
			pPlayer->SetScore(iScore);
			pPlayer->SetSpawned(bSpawned);
			pPlayer->SetClassID(ucClass);
			pPlayer->SetTeam(ucTeam);
			pPlayer->SetSkin(ucSkin);
		}
		uc2++;
	}
}

void CMiscRecvPackets::VehicleList(void)
{
	unsigned char uc = 0, uc2 = 0, uc3 = 0, uc4 = 0;
	float fX = 0.0, fY = 0.0, fZ = 0.0, fHealth = 0.0;
	bool bLocked = false;

	bs.ReadCompressed( uc );

	while( uc2 < uc )
	{
		bs.ReadCompressed( uc3 );
		bs.ReadCompressed( uc4 );
		bs.ReadVector( fX, fY, fZ );
		bs.Read( fHealth );
		bs.Read( bLocked );

		CVehiclePool* pVehicle = CVehiclePoolManager::Find( uc3 );
		if ( !pVehicle )
		{
			pVehicle = CVehiclePoolManager::New( uc3 );
			if ( pVehicle ) 
			{
				pVehicle->SetModel( uc4 );
			}
		}

		if ( pVehicle )
		{
			pVehicle->SetX( fX );
			pVehicle->SetY( fY );
			pVehicle->SetZ( fZ );
			pVehicle->SetHealth( fHealth );
			pVehicle->SetLocked( bLocked );
		}
		uc2++;
	}
}

void CMiscRecvPackets::PickupList(void)
{
	unsigned int ui = 0, ui2 = 0, ui3 = 0, ui4 = 0;
	float fX = 0.0, fY = 0.0, fZ = 0.0;

	bs.Read( ui );

	while( ui2 < ui )
	{
		bs.Read( ui3 );
		bs.Read( ui4 );
		bs.ReadVector( fX, fY, fZ );
		CPickupPool* pPickup = CPickupPoolManager::Find( ui3 );
		if ( !pPickup )
		{
			pPickup = CPickupPoolManager::New( ui3 );
			if ( pPickup )
			{
				pPickup->SetModel( ui4 );
			}
		}

		if ( pPickup )
		{
			pPickup->SetX( fX );
			pPickup->SetY( fY );
			pPickup->SetZ( fZ );
		}
		ui2++;
	}
}

void CMiscRecvPackets::AmmuList(void)
{
	unsigned char uc = 0, uc2 = 0, uc3 = 0, uc4 = 0, uc5 = 0, uc6 = 0, uc7 = 0;
	unsigned int ui = 0, ui2 = 0, ui3 = 0, ui4 = 0;
	bool bEnable = false, bEnable2 = false, bEnable3 = false, bEnable4 = false;

	bs.ReadCompressed( uc );

	while( uc < uc2 )
	{
		bs.ReadCompressed( uc3 );
		bs.ReadCompressed( uc4 );
		bs.ReadCompressed( uc5 );
		bs.ReadCompressed( uc6 );
		bs.ReadCompressed( uc7 );
		bs.ReadCompressed( ui );
		bs.ReadCompressed( ui2 );
		bs.ReadCompressed( ui3 );
		bs.ReadCompressed( ui4 );
		bs.Read( bEnable );
		bs.Read( bEnable2 );
		bs.Read( bEnable3 );
		bs.Read( bEnable4 );

		CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc3 );
		if ( !pAmmu )
		{
			pAmmu = CAmmuPoolManager::New( uc3 );
			if ( pAmmu )
			{
				pAmmu->SetWeapon( 0, uc4 );
				pAmmu->SetWeapon( 1, uc5 );
				pAmmu->SetWeapon( 2, uc6 );
				pAmmu->SetWeapon( 3, uc7 );
			}
		}

		if ( pAmmu )
		{
			pAmmu->SetWeapon( 0, uc4 );
			pAmmu->SetWeapon( 1, uc5 );
			pAmmu->SetWeapon( 2, uc6 );
			pAmmu->SetWeapon( 3, uc7 );
			pAmmu->SetWeaponCost( 0, ui );
			pAmmu->SetWeaponCost( 1, ui2 );
			pAmmu->SetWeaponCost( 2, ui3 );
			pAmmu->SetWeaponCost( 3, ui4 );
			pAmmu->SetWeaponEnabled( 0, bEnable );
			pAmmu->SetWeaponEnabled( 1, bEnable2 );
			pAmmu->SetWeaponEnabled( 2, bEnable3 );
			pAmmu->SetWeaponEnabled( 3, bEnable4 );
		}
		uc2++;
	}
}

void CMiscRecvPackets::ServerData(void)
{
	unsigned char uc1 = 0, uc2 = 0, uc3 = 0, uc4 = 0, uc5 = 0;
	unsigned int ui1 = 0;
	bool bScoreBoard = true, bDriveBy = true;
	char szPassword[ MAX_PASS_LEN ] = { NULL };
	char szServName[ MAX_SERVNAME_LEN ] = { NULL };
	char szServMode[ MAX_GAMEMODE_LEN ] = { NULL };

	bs.ReadCompressed(ui1); // Weather rate
	bs.ReadCompressed(uc1); // Time rate
	bs.ReadCompressed(uc2); // Weather
	bs.ReadCompressed(uc3); // Hour
	bs.ReadCompressed(uc4); // Minute
	bs.ReadCompressed(uc5); // Max players
	bs.Read(bScoreBoard); // Scoreboard
	bs.Read(bDriveBy); // Drive-by

	stringCompressor->DecodeString(szPassword, MAX_PASS_LEN, &bs);
	stringCompressor->DecodeString(szServName, MAX_SERVNAME_LEN, &bs);
	stringCompressor->DecodeString(szServMode, MAX_GAMEMODE_LEN, &bs);

	uc5 = ui1 /= 1000;

	CGameSettings::SetWeatherRate(ui1);
	CGameSettings::SetTimeRate(uc1);
	CGameSettings::SetWeather(uc2);
	CGameSettings::SetHour(uc3);
	CGameSettings::SetMinute(uc4);
	CGameSettings::SetMaxPlayers(uc5);
	CGameSettings::SetScoreBoard(bScoreBoard);
	CGameSettings::SetDrivebyEnabled(bDriveBy);
	CGameSettings::SetPassword(szPassword);
	CGameSettings::SetServerName(szServName);
	CGameSettings::SetServerMode(szServMode);
}

void CMiscRecvPackets::AdminChat(void)
{
	unsigned char uc = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed( uc );

	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		CallSignal("vcmp.admin.chat", "%s %s", pRcon->GetNick(), szText);
	}
}

void CMiscRecvPackets::AdminPM(void)
{
	unsigned char uc = 0, uc2 = 0;

	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed( uc );
	bs.ReadCompressed( uc2 );

	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc2 );
		if ( pPlayer )
		{
			CallSignal("vcmp.admin.message", "%s %i %s %s", pRcon->GetNick(), uc2, pPlayer->GetNick(), szText);
		}
	}
}

void CMiscRecvPackets::Announce(void)
{
	unsigned char uc = 0, uc2 = 0;

	char szText[ MAX_TEXT_LEN ] = { NULL };

	bs.ReadCompressed( uc );
	bs.ReadCompressed( uc2 );

	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc2 );
		if ( pPlayer )
		{
			CallSignal("vcmp.admin.announce", "%s %i %s %s", pRcon->GetNick(), uc2, pPlayer->GetNick(), szText);
		}
	}
}

void CMiscRecvPackets::AnnounceAll(void)
{
	unsigned char uc = 0;
	char szText[ MAX_TEXT_LEN ] = { NULL };

	stringCompressor->DecodeString(szText, MAX_TEXT_LEN, &bs);

	CRconPool* pRcon = CRconPoolManager::Find( uc );
	if ( pRcon )
	{
		CallSignal("vcmp.admin.announceall", "%s %s", pRcon->GetNick(), szText);
	}
}