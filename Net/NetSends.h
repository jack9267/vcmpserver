#ifndef _NETSENDS_H
#define _NETSENDS_H

#include "Net.h"

class CNetSends
{
public:
	static void						InitGame						( unsigned char uc );
	static void						PlayerJoin						( unsigned char uc );
	static void						PlayerPart						( unsigned char uc, unsigned char ucReason );

	static void						Chat							( unsigned char uc, char* szText );
	static void						ChatToPlayer					( unsigned char ucPlayerTo, unsigned char ucPlayer, char* szText );
	static void						Action							( unsigned char uc, char* szText );
	static void						PM								( unsigned char uc, unsigned char ucPlayerTo, char* szText );
	static void						TeamChat						( unsigned char uc, unsigned char ucTeam, char* szText );

	static void						Spawn							( unsigned char uc );
	static void						SpawnForWorld					( unsigned char uc );
	static void						SpawnForPlayer					( unsigned char uc, unsigned char ucPlayer );

	static void						SpawnVehicleForPlayer			( unsigned char uc, unsigned char ucVehicle );
	static void						SpawnPickupForPlayer			( unsigned char uc, unsigned int uiPickup, unsigned char ucOwner = 255 );

	static void						Death							( unsigned char uc, unsigned char ucKiller, unsigned char ucReason );

	static void						SetHour							( unsigned char ucHour );
	static void						SetMinute						( unsigned char ucMinute );
	static void						SetWeather						( unsigned char ucWeather );

	static void						Set								( char* sz );

	static void						EnterVehicle					( unsigned char uc, unsigned char ucVehicle );
	static void						ExitVehicle						( unsigned char uc, unsigned char ucVehicle );

	static void						UpdateScoreAndPing				( unsigned char uc );

	static void						SetAdmin						( unsigned char uc );
	static void						NotAdmin						( unsigned char uc );

	static void						PickedUpPickup					( unsigned char uc, unsigned int ui );
	static void						DeletePickup					( unsigned char uc, unsigned int ui );
	static void						DeletePickupForPlayer			( unsigned char uc, unsigned int ui );
	static void						DeletePickupForAll				( unsigned int uiPickup );
	static void						DoAnim							( unsigned char uc, unsigned char ucAnim );
	static void						SpecOn							( unsigned char uc );

	static void						AdminChat						( char* sz );
	static void						AdminChatToPlayer				( PlayerID playerID, char* sz );
	static void						AdminChatAllExcept				( PlayerID playerID, char* sz );
	static void						AdminMsg						( unsigned char uc, char* sz );

	static void						RequestAmmu						( unsigned char uc, unsigned char ucAmmu );
	static void						RequestWep						( unsigned char uc, unsigned char ucAmmu, unsigned int iWeapon );

	static void						PlayerSync						( unsigned char uc, ONFOOT_SYNC_DATA ofSync );
	static void						AimSync							( unsigned char uc, ONFOOT_SYNC_DATA ofSync, AIM_SYNC_DATA aimSync );
	static void						VehicleSync						( unsigned char uc, unsigned char ucVehicle, INCAR_SYNC_DATA icSync );

	static void						PassengerSync					( unsigned char uc, unsigned char ucVehicle, unsigned int uiPassengerSeat );

	// Some rcon stuff
	static void						SetPlayerPos					( unsigned char uc1, unsigned char uc2 );
	static void						SetPlayerHealth					( unsigned char uc );
	static void						SetPlayerArmour					( unsigned char uc );
	static void						SetPlayerFrozen					( unsigned char uc );

	static void						SetPlayerWeapon					( unsigned char uc, unsigned char ucWeapon, unsigned int uiAmmo );
	static void						SetPlayerCash					( unsigned char uc );
	static void						RemovePlayerMarker				( unsigned char uc );
	static void						RemovePlayerLocalMarker			( unsigned char uc, unsigned char uc2 );
	static void						PutPlayerInVehicle				( unsigned char uc1, unsigned char uc2 );
	static void						SetPlayerMarker					( unsigned char uc1, unsigned char uc2 );
	static void						SetPlayerLocalMarker			( unsigned char uc, unsigned char uc2, unsigned char uc3 );

	static void						Announce						( unsigned char uc, char* sz );
	static void						SetMarker						( unsigned char uc, float fX, float fY, float fZ );
	static void						SetMarkerForPlayer				( unsigned char uc, unsigned char uc2, float fX, float fY, float fZ );

	static void						SetVehicleDoorStatus			( unsigned char uc );
	static void						SetVehiclePosition				( unsigned char uc );
	static void						KillVehicleEngine				( unsigned char uc );
	static void						SetVehicleHealth				( unsigned char uc );

	static void						KickPlayer						( unsigned char uc );
	static void						BanPlayer						( unsigned char uc );
};

#endif
