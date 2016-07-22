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
			Weather();
			break;

		case ID_GAME_SETTIME:
			Time();
			break;

		case ID_GAME_UPDATETIMERATE:
			TimeRate();
			break;
		}

	}
}

void CGameRecvPackets::Weather( void )
{
	unsigned char ucRcon = 0, ucWeather = 0;

	bs.ReadCompressed( ucRcon );
	bs.ReadCompressed( ucWeather );

	CRconPool* pRcon = CRconPoolManager::Find( ucRcon );
	if ( pRcon )
	{
		CallSignal( "vcmp.weatherchange", "%s %i %i", pRcon->GetNick(), CGameSettings::GetWeather(), ucWeather );
	}
	else if ( ucRcon == 255 )
	{
		CallSignal("vcmp.weatherchange", "Server %i", CGameSettings::GetWeather(), ucWeather);
	}
	CGameSettings::SetWeather( ucWeather );
}

void CGameRecvPackets::Time( void )
{
	unsigned char ucRcon = 0, ucHour = 0, ucMinute = 0;

	bs.ReadCompressed( ucRcon );
	bs.ReadCompressed( ucHour );
	bs.ReadCompressed( ucMinute );

	CRconPool* pRcon = CRconPoolManager::Find( ucRcon );
	if ( pRcon )
	{
		CallSignal("vcmp.timechange", "%s %i %i %i %i", pRcon->GetNick(), CGameSettings::GetHour(), CGameSettings::GetMinute(), ucHour, ucMinute);
	}
	else if ( ucRcon == 255 )
	{
		CallSignal("vcmp.timechange", "Server %i %i %i %i", CGameSettings::GetHour(), CGameSettings::GetMinute(), ucHour, ucMinute);
	}

	CGameSettings::SetHour( ucHour );
	CGameSettings::SetMinute( ucMinute );
}

void CGameRecvPackets::TimeRate( void )
{
	unsigned char ucRcon = 0, ucTimeRate = 0;

	bs.ReadCompressed( ucRcon );
	bs.ReadCompressed( ucTimeRate );

	CRconPool* pRcon = CRconPoolManager::Find( ucRcon );
	if ( pRcon )
	{
		CGameSettings::SetTimeRate( ucTimeRate );
	}
}