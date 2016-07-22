/*
	SquirrelConstants.cpp
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

	Edited: 1/4/2009

	--------------------------------------------------------------------
*/


#include "SquirrelConstants.h"
#include "KeyBinds.h"

void CSquirrelConstants::RegisterConstants( HSQUIRRELVM v )
{
	// Part reasons
	BindConstant<int>( 0, "PARTREASON_TIMEOUT" );
	BindConstant<int>( 1, "PARTREASON_DISCONNECTED" );
	BindConstant<int>( 2, "PARTREASON_KICKED" );
	BindConstant<int>( 3, "PARTREASON_BANNED" );

	// Weapons
	BindConstant<int>( 0, "WEP_FIST" );
	BindConstant<int>( 1, "WEP_BRASSKNUCKLES" );
	BindConstant<int>( 2, "WEP_SCREWDRIVER" );
	BindConstant<int>( 3, "WEP_GOLFCLUB" );
	BindConstant<int>( 4, "WEP_NIGHTSTICK" );
	BindConstant<int>( 5, "WEP_KNIFE" );
	BindConstant<int>( 6, "WEP_BAT" );
	BindConstant<int>( 7, "WEP_HAMMER" );
	BindConstant<int>( 8, "WEP_CLEAVER" );
	BindConstant<int>( 9, "WEP_MACHETE" );
	BindConstant<int>( 10, "WEP_KATANA" );
	BindConstant<int>( 11, "WEP_CHAINSAW" );
	BindConstant<int>( 12, "WEP_GRENADE" );
	BindConstant<int>( 13, "WEP_REMOTE" );
	BindConstant<int>( 14, "WEP_TEARGAS" );
	BindConstant<int>( 15, "WEP_MOLOTOV" );
	BindConstant<int>( 16, "WEP_ROCKET" );
	BindConstant<int>( 17, "WEP_PISTOL" );
	BindConstant<int>( 18, "WEP_PYTHON" );
	BindConstant<int>( 19, "WEP_SHOTGUN" );
	BindConstant<int>( 20, "WEP_SPAZ" );
	BindConstant<int>( 21, "WEP_STUBBY" );
	BindConstant<int>( 22, "WEP_TEC9" );
	BindConstant<int>( 23, "WEP_UZI" );
	BindConstant<int>( 24, "WEP_INGRAM" );
	BindConstant<int>( 25, "WEP_MP5" );
	BindConstant<int>( 26, "WEP_M4" );
	BindConstant<int>( 27, "WEP_RUGER" );
	BindConstant<int>( 28, "WEP_SNIPER" );
	BindConstant<int>( 29, "WEP_LASERSNIPER" );
	BindConstant<int>( 30, "WEP_ROCKETLAUNCHER" );
	BindConstant<int>( 31, "WEP_FLAMETHROWER" );
	BindConstant<int>( 32, "WEP_M60" );
	BindConstant<int>( 33, "WEP_MINIGUN" );
	BindConstant<int>( 34, "WEP_BOMB" );
	BindConstant<int>( 35, "WEP_HELICANNON" );
	BindConstant<int>( 36, "WEP_CAMERA" );
	BindConstant<int>( 39, "WEP_VEHICLE" );
	BindConstant<int>( 41, "WEP_FALL" );
	BindConstant<int>( 42, "WEP_DRIVEBY" );
	BindConstant<int>( 43, "WEP_DROWNED" );
	BindConstant<int>( 51, "WEP_EXPLOSION" );

	// Vehicles
	BindConstant<int>( 130, "VEH_LANDSTALKER" );
	BindConstant<int>( 131, "VEH_IDAHO" );
	BindConstant<int>( 132, "VEH_STINGER" );
	BindConstant<int>( 133, "VEH_LINERUNNER" );
	BindConstant<int>( 134, "VEH_PERENNIAL" );
	BindConstant<int>( 135, "VEH_SENTINEL" );
	BindConstant<int>( 136, "VEH_RIO" );
	BindConstant<int>( 137, "VEH_FIRETRUCK" );
	BindConstant<int>( 138, "VEH_TRASHMASTER" );
	BindConstant<int>( 139, "VEH_STRETCH" );
	BindConstant<int>( 140, "VEH_MANANA" );
	BindConstant<int>( 141, "VEH_INFERNUS" );
	BindConstant<int>( 142, "VEH_VOODOO" );
	BindConstant<int>( 143, "VEH_PONY" );
	BindConstant<int>( 144, "VEH_MULE" );
	BindConstant<int>( 145, "VEH_CHEETAH" );
	BindConstant<int>( 146, "VEH_AMBULANCE" );
	BindConstant<int>( 147, "VEH_FBIWASHINGTON" );
	BindConstant<int>( 148, "VEH_MOONBEAM" );
	BindConstant<int>( 149, "VEH_ESPERANTO" );
	BindConstant<int>( 150, "VEH_TAXI" );
	BindConstant<int>( 151, "VEH_WASHINGTON" );
	BindConstant<int>( 152, "VEH_BOBCAT" );
	BindConstant<int>( 153, "VEH_MRWHOOPEE" );
	BindConstant<int>( 154, "VEH_BFINJECTION" );
	BindConstant<int>( 155, "VEH_HUNTER" );
	BindConstant<int>( 156, "VEH_POLICE" );
	BindConstant<int>( 157, "VEH_ENFORCER" );
	BindConstant<int>( 158, "VEH_SECURICAR" );
	BindConstant<int>( 159, "VEH_BANSHEE" );
	BindConstant<int>( 160, "VEH_PREDATOR" );
	BindConstant<int>( 161, "VEH_BUS" );
	BindConstant<int>( 162, "VEH_RHINO" );
	BindConstant<int>( 163, "VEH_BARRACKS" );
	BindConstant<int>( 164, "VEH_CUBANHERMES" );
	BindConstant<int>( 165, "VEH_HELICOPTER" );
	BindConstant<int>( 166, "VEH_ANGEL" );
	BindConstant<int>( 167, "VEH_COACH" );
	BindConstant<int>( 168, "VEH_CABBIE" );
	BindConstant<int>( 169, "VEH_STALLION" );
	BindConstant<int>( 170, "VEH_RUMPO" );
	BindConstant<int>( 171, "VEH_RCBANDIT" );
	BindConstant<int>( 172, "VEH_ROMEROSHEARSE" );
	BindConstant<int>( 173, "VEH_PACKER" );
	BindConstant<int>( 174, "VEH_SENTINELXS" );
	BindConstant<int>( 175, "VEH_ADMIRAL" );
	BindConstant<int>( 176, "VEH_SQUALO" );
	BindConstant<int>( 177, "VEH_SEASPARROW" );
	BindConstant<int>( 178, "VEH_PIZZABOY" );
	BindConstant<int>( 179, "VEH_GANGBURRITO" );
	BindConstant<int>( 180, "VEH_AIRTRAIN" );
	BindConstant<int>( 181, "VEH_DEADDODO" );
	BindConstant<int>( 182, "VEH_SPEEDER" );
	BindConstant<int>( 183, "VEH_REEFER" );
	BindConstant<int>( 184, "VEH_TROPIC" );
	BindConstant<int>( 185, "VEH_FLATBED" );
	BindConstant<int>( 186, "VEH_YANKEE" );
	BindConstant<int>( 187, "VEH_CADDY" );
	BindConstant<int>( 188, "VEH_ZEBRACAB" );
	BindConstant<int>( 189, "VEH_TOPFUN" );
	BindConstant<int>( 190, "VEH_SKIMMER" );
	BindConstant<int>( 191, "VEH_PCJ600" );
	BindConstant<int>( 192, "VEH_FAGGIO" );
	BindConstant<int>( 193, "VEH_FREEWAY" );
	BindConstant<int>( 194, "VEH_RCBARON" );
	BindConstant<int>( 195, "VEH_RCRAIDER" );
	BindConstant<int>( 196, "VEH_GLENDALE" );
	BindConstant<int>( 197, "VEH_OCEANIC" );
	BindConstant<int>( 198, "VEH_SANCHEZ" );
	BindConstant<int>( 199, "VEH_SPARROW" );
	BindConstant<int>( 200, "VEH_PATRIOT" );
	BindConstant<int>( 201, "VEH_LOVEFIST" );
	BindConstant<int>( 202, "VEH_COASTGUARD" );
	BindConstant<int>( 203, "VEH_DINGHY" );
	BindConstant<int>( 204, "VEH_HERMES" );
	BindConstant<int>( 205, "VEH_SABRE" );
	BindConstant<int>( 206, "VEH_SABRETURBO" );
	BindConstant<int>( 207, "VEH_PHOENIX" );
	BindConstant<int>( 208, "VEH_WALTON" );
	BindConstant<int>( 209, "VEH_REGINA" );
	BindConstant<int>( 210, "VEH_COMET" );
	BindConstant<int>( 211, "VEH_DELUXO" );
	BindConstant<int>( 212, "VEH_BURRITO" );
	BindConstant<int>( 213, "VEH_SPANDEXPRESS" );
	BindConstant<int>( 214, "VEH_MARQUIS" );
	BindConstant<int>( 215, "VEH_BAGGAGEHANDLER" );
	BindConstant<int>( 216, "VEH_KAUFMANCAB" );
	BindConstant<int>( 217, "VEH_MAVERICK" );
	BindConstant<int>( 218, "VEH_VCNMAVERICK" );
	BindConstant<int>( 219, "VEH_RANCHER" );
	BindConstant<int>( 220, "VEH_FBIRANCHER" );
	BindConstant<int>( 221, "VEH_VIRGO" );
	BindConstant<int>( 222, "VEH_GREENWOOD" );
	BindConstant<int>( 223, "VEH_CUBANJETMAX" );
	BindConstant<int>( 224, "VEH_HOTRING1" );
	BindConstant<int>( 225, "VEH_SANDKING" );
	BindConstant<int>( 226, "VEH_BLISTACOMPACT" );
	BindConstant<int>( 227, "VEH_POLICEMAVERICK" );
	BindConstant<int>( 228, "VEH_BOXVILLE" );
	BindConstant<int>( 229, "VEH_BENSON" );
	BindConstant<int>( 230, "VEH_MESAGRANDE" );
	BindConstant<int>( 231, "VEH_RCGOBLIN" );
	BindConstant<int>( 232, "VEH_HOTRING2" );
	BindConstant<int>( 233, "VEH_HOTRING3" );
	BindConstant<int>( 234, "VEH_BLOODRING1" );
	BindConstant<int>( 235, "VEH_BLOODRING2" );
	BindConstant<int>( 236, "VEH_VICECHEETAH" );

	// Bindable keys
	BindConstant<int>( ONFOOT_AIM,			"KEY_ONFOOT_AIM" );
	BindConstant<int>( ONFOOT_WEPNEXT,		"KEY_ONFOOT_WEPNEXT" );
	BindConstant<int>( ONFOOT_WEPPREV,		"KEY_ONFOOT_WEPPREV" );
	BindConstant<int>( ONFOOT_CROUCH,		"KEY_ONFOOT_CROUCH" );
	BindConstant<int>( ONFOOT_SPRINT,		"KEY_ONFOOT_SPRINT" );
	BindConstant<int>( ONFOOT_JUMP,			"KEY_ONFOOT_JUMP" );
	BindConstant<int>( ONFOOT_RIGHT,		"KEY_ONFOOT_RIGHT" );
	BindConstant<int>( ONFOOT_LEFT,			"KEY_ONFOOT_LEFT" );
	BindConstant<int>( ONFOOT_BACKWARD,		"KEY_ONFOOT_BACKWARD" );
	BindConstant<int>( ONFOOT_FORWARD,		"KEY_ONFOOT_FORWARD" );
	BindConstant<int>( AIM_FIRE,			"KEY_ONFOOT_FIRE" );
	BindConstant<int>( INVEHICLE_HANDBRAKE,	"KEY_VEHICLE_HANDBRAKE" );
	BindConstant<int>( INVEHICLE_LOOKLEFT,	"KEY_VEHICLE_LOOKLEFT" );
	BindConstant<int>( INVEHICLE_LOOKRIGHT,	"KEY_VEHICLE_LOOKRIGHT" );
	BindConstant<int>( INVEHICLE_LEANDOWN,	"KEY_VEHICLE_LEANDOWN" );
	BindConstant<int>( INVEHICLE_LEANUP,	"KEY_VEHICLE_LEANUP" );
	BindConstant<int>( INVEHICLE_HORN,		"KEY_VEHICLE_HORN" );
	BindConstant<int>( INVEHICLE_FIRE,		"KEY_VEHICLE_FIRE" );
	BindConstant<int>( INVEHICLE_THROTTLE,	"KEY_VEHICLE_THROTTLE" );
	BindConstant<int>( INVEHICLE_BRAKE,		"KEY_VEHICLE_BRAKE" );
	BindConstant<int>( INVEHICLE_RIGHT,		"KEY_VEHICLE_RIGHT" );
	BindConstant<int>( INVEHICLE_LEFT,		"KEY_VEHICLE_LEFT" );

}
