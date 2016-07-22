/*
	ConfigLoader.cpp
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

#include "ConfigLoader.h"
#include "Console.h"
#include "Pools.h"
#include "Scripting.h"
#include "Utils.h"
#include "BanList.h"
#include "CarCols.h"

char* CConfigLoader::m_szConfig = NULL;
TiXmlDocument* CConfigLoader::m_xmlDoc = NULL;

unsigned char CConfigLoader::m_ucMaxPlayers = 32;
unsigned short CConfigLoader::m_usListenPort = 5192;
char CConfigLoader::m_szServerName[ MAX_SERVNAME_LEN ] = { "Unknown Server" };
unsigned char CConfigLoader::m_ucMap = 1;
char CConfigLoader::m_szPassword[ MAX_PASS_LEN ] = { "none" };
char CConfigLoader::m_szAdminPassword[ MAX_PASS_LEN ] = { NULL };
char CConfigLoader::m_szGamemode[ MAX_GAMEMODE_LEN ] = { "Default" };
bool CConfigLoader::m_bFriendlyFire = false;
unsigned char CConfigLoader::m_ucShowOnRadar = 1;
unsigned short CConfigLoader::m_usRconPort = 5315;
unsigned short CConfigLoader::m_usRconMaxUsers = 5;
unsigned char CConfigLoader::m_ucWeather = 4;
unsigned char CConfigLoader::m_ucHour = 12;
unsigned char CConfigLoader::m_ucMinute = 30;
unsigned char CConfigLoader::m_ucRate = 4;
unsigned int CConfigLoader::m_uiWeatherRate = 300000;
unsigned int CConfigLoader::m_uiPickupRespawnTime = 60000;
bool CConfigLoader::m_bOutputConsole = true;
bool CConfigLoader::m_bScore = true;
bool CConfigLoader::m_bDriveby = true;
VECTOR CConfigLoader::m_vecInitPlayerPos;
VECTOR CConfigLoader::m_vecInitCameraPos;
VECTOR CConfigLoader::m_vecInitCameraLook;
float CConfigLoader::m_WorldBounds[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };

void CConfigLoader::Initialize( const char* sz )
{
	m_szConfig = NULL;
	if ( sz )
	{
		m_szConfig = new char [ strlen ( sz ) + 1 ];
		strcpy ( m_szConfig, sz );

		m_xmlDoc = new TiXmlDocument( sz );

		if (!m_xmlDoc->LoadFile())
		{
			m_xmlDoc->Clear();

			delete m_xmlDoc;
			m_xmlDoc = NULL;
		}
	}
}

void CConfigLoader::Destroy( void )
{
	m_szConfig = NULL;

	if ( m_xmlDoc )
	{
		m_xmlDoc->Clear();
		delete m_xmlDoc;
		m_xmlDoc = NULL;
	}
}

void CConfigLoader::LoadBans( void )
{
	FILE* m_BanDoc = fopen( "bans.lst", "r" );

	if ( m_BanDoc )
	{
		char szLine[255] = { 0 };

		while ( fgets( szLine, 255, m_BanDoc ) )
		{
			if ( ( strlen( szLine ) > 0 ) && ( szLine[ 0 ] != '\n' ) ) CBanList::Parse( szLine );
		}

		fclose( m_BanDoc );
	}
}

void CConfigLoader::LoadSettings(void)
{
	if ((m_szConfig) && (m_xmlDoc))
	{
		TiXmlElement* xmlRoot = m_xmlDoc->FirstChildElement("Settings");

		if (xmlRoot)
		{
			TiXmlElement* xmlMaxPlayers = xmlRoot->FirstChildElement("MaxPlayers");

			if ((xmlMaxPlayers) && (xmlMaxPlayers->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlMaxPlayers->GetText() ) ) m_ucMaxPlayers = static_cast< unsigned char >( atoi( xmlMaxPlayers->GetText() ) );
				if ( m_ucMaxPlayers > 50 ) m_ucMaxPlayers = 50;
			}
			else m_ucMaxPlayers = 32;

			TiXmlElement* xmlListenPort = xmlRoot->FirstChildElement("ListenPort");

			if ((xmlListenPort) && (xmlListenPort->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlListenPort->GetText() ) ) m_usListenPort = static_cast< unsigned short >( atoi( xmlListenPort->GetText() ) );
			}
			else m_usListenPort = 5192;

			TiXmlElement* xmlServerName = xmlRoot->FirstChildElement("ServerName");

			if ((xmlServerName) && (xmlServerName->GetText()))
			{
				strncpy( m_szServerName, xmlServerName->GetText(), MAX_SERVNAME_LEN );
			}
			else
			{
				strcpy( m_szServerName, "Unknown Server" );
				CConsole::PrintC("Error", COLOUR_RED, "'ServerName' not found in %s", m_szConfig);
			}

			TiXmlElement* xmlMap = xmlRoot->FirstChildElement("Map");

			if ((xmlMap) && (xmlMap->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlMap->GetText() ) ) m_ucMap = static_cast< unsigned char >( atoi( xmlMap->GetText() ) );
			}
			else m_ucMap = 1;

			TiXmlElement* xmlPassword = xmlRoot->FirstChildElement("Password");

			if ((xmlPassword) && (xmlPassword->GetText()))
			{
				strncpy( m_szPassword, xmlPassword->GetText(), MAX_PASS_LEN );
			}
			else strcpy(m_szPassword,"none");

			TiXmlElement* xmlRconPassword = xmlRoot->FirstChildElement("AdminPassword");

			if ((xmlRconPassword) && (xmlRconPassword->GetText()))
			{
				strncpy( m_szAdminPassword, xmlRconPassword->GetText(), MAX_PASS_LEN );
			}
			else
			{
				CUtils::RandomString( m_szAdminPassword );
				CConsole::PrintC("Error", COLOUR_RED, "'AdminPassword' not found in %s - Setting to %s", m_szConfig, m_szAdminPassword);
			}

			TiXmlElement* xmlGamemode = xmlRoot->FirstChildElement("GameModeName");

			if ((xmlGamemode) && (xmlGamemode->GetText()))
			{
				strncpy( m_szGamemode, xmlGamemode->GetText(), MAX_GAMEMODE_LEN );
			}
			else
			{
				strcpy( m_szGamemode, "Default" );
				CConsole::PrintC("Error", COLOUR_RED, "'GameModeName' not found in %s", m_szConfig);
			}

			TiXmlElement* xmlFriendlyFire = xmlRoot->FirstChildElement("FriendlyFire");

			if ((xmlFriendlyFire) && (xmlFriendlyFire->GetText()))
			{
				char szText[ 8 ] = { NULL };
				strncpy( szText, xmlFriendlyFire->GetText(), 8 );
				if ((!strcmp(szText, "1")) || (!strcmp(szText, "yes")) || (!strcmp(szText, "true")) || (!strcmp( szText, "on"))) m_bFriendlyFire = true;
				else m_bFriendlyFire = false;
			}

			TiXmlElement* xmlShowOnRadar = xmlRoot->FirstChildElement("ShowOnRadar");

			if ((xmlShowOnRadar) && (xmlShowOnRadar->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlShowOnRadar->GetText() ) ) m_ucShowOnRadar = static_cast< unsigned char >( atoi( xmlShowOnRadar->GetText() ) );
			}
			else m_ucShowOnRadar = 1;

			TiXmlElement* xmlRconPort = xmlRoot->FirstChildElement("RconPort");

			if ((xmlRconPort) && (xmlRconPort->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlRconPort->GetText() ) ) m_usRconPort = static_cast< unsigned short >( atoi( xmlRconPort->GetText() ) );
			}
			else m_usRconPort = 5315;

			TiXmlElement* xmlRconMaxUsers = xmlRoot->FirstChildElement("RconMaxUsers");

			if ((xmlRconMaxUsers) && (xmlRconMaxUsers->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlRconMaxUsers->GetText() ) ) m_usRconMaxUsers = static_cast< unsigned short >( atoi( xmlRconMaxUsers->GetText() ) );
				if ( m_usRconMaxUsers > 50 ) m_usRconMaxUsers = 50;
			}
			else m_usRconMaxUsers = 1;

			TiXmlElement* xmlWeather = xmlRoot->FirstChildElement("WeatherDefault");

			if ((xmlWeather) && (xmlWeather->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlWeather->GetText() ) ) m_ucWeather = static_cast< unsigned char >( atoi( xmlWeather->GetText() ) );
			}
			else m_ucWeather = 4;

			TiXmlElement* xmlWeatherRate = xmlRoot->FirstChildElement("WeatherRate");

			if ((xmlWeatherRate) && (xmlWeatherRate->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlWeatherRate->GetText() ) )
				{
					unsigned int uiRate = static_cast< unsigned int >( atoi( xmlWeatherRate->GetText() ) );
					uiRate *= 1000; // Converts from seconds to milliseconds

					m_uiWeatherRate = uiRate;
				}
			}
			else m_uiWeatherRate = 300000;

			TiXmlElement* xmlHour = xmlRoot->FirstChildElement("HourDefault");

			if ((xmlHour) && (xmlHour->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlHour->GetText() ) ) m_ucHour = static_cast< unsigned char >( atoi( xmlHour->GetText() ) );

				if ( m_ucHour > 23 ) m_ucHour = 0;
			}
			else m_ucHour = 12;

			TiXmlElement* xmlMinute = xmlRoot->FirstChildElement("MinuteDefault");

			if ((xmlMinute) && (xmlMinute->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlMinute->GetText() ) ) m_ucMinute = static_cast< unsigned char >( atoi( xmlMinute->GetText() ) );

				if ( m_ucMinute > 59 ) m_ucMinute = 0;
			}
			else m_ucMinute = 30;

			TiXmlElement* xmlRate = xmlRoot->FirstChildElement("TimeRate");

			if ((xmlRate) && (xmlRate->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlRate->GetText() ) ) m_ucRate = static_cast< unsigned char >( atoi( xmlRate->GetText() ) );
			}
			else m_ucRate = 4;

			TiXmlElement* xmlPickupTime = xmlRoot->FirstChildElement("PickupRespawnTime");

			if ((xmlPickupTime) && (xmlPickupTime->GetText()))
			{
				if ( CUtils::IsNum( (char*)xmlRate->GetText() ) )
				{
					unsigned int uiTime = static_cast< unsigned int >( atoi( xmlPickupTime->GetText() ) );
					uiTime *= 1000; // Converts from seconds to milliseconds

					m_uiPickupRespawnTime = uiTime;
				}
			}
			else m_uiPickupRespawnTime = 60000;

			TiXmlElement* xmlPlayerPos = xmlRoot->FirstChildElement("PlayerPos");
			if (xmlPlayerPos)
			{
				m_vecInitPlayerPos.X = static_cast< float >( atof( xmlPlayerPos->Attribute("x") ) );
				m_vecInitPlayerPos.Y = static_cast< float >( atof( xmlPlayerPos->Attribute("y") ) );
				m_vecInitPlayerPos.Z = static_cast< float >( atof( xmlPlayerPos->Attribute("z") ) );
			}
			else CConsole::PrintC("Error", COLOUR_RED, "'PlayerPos' not found in %s", m_szConfig);

			TiXmlElement* xmlCamPos = xmlRoot->FirstChildElement("CamPos");
			if (xmlCamPos)
			{
				m_vecInitCameraPos.X = static_cast< float >( atof( xmlCamPos->Attribute("x") ) );
				m_vecInitCameraPos.Y = static_cast< float >( atof( xmlCamPos->Attribute("y") ) );
				m_vecInitCameraPos.Z = static_cast< float >( atof( xmlCamPos->Attribute("z") ) );
			}
			else CConsole::PrintC("Error", COLOUR_RED, "'CamPos' not found in %s", m_szConfig);

			TiXmlElement* xmlCamLook = xmlRoot->FirstChildElement("CamLook");
			if (xmlCamLook)
			{
				m_vecInitCameraLook.X = static_cast< float >( atof( xmlCamLook->Attribute("x") ) );
				m_vecInitCameraLook.Y = static_cast< float >( atof( xmlCamLook->Attribute("y") ) );
				m_vecInitCameraLook.Z = static_cast< float >( atof( xmlCamLook->Attribute("z") ) );
			}
			else CConsole::PrintC("Error", COLOUR_RED, "'CamLook' not found in %s", m_szConfig);

			TiXmlElement* xmlWorldBounds = xmlRoot->FirstChildElement("WorldBounds");
			if (xmlWorldBounds)
			{
				m_WorldBounds[0] = static_cast< float >( atof( xmlWorldBounds->Attribute("x1") ) );
				m_WorldBounds[1] = static_cast< float >( atof( xmlWorldBounds->Attribute("y1") ) );
				m_WorldBounds[2] = static_cast< float >( atof( xmlWorldBounds->Attribute("x2") ) );
				m_WorldBounds[3] = static_cast< float >( atof( xmlWorldBounds->Attribute("y2") ) );
			}
			else CConsole::PrintC("Error", COLOUR_RED, "'WorldBounds' not found in %s", m_szConfig);

			TiXmlElement* xmlConsoleOutput = xmlRoot->FirstChildElement("ConsoleOutput");

			if ((xmlConsoleOutput) && (xmlConsoleOutput->GetText()))
			{
				char szText[ 8 ] = { NULL };
				strncpy( szText, xmlConsoleOutput->GetText(), 8 );

				if ((!strcmp(szText, "1")) || (!strcmp(szText, "yes")) || (!strcmp(szText, "true")) || (!strcmp(szText, "on"))) m_bOutputConsole = true;
				else m_bOutputConsole = false;
			}

			TiXmlElement* xmlScoreBoard = xmlRoot->FirstChildElement("EnableDeathmatchScoreboard");

			if ((xmlScoreBoard) && (xmlScoreBoard->GetText()))
			{
				char szText[ 8 ] = { NULL };
				strncpy( szText, xmlScoreBoard->GetText(), 8 );
				if ((!strcmp(szText, "1")) || (!strcmp(szText, "yes")) || (!strcmp(szText, "true")) || (!strcmp(szText, "on"))) m_bScore = true;
				else m_bScore = false;
			}

			TiXmlElement* xmlDriveby = xmlRoot->FirstChildElement("EnableDriveby");

			if ((xmlDriveby) && (xmlDriveby->GetText()))
			{
				char szText[ 8 ] = { NULL };
				strncpy( szText, xmlDriveby->GetText(), 8 );
				if ((!strcmp(szText, "0")) || (!strcmp(szText, "no")) || (!strcmp(szText, "false")) || (!strcmp(szText, "off"))) m_bDriveby = false;
				else m_bScore = true;
			}
		}
		else CConsole::PrintC("Error", COLOUR_RED, "'Settings' section not found in %s", m_szConfig);
	}
}

void CConfigLoader::LoadClasses(void)
{
	if ((m_szConfig) && (m_xmlDoc))
	{
		int x=0;

		TiXmlElement* xmlRoot = m_xmlDoc->FirstChildElement("Class");

		while (xmlRoot)
		{
			CSpawnPool* pSpawn = CSpawnPoolManager::New( x );
			if (pSpawn)
			{
				pSpawn->SetTeam( static_cast< BYTE >(atoi(xmlRoot->Attribute("team"))) );
				pSpawn->SetSkin( static_cast< BYTE >(atoi(xmlRoot->Attribute("skin"))) );
				pSpawn->SetX( (float)atof(xmlRoot->Attribute("x")) );
				pSpawn->SetY( (float)atof(xmlRoot->Attribute("y")) );
				pSpawn->SetZ( (float)atof(xmlRoot->Attribute("z")) );
				pSpawn->SetAngle( (float)atof(xmlRoot->Attribute("angle")) );
				pSpawn->SetWeapon( 0, (int)atoi(xmlRoot->Attribute("weapon1")) );
				pSpawn->SetWeaponAmmo( 0, (int)atoi(xmlRoot->Attribute("ammo1")) );
				pSpawn->SetWeapon( 1, (int)atoi(xmlRoot->Attribute("weapon2")) );
				pSpawn->SetWeaponAmmo( 1, (int)atoi(xmlRoot->Attribute("ammo2")) );
				pSpawn->SetWeapon( 2, (int)atoi(xmlRoot->Attribute("weapon3")) );
				pSpawn->SetWeaponAmmo( 2, (int)atoi(xmlRoot->Attribute("ammo3")) );
				x++;
			}

			xmlRoot = xmlRoot->NextSiblingElement("Class");
		}
	}
}

void CConfigLoader::LoadPickups(void)
{
	if ((m_szConfig) && (m_xmlDoc))
	{
		int x=1;

		TiXmlElement* xmlRoot = m_xmlDoc->FirstChildElement("Pickup");

		while (xmlRoot)
		{
			CPickupPool* pPickup = CPickupPoolManager::New( x );
			if (pPickup)
			{
				pPickup->SetModel( static_cast< unsigned int >(atoi(xmlRoot->Attribute("model"))) );
				pPickup->SetX( (float)atof(xmlRoot->Attribute("x")) );
				pPickup->SetY( (float)atof(xmlRoot->Attribute("y")) );
				pPickup->SetZ( (float)atof(xmlRoot->Attribute("z")) );
				x++;
			}

			xmlRoot = xmlRoot->NextSiblingElement("Pickup");
		}

		if ( CPickupPoolManager::Count() == 336 ) CConsole::PrintC( "WARNING", COLOUR_YELLOW, "The maximum pickup limit has been reached. Some pickups may not spawn." );
	}
}

void CConfigLoader::LoadVehicles(void)
{
	if ((m_szConfig) && (m_xmlDoc))
	{
		int x = 1;

		TiXmlElement* xmlRoot = m_xmlDoc->FirstChildElement("Vehicle");

		while (xmlRoot) 
		{
			CVehiclePool* pVehicle = CVehiclePoolManager::New( x );
			if (pVehicle)
			{
				BYTE bModel = static_cast< BYTE >(atoi(xmlRoot->Attribute("model")));
				pVehicle->SetModel( bModel );
				float fX = (float)atof(xmlRoot->Attribute("x"));
				float fY = (float)atof(xmlRoot->Attribute("y"));
				float fZ = (float)atof(xmlRoot->Attribute("z"));
				float fAngle = (float)atof(xmlRoot->Attribute("angle"));
				pVehicle->SetX( fX );
				pVehicle->SetY( fY );
				pVehicle->SetZ( fZ );
				pVehicle->SetRotation( fAngle );
				pVehicle->SetSpawnX( fX );
				pVehicle->SetSpawnY( fY );
				pVehicle->SetSpawnZ( fZ );
				pVehicle->SetSpawnRotation( fAngle );

				char cCol1 = (char)atoi( xmlRoot->Attribute("col1") );
				char cCol2 = (char)atoi( xmlRoot->Attribute("col2") );

				CCarCols* pCarCols = CCarColManager::Find( bModel );
				if ( pCarCols )
				{
					pCarCols->GenerateRandCol();
					if ( cCol1 == -1 ) 
					{
						cCol1 = pCarCols->RandomColour1();
						pVehicle->SetRandomColour1( true );
					}
					if ( cCol2 == -1 ) 
					{
						cCol2 = pCarCols->RandomColour2();
						pVehicle->SetRandomColour2( true );
					}
				}
				else
				{
					if ( cCol1 == -1 ) cCol1 = 0;
					if ( cCol2 == -1 ) cCol2 = 0;
				}
				pVehicle->SetColour1( cCol1 );
				pVehicle->SetColour2( cCol2 );

				pVehicle->SetSpawned( true );
				x++;
			}

			xmlRoot = xmlRoot->NextSiblingElement("Vehicle");
		}

		if ( CVehiclePoolManager::Count() == MAX_VEHICLES ) CConsole::PrintC( "WARNING", COLOUR_YELLOW, "The maximum vehicle limit has been reached. Some vehicles may not spawn." );
	}
}

void CConfigLoader::LoadAmmunations(void)
{
	if ((m_szConfig) && (m_xmlDoc))
	{
		TiXmlElement* xmlRoot = m_xmlDoc->FirstChildElement("Ammunation");

		while (xmlRoot)
		{
			BYTE bShopID = static_cast< BYTE >(atoi(xmlRoot->Attribute("shopid")));

			CAmmuPool* pAmmunation = CAmmuPoolManager::New( bShopID );
			if (pAmmunation)
			{
				pAmmunation->SetX( (float)atof(xmlRoot->Attribute("x")) );
				pAmmunation->SetY( (float)atof(xmlRoot->Attribute("y")) );
				pAmmunation->SetZ( (float)atof(xmlRoot->Attribute("z")) );
				pAmmunation->SetCameraAngleX( (float)atof(xmlRoot->Attribute("camanglex")) );
				pAmmunation->SetCameraAngleY( (float)atof(xmlRoot->Attribute("camangley")) );
				pAmmunation->SetCameraAngleZ( (float)atof(xmlRoot->Attribute("camanglez")) );
				pAmmunation->SetCameraX( (float)atof(xmlRoot->Attribute("camposx")) );
				pAmmunation->SetCameraY( (float)atof(xmlRoot->Attribute("camposy")) );
				pAmmunation->SetCameraZ( (float)atof(xmlRoot->Attribute("camposz")) );
			}

			xmlRoot = xmlRoot->NextSiblingElement("Ammunation");
		}

		xmlRoot = m_xmlDoc->FirstChildElement("A_Weapon");

		while (xmlRoot)
		{
			BYTE bShopID = static_cast< BYTE >(atoi(xmlRoot->Attribute("shopid")));

			CAmmuPool* pAmmunation = CAmmuPoolManager::Find( bShopID );
			if (pAmmunation)
			{
				pAmmunation->SetWeaponX( (float)atof(xmlRoot->Attribute("x")) );
				pAmmunation->SetWeaponY( (float)atof(xmlRoot->Attribute("y")) );
				pAmmunation->SetWeaponZ( (float)atof(xmlRoot->Attribute("z")) );
				pAmmunation->SetWeapon( 0, (int)atoi(xmlRoot->Attribute("weapon1")) );
				pAmmunation->SetWeaponCost( 0, (int)atoi(xmlRoot->Attribute("cost1")) );
				pAmmunation->SetWeaponEnabled( 0, true );
				pAmmunation->SetWeapon( 1, (int)atoi(xmlRoot->Attribute("weapon2")) );
				pAmmunation->SetWeaponCost( 1, (int)atoi(xmlRoot->Attribute("cost2")) );
				pAmmunation->SetWeaponEnabled( 1, true );
				pAmmunation->SetWeapon( 2, (int)atoi(xmlRoot->Attribute("weapon3")) );
				pAmmunation->SetWeaponCost( 2, (int)atoi(xmlRoot->Attribute("cost3")) );
				pAmmunation->SetWeaponEnabled( 2, true );
				pAmmunation->SetWeapon( 3, (int)atoi(xmlRoot->Attribute("weapon4")) );
				pAmmunation->SetWeaponCost( 3, (int)atoi(xmlRoot->Attribute("cost4")) );
				pAmmunation->SetWeaponEnabled( 3, true );
			}

			xmlRoot = xmlRoot->NextSiblingElement("A_Weapon");
		}
	}
}

void CConfigLoader::LoadSquirrelScripts(void)
{
	if ((m_szConfig) && (m_xmlDoc))
	{
		TiXmlElement* xmlScript = m_xmlDoc->FirstChildElement("script");

		while ((xmlScript) && (xmlScript->GetText()))
		{
			CSquirrel* pSquirrel = CSquirrelManager::New((char*)xmlScript->GetText());
			if (pSquirrel)
			{
				pSquirrel->PushFunction("onScriptLoad");
				pSquirrel->CallFunction();
			}
			xmlScript = xmlScript->NextSiblingElement("script");
		}
	}
}

void CConfigLoader::LoadVehicleColours(void)
{
	TiXmlDocument XMLdoc("carcols.conf");
    bool loadOkay = XMLdoc.LoadFile();
	if ( loadOkay )
	{
		TiXmlElement* xmlColour = XMLdoc.FirstChildElement("Colours");

		while ( xmlColour )
		{
			unsigned char ucModel = static_cast< unsigned char >(atoi(xmlColour->Attribute("model")));
			unsigned char uc1 = static_cast< unsigned char >(atoi(xmlColour->Attribute("col1")));
			unsigned char uc2 = static_cast< unsigned char >(atoi(xmlColour->Attribute("col2")));

			CCarCols* pCarCols = CCarColManager::Find( ucModel );
			if ( !pCarCols ) pCarCols = CCarColManager::New( ucModel );

			if ( pCarCols )
			{
				pCarCols->AddColour( uc1, uc2 );
			}

			xmlColour = xmlColour->NextSiblingElement("Colours");
		}
	}
}
