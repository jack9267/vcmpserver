/*
	SQMiscFuncs.cpp
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

	File Author: Juppi

	--------------------------------------------------------------------

	Edited: 5/5/2009

	--------------------------------------------------------------------
*/


#include "SQMiscFuncs.h"
#include "Console.h"
#include "Pools.h"
#include "Utils.h"
#include "BanList.h"
#include "time.h"

using namespace SqPlus;

_SQUIRRELDEF( CSQMiscFuncs::Print )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		const SQChar* sz = NULL;

		sq_getstring( v, 2, &sz );

		CConsole::PrintC( "SCRIPT", COLOUR_CYAN, (char*)sz );

		sq_pushbool( v, true );
		return 1;
	}

	sq_pushbool( v, true );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::Message )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		const SQChar* szMessage = NULL;
		sq_getstring( v, 2, &szMessage );

		CNetSends::AdminChat( (char*)szMessage );
		sq_pushbool( v, (SQBool)true );
		return 1;
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::MessageAllExcept )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		const SQChar* szMessage = NULL;
		sq_getstring( v, 2, &szMessage );

		_CHECK_INST_PARAM( pPlayer, 3, CPlayerPool, Player );

		if ( pPlayer )
		{	
			CNetSends::AdminChatAllExcept( pPlayer->GetPlayerID(), (char*)szMessage );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::MessagePlayer )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		const SQChar* szMessage = NULL;
		sq_getstring( v, 2, &szMessage );

		_CHECK_INST_PARAM( pPlayer, 3, CPlayerPool, Player );

		if ( pPlayer )
		{	
			CNetSends::AdminChatToPlayer( pPlayer->GetPlayerID(), (char*)szMessage );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::PrivMessage )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		const SQChar* szMessage = NULL;
		sq_getstring( v, 2, &szMessage );

		 _CHECK_INST_PARAM( pPlayer, 3, CPlayerPool, Player );

		if ( pPlayer )
		{	
			CNetSends::AdminMsg( (unsigned int)pPlayer->GetID(), (char*)szMessage );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::Announce )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		const SQChar* szMessage = NULL;
		sq_getstring( v, 2, &szMessage );

		_CHECK_INST_PARAM( pPlayer, 3, CPlayerPool, Player );

		if ( pPlayer )
		{	
			CNetSends::Announce( (unsigned int)pPlayer->GetID(), (char*)szMessage );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::AnnounceAll )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		const SQChar* szMessage = NULL;
		sq_getstring( v, 2, &szMessage );

		unsigned char uc = 0;
		while ( ( uc < MAX_PLAYERS ) && ( uc < CPlayerPoolManager::Count() ) )
		{
			CPlayerPool* pPlayer = CPlayerPoolManager::Find( uc );
			if ( pPlayer )
			{
				CNetSends::Announce( uc, (char*)szMessage );
			}
			uc++;
		}
		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SendPlayerMessage )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 4 )
	{
		_CHECK_INST_PARAM( pPlayer, 2, CPlayerPool, Player );
		_CHECK_INST_PARAM( pPlayerTo, 3, CPlayerPool, Player );

		const SQChar* szMessage = NULL;
		sq_getstring( v, 4, &szMessage );

		if ( ( pPlayerTo ) && ( pPlayer ) )
		{	
			CNetSends::ChatToPlayer( pPlayerTo->GetID(), pPlayer->GetID(), (char*)szMessage );
			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetTickCount )
{
	sq_pushinteger( v, (SQInteger)RakNet::GetTime() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetTime )
{
	sq_pushinteger( v, time( NULL ) );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetFullTime )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 1 )
	{
		time_t iTime;
		struct tm * pTime;

		time( &iTime );
		pTime = localtime( &iTime );

		sq_pushstring( v, asctime( pTime ), -1 );

		return 1;
	}

	sq_pushnull( v );

	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::IsNum )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		const SQChar* sz = NULL;

		sq_getstring( v, 2, &sz );

		if ( CUtils::IsNum( (char*)sz ) )
		{
			sq_pushbool( v, true );

			return 1;
		}
	}

	sq_pushbool( v, false );

	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::KickPlayer )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		_CHECK_INST_PARAM( pPlayer, 2, CPlayerPool, Player );
		/*sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;*/
		if ( pPlayer )
		{	
			CNetSends::KickPlayer( pPlayer->GetID() );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::BanPlayer )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		_CHECK_INST_PARAM( pPlayer, 2, CPlayerPool, Player );
		/*sq_getuserpointer( v, 2, &pPointer );

		CPlayerPool* pPlayer = (CPlayerPool*)pPointer;*/
		if ( pPlayer )
		{	
			CNetSends::BanPlayer( pPlayer->GetID() );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::BanIP )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		const SQChar* szIP = 0;
		sq_getstring( v, 2, &szIP );

		CBanList::AddToBanList( (char*)szIP );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::UnbanIP )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		const SQChar* szIP = 0;
		sq_getstring( v, 2, &szIP );

		CBanList::RemoveFromBanList( (char*)szIP );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetServerName )
{
	sq_pushstring( v, (SQChar*)CConfigLoader::GetServerName(), -1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SetServerName )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		extern SERVER_INFO pServerInfo;
		
		const SQChar* szName = 0;
		sq_getstring( v, 2, &szName );

		if ( strlen( szName ) <=  MAX_SERVNAME_LEN )
		{
			pServerInfo.ucServerNameLen = strlen( (char*)szName );
			strcpy( pServerInfo.szServerName, (char*)szName );
			CConfigLoader::SetServerName( (char*)szName );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetGamemodeName )
{
	sq_pushstring( v, (SQChar*)CConfigLoader::GetGamemodeName(), -1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SetGamemodeName )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		extern SERVER_INFO pServerInfo;

		const SQChar* szMode = 0;
		sq_getstring( v, 2, &szMode );

		if ( strlen( szMode ) <= MAX_GAMEMODE_LEN )
		{
			strcpy( pServerInfo.szMode, (char*)szMode );
			CConfigLoader::SetGamemodeName( (char*)szMode );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetMapName )
{
	extern SERVER_INFO pServerInfo;

	sq_pushstring( v, (SQChar*)pServerInfo.szMap, -1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SetMapName )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		extern SERVER_INFO pServerInfo;

		const SQChar* szMap = 0;
		sq_getstring( v, 2, &szMap );

		if ( strlen( szMap ) <= MAX_GAMEMODE_LEN )
		{
			strcpy( pServerInfo.szMap, (char*)szMap );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetPassword )
{
	sq_pushstring( v, (SQChar*)CConfigLoader::GetPassword(), -1 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SetPassword )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		const SQChar* szPasswd = 0;
		sq_getstring( v, 2, &szPasswd );

		if ( strlen( szPasswd ) <= MAX_PASS_LEN )
		{
			CConfigLoader::SetPassword( (char*)szPasswd );
			NetInterface->SetPassword( (char*)szPasswd );

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetMaxPlayers )
{
	sq_pushinteger( v, CConfigLoader::GetMaxPlayers() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SetMaxPlayers )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		extern SERVER_INFO pServerInfo;

		SQInteger iMaxPlayers = NULL;
		sq_getinteger( v, 2, &iMaxPlayers );

		if ( iMaxPlayers > 0 )
		{
			if ( iMaxPlayers > MAX_PLAYERS )
			{
				pServerInfo.ucMaxPlayers = MAX_PLAYERS;
				CConfigLoader::SetMaxPlayers( MAX_PLAYERS );
				NetInterface->SetAllowedPlayers( MAX_PLAYERS );
			}
			else
			{
				pServerInfo.ucMaxPlayers = (unsigned char)iMaxPlayers;
				CConfigLoader::SetMaxPlayers( (unsigned char)iMaxPlayers );
				NetInterface->SetAllowedPlayers( (unsigned char)iMaxPlayers );
			}

			sq_pushbool( v, (SQBool)true );
			return 1;
		}
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetLocked )
{
	extern SERVER_INFO pServerInfo;

	sq_pushbool( v, pServerInfo.bLocked );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SetLocked )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		extern SERVER_INFO pServerInfo;

		SQBool bLocked = FALSE;
		sq_getbool( v, 2, &bLocked );

		pServerInfo.bLocked = bLocked;

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetPlayers )
{
	sq_pushinteger( v, CPlayerPoolManager::Count() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetDeathmatchScoreboard )
{
	sq_pushbool( v, CConfigLoader::GetDeathmatchScoreB() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SetDeathmatchScoreboard )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQBool bEnabled = FALSE;
		sq_getbool( v, 2, &bEnabled );

		CConfigLoader::SetDeathmatchScoreB( bEnabled );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetDrivebyEnabled )
{
	sq_pushbool( v, CConfigLoader::GetDrivebyEnabled() );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::SetDrivebyEnabled )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQBool bEnabled = FALSE;
		sq_getbool( v, 2, &bEnabled );

		CConfigLoader::SetDrivebyEnabled( bEnabled );

		sq_pushbool( v, (SQBool)true );
		return 1;
	}

	sq_pushbool( v, (SQBool)false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetWeaponName )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQInteger iID = NULL;
		sq_getinteger( v, 2, &iID );

		sq_pushstring( v, CUtils::GetDeathReason( (BYTE)iID ), -1);
		return 1;
	}

	sq_pushstring( v, "Unknown", 7 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetWeaponID )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		const SQChar* szWeapon = 0;
		sq_getstring( v, 2, &szWeapon );

		sq_pushinteger( v, CUtils::GetWeaponID( (char*)szWeapon ) );
		return 1;
	}

	sq_pushinteger( v, 255 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetDistrictName )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 3 )
	{
		SQFloat fX = NULL;
		sq_getfloat( v, 2, &fX );

		SQFloat fY = NULL;
		sq_getfloat( v, 3, &fY );

		sq_pushstring( v, CUtils::GetAreaName( fX, fY ), -1 );
		return 1;
	}

	sq_pushstring( v, "Vice-City", 9 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::GetSkinName )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQInteger iID = NULL;
		sq_getinteger( v, 2, &iID );

		sq_pushstring( v, CUtils::GetSkinName( (unsigned char)iID ), -1);
		return 1;
	}

	sq_pushstring( v, "Unknown", 7 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::InPoly )
{
	SQInteger iArgs = sq_gettop( v );

	if ( ( iArgs >= 9 ) && ( ( iArgs - 1 ) % 2 == 0 ) )
	{
		SQFloat fX = 0.0f, fY = 0.0f;
		sq_getfloat( v, 2, &fX );
		sq_getfloat( v, 3, &fY );

		AreaPoints areaPoints[ 128 ];

		SQFloat fPointX = 0.0f, fPointY = 0.0f;
		unsigned int uiPoints = ( iArgs - 3 );
		unsigned int uiThing = 4;
		for ( unsigned int ui = 0; ui < uiPoints; ui++ )
		{
			sq_getfloat( v, uiThing, &fPointX );
			sq_getfloat( v, uiThing + 1, &fPointY );

			areaPoints[ ui ].x = (float)fPointX;
			areaPoints[ ui ].y = (float)fPointY;

			uiThing += 2;
		}

		bool bRet = CUtils::InPoly( (float)fX, (float)fY, uiPoints, areaPoints );

		sq_pushbool( v, bRet );
		return 1;
	}
	else if ( iArgs == 4 )
	{
		if ( sq_gettype( v, 4 ) == OT_STRING )
		{
			SQFloat fX = 0.0f, fY = 0.0f;
			sq_getfloat( v, 2, &fX );
			sq_getfloat( v, 3, &fY );

			const SQChar* szSqParams = 0;
			sq_getstring( v, 4, &szSqParams );

			char szParams[ 512 ] = { 0 };
			strncpy( szParams, szSqParams, 512 );

			AreaPoints areaPoints[ 128 ];

			unsigned int uiPoints = CUtils::NumTok( szParams, "," );

			uiPoints /= 2;

			if ( uiPoints >= 3 )
			{
				areaPoints[ 0 ].x = (float)atof( strtok( szParams, "," ) );
				areaPoints[ 0 ].y = (float)atof( strtok( 0, "," ) );

				for ( unsigned int ui = 1; ui < uiPoints; ui++ )
				{
					areaPoints[ ui ].x = (float)atof( strtok( 0, "," ) );
					areaPoints[ ui ].y = (float)atof( strtok( 0, "," ) );
				}

				bool bRet = CUtils::InPoly( (float)fX, (float)fY, uiPoints, areaPoints );

				sq_pushbool( v, bRet );
				return 1;
			}
		}
		else if ( sq_gettype( v, 4 ) == OT_ARRAY )
		{
			SQFloat fX = 0.0f, fY = 0.0f;
			sq_getfloat( v, 2, &fX );
			sq_getfloat( v, 3, &fY );

			SQObject obj;
			sq_getstackobj( v, 4, &obj );

			SquirrelObject arr = (SquirrelObject)obj;

			AreaPoints areaPoints[ 128 ];

			unsigned int uiPoints = arr.Len() / 2;

			if ( uiPoints >= 3 )
			{
				for ( unsigned int ui = 0; ui < uiPoints; ui++ )
				{
					areaPoints[ ui ].x = arr.GetFloat( 2 * ui );
					areaPoints[ ui ].y = arr.GetFloat( 2 * ui + 1 );
				}

				bool bRet = CUtils::InPoly( (float)fX, (float)fY, uiPoints, areaPoints );

				sq_pushbool( v, bRet );
				return 1;
			}
		}
	}

	sq_pushbool( v, false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::CallFunc )
{
	// Arguments - Script, Function, ...
	SQInteger iArgs = sq_gettop( v ); // Get args

	if ( iArgs >= 3 )
	{
		const SQChar* szScript = NULL;
		const SQChar* szFunction = NULL;

		sq_getstring( v, 2, &szScript );
		sq_getstring( v, 3, &szFunction );

		if ( strlen( szScript ) )
		{
			CSquirrel* pScript = CSquirrelManager::Find( szScript );
			if ( pScript )
			{
				pScript->PushFunction( (char*)szFunction );

				unsigned char uc = 4;
				if ( iArgs > 3 )
				{
					for ( uc = 4; uc <= iArgs; uc++ )
					{
						SQInteger i = 0;
						SQFloat f = 0.0f;
						const SQChar* sz = NULL;
						SQUserPointer p = NULL;

						switch( sq_gettype( v, uc ) )
						{
						case OT_INTEGER:	
							sq_getinteger( v, uc, &i );

							pScript->PushInteger( (int)i );
							break;

						case OT_FLOAT:
							sq_getfloat( v, uc, &f );

							pScript->PushFloat( (float)f );
							break;

						case OT_STRING:
							sq_getstring( v, uc, &sz );

							pScript->PushString( (char*)sz );
							break;

						case OT_USERPOINTER:
							sq_getuserpointer( v, uc, &p );

							pScript->PushPointer( (void*)p );
							break;

						case OT_INSTANCE:
							if( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, (SQUserPointer)&__Player_decl ) ) )
							{
								// Push Player
								pScript->PushPlayerPointer( (CPlayerPool*)p );
							}
							else if( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, (SQUserPointer)&__Vehicle_decl ) ) )
							{
								// Push Vehicle
								pScript->PushVehiclePointer( (CVehiclePool*)p );
							}
							else if( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, (SQUserPointer)&__Pickup_decl ) ) )
							{
								// Push Pickup
								pScript->PushPickupPointer( (CPickupPool*)p );
							}
							else if( !SQ_FAILED( sq_getinstanceup( v, uc, (SQUserPointer*)&p, (SQUserPointer)&__Socket_decl ) ) )
							{
								// Push Socket
								pScript->PushSocketPointer( (CSquirrelSockets*)p );
							}
							break;
						}
					}
				}

				SquirrelRetn pRet = pScript->CallFunctionAndRetn();

				switch ( pRet.pType )
				{
				case ST_STRING:
					sq_pushstring( v, pRet.pValue.szString, -1 );
					break;

				case ST_INTEGER:
					sq_pushinteger( v, pRet.pValue.i );
					break;

				case ST_FLOAT:
					sq_pushfloat( v, pRet.pValue.f );
					break;

				case ST_BOOL:
					sq_pushbool( v, pRet.pValue.b );
					break;

				case ST_POINTER:
					sq_pushuserpointer( v, pRet.pValue.p );
					break;

				case ST_PLAYER:
					push_Player( (CPlayerPool*)pRet.pValue.p );
					break;

				case ST_VEHICLE:
					push_Vehicle( (CVehiclePool*)pRet.pValue.p );
					break;

				case ST_PICKUP:
					push_Pickup( (CPickupPool*)pRet.pValue.p );
					break;

				case ST_SOCKET:
					push_Socket( (CSquirrelSockets*)pRet.pValue.p );
					break;

				case ST_TIMER:
					push_Timer( (CSquirrelTimers*)pRet.pValue.p );
					break;

				case ST_NULL:
					sq_pushnull( v );
					break;

				default:
					sq_pushnull( v );
					break;
				}
				return 1;
			}
		}

		sq_pushbool(v, true );
		return 1;
	}

	sq_pushbool( v, false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQMiscFuncs::Halt )
{
	CSquirrel* pSquirrel = CSquirrelManager::Find( v );
	if ( pSquirrel ) 
	{
		pSquirrel->SetContinueProcessing( false );

		sq_pushbool( v, true );
	}
	else sq_pushbool( v, false );

	return 1;
}