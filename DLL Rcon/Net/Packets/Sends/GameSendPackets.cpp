#include "GameSendPackets.h"

void CGameSendPackets::PreparePacket( unsigned char uc, RakNet::BitStream &bs )
{
	bs.Write( (unsigned char)ID_GAME_PACKET );
	bs.WriteCompressed( uc );
}

void CGameSendPackets::SetWeather(unsigned char uc)
{
	RakNet::BitStream bs;

	PreparePacket(ID_GAME_SETWEATHER, bs);

	bs.WriteCompressed(uc);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CGameSendPackets::SetTime(unsigned char uc1, unsigned char uc2)
{
	RakNet::BitStream bs;

	PreparePacket(ID_GAME_SETTIME, bs);

	bs.WriteCompressed(uc1);
	bs.WriteCompressed(uc2);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CGameSendPackets::SetMarker(unsigned char uc, float fX, float fY, float fZ)
{
	RakNet::BitStream bs;

	PreparePacket(ID_GAME_SETMARKER, bs);

	bs.WriteCompressed(uc);
	bs.WriteVector(fX,fY,fZ);

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CGameSendPackets::EnableWeaponAtAmmu( unsigned char uc, unsigned char uc2 )
{
	CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc );
	if ( pAmmu )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_GAME_AMMUWEAPON, bs);

		bs.WriteCompressed( uc );
		bs.WriteCompressed( uc2 );
		bs.Write( pAmmu->GetWeaponEnabled( uc2 ) );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}

void CGameSendPackets::SetTimeRate( unsigned char uc )
{
	RakNet::BitStream bs;

	PreparePacket(ID_GAME_SETTIMERATE, bs);

	bs.WriteCompressed( uc );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CGameSendPackets::SetWeatherRate( unsigned char uc )
{
	RakNet::BitStream bs;

	PreparePacket(ID_GAME_SETWEATHERRATE, bs);

	bs.WriteCompressed( uc );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CGameSendPackets::CreateMarkerForPlayer(unsigned char uc1, unsigned char uc2, float fX, float fY, float fZ)
{
	RakNet::BitStream bs;

	PreparePacket(ID_GAME_SETMARKER4PLAYER, bs);

	bs.WriteCompressed( uc1 );
	bs.WriteCompressed( uc2 );
	bs.WriteVector( fX, fY, fZ );

	CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
}

void CGameSendPackets::SetAmmuWeapon( unsigned char uc1, unsigned char uc2 )
{
	CAmmuPool* pAmmu = CAmmuPoolManager::Find( uc1 );
	if ( pAmmu )
	{
		RakNet::BitStream bs;

		PreparePacket(ID_GAME_SETMAMMUWEAPON, bs);

		bs.WriteCompressed( uc1 );
		bs.WriteCompressed( uc2 );
		bs.WriteCompressed( pAmmu->GetWeapon( uc2 ) );
		bs.WriteCompressed( pAmmu->GetWeaponCost( uc2 ) );

		CNet::GetInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE, 0);
	}
}