/*
	SquirrelFuncs.cpp
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

	File Author: VRocker, Juppi

	--------------------------------------------------------------------
*/

#include "SquirrelFuncs.h"
// We include the functions header files below
#include "Functions/SQMiscFuncs.h"
#include "Functions/SQPlayerFuncs.h"
#include "Functions/SQVehicleFuncs.h"
#include "Functions/SQPickupFuncs.h"
#include "Functions/SQGameFuncs.h"
#include "Functions/SQKeyBindFuncs.h"
#include "Functions/SQTimerFuncs.h"
#include "Functions/SQIniFuncs.h"
#include "Functions/SQSocketFuncs.h"
#include "Functions/SQXmlFuncs.h"
#include "Functions/SQLiteFuncs.h"
#include "Pools.h"

using namespace SqPlus;

_IMPL_NATIVE_CONSTRUCTION_NODELETE( Player, CPlayerPool );
_IMPL_NATIVE_CONSTRUCTION_NODELETE( Vehicle, CVehiclePool );
_IMPL_NATIVE_CONSTRUCTION_NODELETE( Pickup, CPickupPool );
_IMPL_NATIVE_CONSTRUCTION_NODELETE( Timer, CSquirrelTimers );
_IMPL_NATIVE_CONSTRUCTION_NODELETE( Socket, CSquirrelSockets );

int constructVector( float fX, float fY, float fZ, HSQUIRRELVM v ) 
{
  return PostConstruct< Vector >( v, new Vector( fX, fY, fZ ), ReleaseClassPtr< Vector >::release );
}

SQInteger CSquirrelFuncs::RegisterGlobalFunc( HSQUIRRELVM v, SQFUNCTION f, const char *fname, unsigned char ucParams, char* szParams )
{
        sq_pushroottable(v);
        sq_pushstring(v,fname,-1);
        sq_newclosure(v,f,0); //create a new function
		if ( ucParams > 0 ) 
		{
			ucParams += 1; // This is to compensate for the root table
			char szNewParams[ 32 ] = { NULL };
			strncpy( szNewParams, "t", 1 );
			strncat( szNewParams, szParams, 32 );
			sq_setparamscheck( v, ucParams, szNewParams ); // Do the params stuff
		}
        sq_createslot(v,-3);
        sq_pop(v,1); //pops the root table
        return 0;
}

void CSquirrelFuncs::RegisterFuncs( HSQUIRRELVM v )
{
	// This is where we register functions to the squirrel VM
	// Params are: SquirrelVM, Class::Function, Name inside squirrel

	// Misc funcs
	RegisterGlobalFunc( v, CSQMiscFuncs::Print, "Print", 1, "s" );

	RegisterGlobalFunc( v, CSQMiscFuncs::Message, "Message", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::MessagePlayer, "MessagePlayer", 2, "s." );
	RegisterGlobalFunc( v, CSQMiscFuncs::MessageAllExcept, "MessageAllExcept", 2, "s." );
	RegisterGlobalFunc( v, CSQMiscFuncs::PrivMessage, "PrivMessage", 2, "s." );
	RegisterGlobalFunc( v, CSQMiscFuncs::Announce, "Announce", 2, "s." );
	RegisterGlobalFunc( v, CSQMiscFuncs::AnnounceAll, "AnnounceAll", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SendPlayerMessage, "SendPlayerMessage", 3, "..s" );

	RegisterGlobalFunc( v, CSQMiscFuncs::GetTickCount, "GetTickCount" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetTime, "GetTime" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetFullTime, "GetFullTime" );
	RegisterGlobalFunc( v, CSQMiscFuncs::IsNum, "IsNum", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::InPoly, "InPoly" );
	RegisterGlobalFunc( v, CSQMiscFuncs::CallFunc, "CallFunc" );
	RegisterGlobalFunc( v, CSQMiscFuncs::Halt, "Halt" );

	RegisterGlobalFunc( v, CSQMiscFuncs::KickPlayer, "KickPlayer", 1, "." );
	RegisterGlobalFunc( v, CSQMiscFuncs::BanPlayer, "BanPlayer", 1, "." );
	RegisterGlobalFunc( v, CSQMiscFuncs::BanIP, "BanIP", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::UnbanIP, "UnbanIP", 1, "s" );

	RegisterGlobalFunc( v, CSQMiscFuncs::GetServerName, "GetServerName" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SetServerName, "SetServerName", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetGamemodeName, "GetGamemodeName" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SetGamemodeName, "SetGamemodeName", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetMapName, "GetMapName" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SetMapName, "SetMapName", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetPassword, "GetPassword" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SetPassword, "SetPassword", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetMaxPlayers, "GetMaxPlayers" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SetMaxPlayers, "SetMaxPlayers", 1, "i" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetLocked, "GetServerLocked" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SetLocked, "SetServerLocked", 1, "b" );

	RegisterGlobalFunc( v, CSQMiscFuncs::GetPlayers, "GetPlayers" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetDeathmatchScoreboard, "GetDeathmatchScoreboard" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SetDeathmatchScoreboard, "SetDeathmatchScoreboard", 1, "b" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetDrivebyEnabled, "GetDrivebyEnabled" );
	RegisterGlobalFunc( v, CSQMiscFuncs::SetDrivebyEnabled, "SetDrivebyEnabled", 1, "b" );

	RegisterGlobalFunc( v, CSQMiscFuncs::GetWeaponName, "GetWeaponName", 1, "i" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetWeaponID, "GetWeaponID", 1, "s" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetDistrictName, "GetDistrictName", 2, "ff" );
	RegisterGlobalFunc( v, CSQMiscFuncs::GetSkinName, "GetSkinName", 1, "i" );


	// Ini Funcs
	RegisterGlobalFunc( v, CSQIniFuncs::ReadString, "ReadIniString", 3, "sss" );
	RegisterGlobalFunc( v, CSQIniFuncs::ReadInteger, "ReadIniInteger", 3, "sss" );
	RegisterGlobalFunc( v, CSQIniFuncs::ReadNumber, "ReadIniNumber", 3, "sss" );
	RegisterGlobalFunc( v, CSQIniFuncs::ReadBool, "ReadIniBool", 3, "sss" );
	RegisterGlobalFunc( v, CSQIniFuncs::WriteString, "WriteIniString", 4, "ssss" );
	RegisterGlobalFunc( v, CSQIniFuncs::WriteInteger, "WriteIniInteger", 4, "sssi" );
	RegisterGlobalFunc( v, CSQIniFuncs::WriteNumber, "WriteIniNumber", 4, "sssf" );
	RegisterGlobalFunc( v, CSQIniFuncs::WriteBool, "WriteIniBool", 4, "sssb" );
	RegisterGlobalFunc( v, CSQIniFuncs::RemoveValue, "RemoveIniValue", 3, "sss" );
	RegisterGlobalFunc( v, CSQIniFuncs::CountSection, "CountIniSection", 2, "ss" );
	RegisterGlobalFunc( v, CSQIniFuncs::DeleteSection, "DeleteIniSection", 2, "ss" );
	RegisterGlobalFunc( v, CSQIniFuncs::Clear, "ClearIni", 1, "s" );


	// Timer Funcs
	RegisterGlobalFunc( v, CSQTimerFuncs::New, "NewTimer" );/*
	RegisterGlobalFunc( v, CSQTimerFuncs::Delete, "DeleteTimer", 1, "p" );
	RegisterGlobalFunc( v, CSQTimerFuncs::Start, "StartTimer", 1, "p" );
	RegisterGlobalFunc( v, CSQTimerFuncs::Stop, "StopTimer", 1, "p" );*/


	// Socket Funcs
	RegisterGlobalFunc( v, CSQSocketFuncs::New, "NewSocket", 0 );
	/*RegisterGlobalFunc( v, CSQSocketFuncs::Delete, "DeleteSocket", 1, "p" );
	RegisterGlobalFunc( v, CSQSocketFuncs::Connect, "ConnectSocket", 3, "psi" );
	RegisterGlobalFunc( v, CSQSocketFuncs::Start, "StartSocket", 3, "pii" );
	RegisterGlobalFunc( v, CSQSocketFuncs::Stop, "StopSocket", 1, "p" );
	RegisterGlobalFunc( v, CSQSocketFuncs::Send, "SendSocket" );
	RegisterGlobalFunc( v, CSQSocketFuncs::SetNewConnFunc, "SetSocketNewConnFunc", 2, "ps" );
	RegisterGlobalFunc( v, CSQSocketFuncs::SetLostConnFunc, "SetSocketLostConnFunc", 2, "ps" );*/


	// Player funcs
	RegisterGlobalFunc( v, CSQPlayerFuncs::FindPlayer, "FindPlayer", 1, "i|s" );
	/*RegisterGlobalFunc( v, CSQPlayerFuncs::GetID, "GetPlayerID", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetNick, "GetPlayerName", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetIP, "GetPlayerIP", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetPing, "GetPlayerPing", 1, "p" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::GetHealth, "GetPlayerHealth", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetHealth, "SetPlayerHealth", 2, "pn" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetArmour, "GetPlayerArmour", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetArmour, "SetPlayerArmour", 2, "pn" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::GetScore, "GetPlayerScore", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetScore, "SetPlayerScore", 2, "pi" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::IncScore, "IncPlayerScore", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::DecScore, "DecPlayerScore", 1, "p" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::GetClass, "GetPlayerClass", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetTeam, "GetPlayerTeam", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetSkin, "GetPlayerSkin", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetAnim, "SetPlayerAnim", 2, "pi" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetWeapon, "GetPlayerWeapon", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetWeapon, "SetPlayerWeapon", 3, "pii" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetControls, "GetPlayerControls", 1, "p" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::GetCash, "GetPlayerCash", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetCash, "SetPlayerCash", 2, "pi" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GiveCash, "GivePlayerCash", 2, "pi" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::TakeCash, "TakePlayerCash", 2, "pi" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::GetVehicle, "GetPlayerVehicle", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::PutInVehicle, "PutPlayerInVehicle", 2, "pp" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::IsPassenger, "IsPlayerPassenger", 1, "p" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::IsAdmin, "IsPlayerRconAdmin", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::IsSpawned, "IsPlayerSpawned", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::IsFrozen, "IsPlayerFrozen", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetSpectating, "GetPlayerSpectating", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::ToggleControls, "TogglePlayerControls", 2, "pb" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::IsMuted, "IsPlayerMuted", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::Mute, "MutePlayer", 2, "pb" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::IsIgnoredBy, "IsPlayerIgnoredBy", 2, "pp" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::Ignore, "IgnorePlayer", 2, "p." );
	RegisterGlobalFunc( v, CSQPlayerFuncs::Unignore, "UnignorePlayer", 2, "p." );

	RegisterGlobalFunc( v, CSQPlayerFuncs::IsSyncBlocked, "IsPlayerSyncBlocked", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetSyncBlocked, "SetPlayerSyncBlocked", 2, "pb" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::IsSyncBlockedTo, "IsPlayerSyncBlockedTo", 1, "pp" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetSyncBlockedTo, "SetPlayerSyncBlockedTo", 2, "ppb" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::GetPos, "GetPlayerPos", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetPos, "SetPlayerPos" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetInterior, "SetPlayerInterior", 2, "pi" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::GetAngle, "GetPlayerAngle", 1, "p" );

	RegisterGlobalFunc( v, CSQPlayerFuncs::SetMarker, "SetPlayerMarker", 2, "pi" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::RemoveMarker, "RemovePlayerMarker", 1, "p" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::SetLocalMarker, "SetPlayerLocalMarker", 3, "pip" );
	RegisterGlobalFunc( v, CSQPlayerFuncs::RemoveLocalMarker, "RemovePlayerLocalMarker", 2, "pp" );
*/

	// Vehicle funcs
	RegisterGlobalFunc( v, CSQVehicleFuncs::CreateStaticVehicle, "CreateStaticVehicle", 5, "i.fii" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::FindVehicle, "FindVehicle", 1, "i" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::GetCount, "GetVehicleCount" );
	
	/*RegisterGlobalFunc( v, CSQVehicleFuncs::GetID, "GetVehicleID", 1, "p" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::GetModel, "GetVehicleModel", 1, "p" );

	RegisterGlobalFunc( v, CSQVehicleFuncs::GetHealth, "GetVehicleHealth", 1, "p" );
	//RegisterGlobalFunc( v, CSQVehicleFuncs::SetHealth, "SetVehicleHealth", 2, "pf" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::Blow, "BlowVehicle", 1, "p" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::SetIndestructible, "SetVehicleIndestructible", 1, "p" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::GetColour1, "GetVehicleColour1", 1, "p" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::GetColour2, "GetVehicleColour2", 1, "p" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::GetDriver, "GetVehicleDriver", 1, "p" );

	RegisterGlobalFunc( v, CSQVehicleFuncs::IsLocked, "IsVehicleLocked", 1, "p" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::SetLocked, "SetVehicleLocked", 2, "pb" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::KillEngine, "KillVehicleEngine", 1, "p" );

	RegisterGlobalFunc( v, CSQVehicleFuncs::GetPos, "GetVehiclePos", 1, "p" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::SetPos, "SetVehiclePos", 2, "p." );

	RegisterGlobalFunc( v, CSQVehicleFuncs::Respawn, "RespawnVehicle", 1, "p" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::SetSpawnInfo, "SetVehicleSpawnInfo", 6, "pi.fii" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::MoveTo, "MoveVehicleTo", 3, "p.f" );*/
	
	RegisterGlobalFunc( v, CSQVehicleFuncs::GetNameFromModel, "GetVehicleNameFromModel", 1, "i" );
	RegisterGlobalFunc( v, CSQVehicleFuncs::GetModelFromName, "GetVehicleModelFromName", 1, "s" );


	// Game funcs
	RegisterGlobalFunc( v, CSQGameFuncs::GetHour, "GetHour" );
	RegisterGlobalFunc( v, CSQGameFuncs::SetHour, "SetHour", 1, "i" );
	RegisterGlobalFunc( v, CSQGameFuncs::GetMinute, "GetMinute" );
	RegisterGlobalFunc( v, CSQGameFuncs::SetMinute, "SetMinute", 1, "i" );
	RegisterGlobalFunc( v, CSQGameFuncs::SetTime, "SetTime", 2, "ii" );
	RegisterGlobalFunc( v, CSQGameFuncs::GetTimeRate, "GetTimeRate" );
	RegisterGlobalFunc( v, CSQGameFuncs::SetTimeRate, "SetTimeRate", 1, "i" );

	RegisterGlobalFunc( v, CSQGameFuncs::GetWeather, "GetWeather" );
	RegisterGlobalFunc( v, CSQGameFuncs::SetWeather, "SetWeather", 1, "i" );
	RegisterGlobalFunc( v, CSQGameFuncs::GetWeatherRate, "GetWeatherRate" );
	RegisterGlobalFunc( v, CSQGameFuncs::SetWeatherRate, "SetWeatherRate", 1, "i" );
	RegisterGlobalFunc( v, CSQGameFuncs::GetWeatherLock, "GetWeatherLock" );
	RegisterGlobalFunc( v, CSQGameFuncs::SetWeatherLock, "SetWeatherLock", 1, "b" );

	RegisterGlobalFunc( v, CSQGameFuncs::SetAmmuWeapon, "SetAmmuWeapon", 3, "iii" );
	RegisterGlobalFunc( v, CSQGameFuncs::IsAmmuWeaponEnabled, "IsAmmuWeaponEnabled", 2, "ii" );
	RegisterGlobalFunc( v, CSQGameFuncs::SetAmmuWeaponEnabled, "SetAmmuWeaponEnabled", 3, "iib" );

	RegisterGlobalFunc( v, CSQGameFuncs::CreateMarker, "CreateMarker", 2, "i." );
	RegisterGlobalFunc( v, CSQGameFuncs::CreateMarkerForPlayer, "CreateMarkerForPlayer", 3, ".i." );


	// Pickup funcs
	RegisterGlobalFunc( v, CSQPickupFuncs::Create, "CreatePickup" );
	//RegisterGlobalFunc( v, CSQPickupFuncs::Remove, "RemovePickup" );

	RegisterGlobalFunc( v, CSQPickupFuncs::FindPickup, "FindPickup", 1, "i" );
	RegisterGlobalFunc( v, CSQPickupFuncs::GetCount, "GetPickupCount" );
	/*RegisterGlobalFunc( v, CSQPickupFuncs::GetID, "GetPickupID", 1, "p" );
	RegisterGlobalFunc( v, CSQPickupFuncs::GetModel, "GetPickupModel", 1, "p" );
	RegisterGlobalFunc( v, CSQPickupFuncs::SetModel, "SetPickupModel", 2, "pi" );
	RegisterGlobalFunc( v, CSQPickupFuncs::IsSpawned, "IsPickupSpawned", 1, "p" );

	RegisterGlobalFunc( v, CSQPickupFuncs::SetOnceOnly, "SetPickupOnceOnly", 1, "p" );
	RegisterGlobalFunc( v, CSQPickupFuncs::SetRespawnTime, "SetPickupRespawnTime", 2, "pi" );*/
	RegisterGlobalFunc( v, CSQPickupFuncs::GetGlobalRespawnTime, "GetGlobalPickupRespawnTime" );
	RegisterGlobalFunc( v, CSQPickupFuncs::SetGlobalRespawnTime, "SetGlobalPickupRespawnTime", 1, "i" );

	/*RegisterGlobalFunc( v, CSQPickupFuncs::GetPos, "GetPickupPos", 1, "p" );
	RegisterGlobalFunc( v, CSQPickupFuncs::SetPos, "SetPickupPos", 2, "p." );
	RegisterGlobalFunc( v, CSQPickupFuncs::Respawn, "RespawnPickup", 1, "p" );
	RegisterGlobalFunc( v, CSQPickupFuncs::SpawnForPlayer, "SpawnPickupForPlayer", 2, "pp" );
	RegisterGlobalFunc( v, CSQPickupFuncs::HideForPlayer, "HidePickupForPlayer", 1, "pp" );*/


	// Bind funcs
	RegisterGlobalFunc( v, CSQKeyBindFuncs::BindKey, "BindKey" );
	RegisterGlobalFunc( v, CSQKeyBindFuncs::RemoveBind, "UnbindKey", 1, "i" );


	// XML funcs
	/*RegisterGlobalFunc( v, CSQXmlFuncs::CreateDoc, "CreateXmlDocument", 1, "s" );
	RegisterGlobalFunc( v, CSQXmlFuncs::LoadDoc, "LoadXmlDocument", 1, "s" );
	RegisterGlobalFunc( v, CSQXmlFuncs::SaveDoc, "SaveXmlDocument", 2, "ps" );

	RegisterGlobalFunc( v, CSQXmlFuncs::CreateElement, "CreateElement", 2, "ps" );
	RegisterGlobalFunc( v, CSQXmlFuncs::RemoveElement, "RemoveElement", 1, "p" );
	RegisterGlobalFunc( v, CSQXmlFuncs::GetElement, "GetElement", 2, "ps" );
	RegisterGlobalFunc( v, CSQXmlFuncs::GetParentElement, "GetParentElement", 1, "p" );
	RegisterGlobalFunc( v, CSQXmlFuncs::GetNextSiblingElement, "GetNextSiblingElement", 1, "p" );
	RegisterGlobalFunc( v, CSQXmlFuncs::GetPrevSiblingElement, "GetPrevSiblingElement", 1, "p" );

	RegisterGlobalFunc( v, CSQXmlFuncs::GetElementAttribute, "GetElementAttribute", 2, "ps" );
	RegisterGlobalFunc( v, CSQXmlFuncs::SetElementAttribute, "SetElementAttribute", 3, "pss" );

	RegisterGlobalFunc( v, CSQXmlFuncs::GetElementName, "GetElementName", 1, "p" );
	RegisterGlobalFunc( v, CSQXmlFuncs::GetElementContent, "GetElementContent", 1, "p" );
	RegisterGlobalFunc( v, CSQXmlFuncs::SetElementContent, "SetElementContent", 2, "ps" );*/


	// SQLite funcs
	RegisterGlobalFunc( v, CSQLiteFuncs::Connect, "ConnectSQL", 1, "s" );
	RegisterGlobalFunc( v, CSQLiteFuncs::Close, "DisconnectSQL", 1, "p" );
	RegisterGlobalFunc( v, CSQLiteFuncs::Query, "QuerySQL", 2, "ps" );
	RegisterGlobalFunc( v, CSQLiteFuncs::GetNextRow, "GetSQLNextRow", 1, "p" );
	RegisterGlobalFunc( v, CSQLiteFuncs::GetRowColumns, "GetSQLColumnCount", 1, "p" );
	RegisterGlobalFunc( v, CSQLiteFuncs::GetColumnResult, "GetSQLColumnData", 2, "pi" );


	ClassBinds();
}


void CSquirrelFuncs::ClassBinds( void )
{
	/*SQClassDef< Vector >( _SC( "Vector" ) ).
		staticFunc(constructVector,_SC("constructor")).
		var(&Vector::x,_SC("x")).
		var(&Vector::y,_SC("y")).
		var(&Vector::z,_SC("z"));*/

	_INIT_CLASS( Vector );

	_INIT_CLASS( Player );
	_INIT_CLASS( Vehicle );
	_INIT_CLASS( Pickup );
	_INIT_CLASS( Timer );
	_INIT_CLASS( Socket );
	_INIT_CLASS( XmlNode );
	_INIT_CLASS( XmlDocument );
	_INIT_CLASS( XmlElement );

}