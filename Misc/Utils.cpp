/*
	Utils.cpp
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

	File Author: VRocker

	--------------------------------------------------------------------
*/

#include "Utils.h"
#include <math.h>

const char* const a_to_z = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" ;

#ifndef _WIN32
char *strdup(char *string)
{
      char *s;

      if (string)
      {
            for (s = string; *s; ++s)
                  *s = toupper(*s);
      }
      return string;
}

char *strlwr(char *string)
{
      char *s;

      if (string)
      {
            for (s = string; *s; ++s)
                  *s = tolower(*s);
      }
      return string;
}
#endif

unsigned char CUtils::NumTok( char* szText, char* szDelimiter )
{
	char* szLocalText = new char [ strlen ( szText ) + 1 ];
	memset( szLocalText, '\0', strlen(szText) + 1 );
	memcpy( szLocalText, szText, strlen( szText ) );
	char* szTokens = strtok( szLocalText, szDelimiter );
	unsigned char i = 1;
	if ( szTokens == NULL ) i = 0;
	while ( szTokens != NULL ) {
		szTokens = strtok( NULL, szDelimiter );
		if ( szTokens != NULL ) i++;
	}
	delete [] szLocalText;
	return i;
}

bool CUtils::IsIn( const char* sz1, const char* sz2 )
{
	const char* szReturn;
	int result = 0;
	sz1 = strlwr( strdup( sz1 ) );
	sz2 = strlwr( strdup( sz2 ) );
	szReturn = strstr( sz1, sz2 );
	result = szReturn - sz1 + 1;
	if( szReturn != NULL ) return true;
	else return false;
}

bool CUtils::IsNum ( const char* sz )
{
	const char szSet [ 15 ] = "1234567890";
	if ( strspn( sz, szSet ) == strlen( sz ) )
	{
		return true;
	}
	return false;
}

float CUtils::DistanceFromPoint( float fX1, float fY1, float fX2, float fY2 )
{
	float fSX,fSY;

	fSX = (fX1 - fX2) * (fX1 - fX2);
	fSY = (fY1 - fY2) * (fY1 - fY2);

	return (float)sqrt(fSX + fSY);
}

void CUtils::RandomString( char* szString )
{
RegenLength:
	unsigned char ucNameLen = rand() % 30;

	if ( ucNameLen < 2 ) goto RegenLength;

	for ( unsigned char uc = 0; uc < ucNameLen; uc++ )
	{
		szString[ uc ] = a_to_z[ rand() % strlen( a_to_z ) ];
	}
}

void CUtils::FixIniBug( const char* sz, char* szBuffer )
{
	for ( unsigned short i = 0; i < strlen( sz ); i++ )
	{
		if ( sz[i] == '[' ) szBuffer[i] = 6;
		else if ( sz[i] == ']' ) szBuffer[i] = 7;
		else if ( sz[i] == 6 ) szBuffer[i] = '[';
		else if ( sz[i] == 7 ) szBuffer[i] = ']';
		else szBuffer[i] = sz[i];
	}
}

bool CUtils::InPoly( float fX, float fY, const unsigned int uiPoints, const AreaPoints fPoints[] )
{
	// FOUND THIS AT http://sidvind.com/wiki/Point-in-polygon:_Jordan_Curve_Theorem

	/* The points creating the polygon. */
	float x1,x2;

	/* How many times the ray crosses a line-segment */
	int crossings = 0;

	/* Iterate through each line */
	for ( int i = 0; i < uiPoints; i++ ){

		/* This is done to ensure that we get the same result when
		the line goes from left to right and right to left */
		if ( fPoints[ i ].x < fPoints[ ( i + 1 ) % uiPoints ].x ){
			x1 = fPoints[ i ].x;
			x2 = fPoints[ ( i + 1 ) % uiPoints ].x;
		} else {
			x1 = fPoints[ ( i + 1 ) % uiPoints ].x;
			x2 = fPoints[ i ].x;
		}

		/* First check if the ray is possible to cross the line */
		if ( fX > x1 && fX <= x2 && ( fY < fPoints[ i ].y || fY <= fPoints[ ( i + 1 ) % uiPoints ].y ) ) {
			static const float eps = 0.000001f;

			/* Calculate the equation of the line */
			float dx = fPoints[ ( i + 1 ) % uiPoints ].x - fPoints[ i ].x;
			float dy = fPoints[ ( i + 1 ) % uiPoints ].y - fPoints[ i ].y;
			float k;

			if ( fabs(dx) < eps ){
				k = 0xffffffff;	// math.h
			} else {
				k = dy/dx;
			}

			float m = fPoints[ i ].y - k * fPoints[ i ].x;

			/* Find if the ray crosses the line */
			float y2 = k * fX + m;
			if ( fY <= y2 ){
				crossings++;
			}
		}
	}

	if ( crossings % 2 == 1 ) return true;

	return false;
}

const double fStarfish[ 8 ] = { -748.206, -241.467, -104.505, -241.467, -104.505, -818.266, -748.206, -818.266 };
const double fOceanBeach[ 8 ] = { -253.206, -930.526, 1254.9, -930.526, 1254.9, -1805.37, -253.206, -1805.37 };
const double fVicePoint[ 8 ] = { 163.656, 1398.85, 1246.03, 1398.85, 1246.03, -351.153, 163.656, -351.153 };
const double fPrawnIsland[ 8 ] = { -213.73, 1243.47, 163.656, 1243.47, 163.656, 797.605, -213.73, 797.605 };
const double fWashingtonBeach[ 8 ] = { -103.97, -351.153, 1246.03, -351.153, 1246.03, -930.526, -103.97, -930.526 };
const double fDowntown[ 8 ] = { -1613.03, 1677.32, -213.73, 1677.32, -213.73, 413.128, -1613.03, 413.128 };
const double fVicePort[ 8 ] = { -1208.21, -898.738, -253.206, -898.738, -253.206, -1779.61, -1208.21, -1779.61 };
const double fLeafLinks[ 8 ] = { -213.73, 797.605, 163.656, 797.605, 163.656, -241.429, -213.73, -241.429 };
const double fLittleHaiti[ 8 ] = { -1208.21, 412.66, -578.289, 412.66, -578.289, -241.467, -1208.21, -241.467 };
const double fJunkyard[ 8 ] = { -1396.76, 230.39, -1208.21, 230.39, -1208.21, -42.9113, -1396.76, -42.9113 };
const double fLittleHavana[ 8 ] = { -1208.21, -241.467, -748.206, -241.467, -748.206, -898.738, -1208.21, -898.738 };
const double fEscobar1[ 8 ] = { -1888.21, 230.39, -1208.21, 230.39, -1208.21, -1779.61, -1888.21, -1779.61 };
const double fEscobar2[ 8 ] = { -1276.19, -470.199, -1233.19, -470.199, -1233.19, -865.199, -1276.19, -865.199 };
const double fEscobar3[ 8 ] = { -1616.19, -695.199, -1276.19, -695.199, -1276.19, -945.199, -1616.19, -945.199 };

const double* fAreas[] = { fStarfish, fOceanBeach, fVicePoint, fPrawnIsland, fWashingtonBeach, fDowntown, fVicePort, fLeafLinks, fLittleHaiti, fJunkyard, fLittleHavana, fEscobar1, fEscobar2, fEscobar3 };
const char* szAreaNames[] = { "Starfish Island", "Ocean Beach", "Vice Point", "Prawn Island", "Washington Beach", "Downtown", "Vice Port", "Leaf Links", "Little Haiti", "Junkyard", "Little Havana", "Escobar International", "Escobar International", "Escobar International" };


bool CUtils::PointInArea( float fX, float fY, const double fArea[ 8 ] )
{
	if ( ( ( fX > fArea[ 0 ] ) && ( fX < fArea[ 4 ] ) ) && ( ( fY > fArea[5] ) && ( fY < fArea[ 1 ] ) ) ) return true;
	return false;
}

const char* CUtils::GetDeathReason( BYTE b )
{
	switch ( b )
	{
	case 0:
		return "Fist";
		break;

	case 1:
		return "Brase Knuckle";
		break;

	case 2:
		return "Screwdriver";
		break;

	case 3:
		return "Golf Club";
		break;

	case 4:
		return "Nightstick";
		break;

	case 5:
		return "Knife";
		break;

	case 6:
		return "Baseball Bat";
		break;

	case 7:
		return "Hammer";
		break;

	case 8:
		return "Cleaver";
		break;

	case 9:
		return "Machete";
		break;

	case 10:
		return "Katana";
		break;

	case 11:
		return "Chainsaw";
		break;

	case 12:
		return "Grenades";
		break;

	case 13:
		return "Remote Grenades";
		break;

	case 14:
		return "Teargas";
		break;

	case 15:
		return "Molotov Cocktails";
		break;

	case 16:
		return "Missile";
		break;

	case 17:
		return "Colt45";
		break;

	case 18:
		return "Python";
		break;

	case 19:
		return "Shotgun";
		break;

	case 20:
		return "Spaz";
		break;

	case 21:
		return "Stubby";
		break;

	case 22:
		return "Tec9";
		break;

	case 23:
		return "Uzi";
		break;

	case 24:
		return "Silenced Ingram";
		break;

	case 25:
		return "MP5";
		break;

	case 26:
		return "M4";
		break;

	case 27:
		return "Ruger";
		break;

	case 28:
		return "Sniper Rifle";
		break;

	case 29:
		return "Laserscope Sniper Rifle";
		break;

	case 30:
		return "Rocketlauncher";
		break;

	case 31:
		return "Flamethrower";
		break;

	case 32:
		return "m60";
		break;

	case 33:
		return "Minigun";
		break;

	case 34:
		return "Bomb";
		break;

	case 35:
		return "Helicannon";
		break;

	case 36:
		return "Camera";
		break;

	case 39:
		return "Vehicle";
		break;

	case 41:
		return "Fall";
		break;

	case 42:
		return "Drive-By";
		break;

	case 43:
		return "Drowned";
		break;

	case 51:
		return "Explosion";
		break;

	default:
		return "Unknown";
		break;
	}
}

const char* CUtils::GetVehicleName( BYTE b )
{
	switch ( b )
	{
	case 166:
		return "Angel";
		break;

	case 178:
		return "Pizza Boy";
		break;

	case 191:
		return "PCJ-600";
		break;

	case 192:
		return "Faggio";
		break;

	case 193:
		return "Freeway";
		break;

	case 198:
		return "Sanchez";
		break;

	case 136:
		return "Rio";
		break;

	case 160:
		return "Predator";
		break;

	case 176:
		return "Squalo";
		break;

	case 182:
		return "Speeder";
		break;

	case 183:
		return "Reefer";
		break;

	case 184:
		return "Tropic";
		break;

	case 202:
		return "Coast Guard";
		break;

	case 203:
		return "Dinghy";
		break;

	case 214:
		return "Marquis";
		break;

	case 223:
		return "Cuban Jetmax";
		break;

	case 155:
		return "Hunter";
		break;

	case 165:
		return "Helicopter";
		break;

	case 177:
		return "Sea Sparrow";
		break;

	case 195:
		return "RC Raider";
		break;

	case 199:
		return "Sparrow";
		break;

	case 217:
		return "Maverick";
		break;

	case 218:
		return "VCN Maverick";
		break;

	case 227:
		return "Police Maverick";
		break;

	case 231:
		return "RC Goblin";
		break;

	case 190:
		return "Skimmer";
		break;

	case 194:
		return "RC Baron";
		break;

	case 133:
		return "Linerunner";
		break;

	case 138:
		return "Trashmaster";
		break;

	case 158:
		return "Securicar";
		break;

	case 161:
		return "Bus";
		break;

	case 173:
		return "Packer";
		break;

	case 185:
		return "Flatbed";
		break;

	case 132:
		return "Stinger";
		break;

	case 141:
		return "Infernus";
		break;

	case 145:
		return "Cheetah";
		break;

	case 154:
		return "BF Injection";
		break;

	case 159:
		return "Banshee";
		break;

	case 174:
		return "Sentinel XS";
		break;

	case 210:
		return "Comet";
		break;

	case 137:
		return "Firetruck";
		break;

	case 139:
		return "Stretch";
		break;

	case 142:
		return "Voodoo";
		break;

	case 146:
		return "Ambulance";
		break;

	case 147:
		return "FBI Washington";
		break;

	case 153:
		return "Mr Whoopee";
		break;

	case 156:
		return "Police";
		break;

	case 157:
		return "Enforcer";
		break;

	case 162:
		return "Rhino";
		break;

	case 163:
		return "Barracks OL";
		break;

	case 164:
		return "Cuban Hermes";
		break;

	case 167:
		return "Coach";
		break;

	case 171:
		return "RC Bandit";
		break;

	case 172:
		return "Romero's Hearse";
		break;

	case 179:
		return "Gang Burrito";
		break;

	case 187:
		return "Caddy";
		break;

	case 188:
		return "Zebra Cab";
		break;

	case 189:
		return "Top Fun";
		break;

	case 200:
		return "Patriot";
		break;

	case 201:
		return "Love Fist";
		break;

	case 206:
		return "Sabre Turbo";
		break;

	case 211:
		return "Deluxo";
		break;

	case 213:
		return "Spand Express";
		break;

	case 215:
		return "Baggage Handler";
		break;

	case 220:
		return "FBI Rancher";
		break;

	case 224:
		return "Hotring Racer";
		break;

	case 225:
		return "Sandking";
		break;

	case 232:
		return "Hotring Racer";
		break;

	case 233:
		return "Hotring Racer";
		break;

	case 234:
		return "Bloodring Banger";
		break;

	case 235:
		return "Bloodring Banger";
		break;

	case 236:
		return "ViceChee";
		break;

	case 130:
		return "Landstalker";
		break;

	case 148:
		return "Moonbeam";
		break;

	case 149:
		return "Esperanto";
		break;

	case 196:
		return "Glendale";
		break;

	case 197:
		return "Oceanic";
		break;

	case 205:
		return "Sabre";
		break;

	case 209:
		return "Regina";
		break;

	case 212:
		return "Burrito";
		break;

	case 221:
		return "Virgo";
		break;

	case 226:
		return "Blista Compact";
		break;

	case 230:
		return "Mesa Grande";
		break;

	case 131:
		return "Idaho";
		break;

	case 134:
		return "Perennial";
		break;

	case 140:
		return "Manana";
		break;

	case 169:
		return "Stallion";
		break;

	case 170:
		return "Rumpo";
		break;

	case 222:
		return "Greenwood";
		break;

	case 135:
		return "Sentinel";
		break;

	case 151:
		return "Washington";
		break;

	case 175:
		return "Admiral";
		break;

	case 204:
		return "Hermes";
		break;

	case 207:
		return "Phoenix";
		break;

	case 219:
		return "Rancher";
		break;

	case 150:
		return "Taxi";
		break;

	case 168:
		return "Cabbie";
		break;

	case 216:
		return "Kaufman Cab";
		break;

	case 143:
		return "Pony";
		break;

	case 144:
		return "Mule";
		break;

	case 152:
		return "Bobcat";
		break;

	case 186:
		return "Yankee";
		break;

	case 208:
		return "Walton";
		break;

	case 228:
		return "Boxville";
		break;

	case 229:
		return "Benson";
		break;

	default:
		return "Unknown";
		break;
	}
}

const char* CUtils::GetAreaName( float fX, float fY )
{
	BYTE b = 0;
	while ( b < 14 )
	{
		if ( PointInArea( fX, fY, fAreas[ b ] ) ) return szAreaNames[ b ];
		b++;
	}
	return "Vice City";
}

BYTE CUtils::GetWeaponID( const char* sz )
{
	if ( IsIn( "Fist", sz ) ) return 0;
	else if ( IsIn( "Brass Knuckle", sz ) ) return 1;
	else if ( IsIn( "Screwdriver", sz ) ) return 2;
	else if ( IsIn( "Golf Club", sz ) ) return 3;
	else if ( IsIn( "Night Stick", sz ) ) return 4;
	else if ( IsIn( "Knife", sz ) ) return 5;
	else if ( IsIn( "Baseball Bat", sz ) ) return 6;
	else if ( IsIn( "Hammer", sz ) ) return 7;
	else if ( IsIn( "Meat Cleaver", sz ) ) return 8;
	else if ( IsIn( "Machete", sz ) ) return 9;
	else if ( IsIn( "Katana", sz ) ) return 10;
	else if ( IsIn( "Chainsaw", sz ) ) return 11;
	else if ( IsIn( "Grenade", sz ) ) return 12;
	else if ( IsIn( "Teargas", sz ) ) return 14;
	else if ( IsIn( "Molotov Cocktails", sz ) ) return 15;
	else if ( IsIn( "Colt45", sz ) ) return 17;
	else if ( IsIn( "Python", sz ) ) return 18;
	else if ( IsIn( "Shotgun", sz ) ) return 19;
	else if ( IsIn( "Spaz Shotgun", sz ) ) return 20;
	else if ( IsIn( "Stubby Shotgun", sz ) ) return 21;
	else if ( IsIn( "Tec9", sz ) ) return 22;
	else if ( IsIn( "Uzi", sz ) ) return 23;
	else if ( IsIn( "Ingrams", sz ) ) return 24;
	else if ( IsIn( "MP5", sz ) ) return 25;
	else if ( IsIn( "M4", sz ) ) return 26;
	else if ( IsIn( "Ruger", sz ) ) return 27;
	else if ( IsIn( "Sniper Rifle", sz ) ) return 28;
	else if ( IsIn( "Laser Sniper", sz ) ) return 29;
	else if ( IsIn( "Rocket Launcher", sz ) ) return 30;
	else if ( IsIn( "Flamethrower", sz ) ) return 31;
	else if ( IsIn( "M60", sz ) ) return 32;
	else if ( IsIn( "Minigun", sz ) ) return 33;
	else if ( IsIn( "Bomb", sz ) ) return 34;
	else return 255;
}

BYTE CUtils::GetVehicleModel( const char* sz )
{
	if ( IsIn( "Landstalker", sz ) ) return 130;
	else if ( IsIn( "Idaho", sz ) ) return 131;
	else if ( IsIn( "Stinger", sz ) ) return 132;
	else if ( IsIn( "Linerunner", sz ) ) return 133;
	else if ( IsIn( "Perennial", sz ) ) return 134;
	else if ( IsIn( "Sentinel", sz ) ) return 135;
	else if ( IsIn( "Rio", sz ) ) return 136;
	else if ( IsIn( "Firetruck", sz ) ) return 137;
	else if ( IsIn( "Trashmaster", sz ) ) return 138;
	else if ( IsIn( "Stretch", sz ) ) return 139;
	else if ( IsIn( "Manana", sz ) ) return 140;
	else if ( IsIn( "Infernus", sz ) ) return 141;
	else if ( IsIn( "Voodoo", sz ) ) return 142;
	else if ( IsIn( "Pony", sz ) ) return 143;
	else if ( IsIn( "Mule", sz ) ) return 144;
	else if ( IsIn( "Cheetah", sz ) ) return 145;
	else if ( IsIn( "Ambulance", sz ) ) return 146;
	else if ( IsIn( "Washington", sz ) ) return 151;
	else if ( IsIn( "FBI Washington", sz ) ) return 147;
	else if ( IsIn( "Moonbeam", sz ) ) return 148;
	else if ( IsIn( "Esperanto", sz ) ) return 149;
	else if ( IsIn( "Taxi", sz ) ) return 150;
	else if ( IsIn( "Bobcat", sz ) ) return 152;
	else if ( IsIn( "Mr. Whoopee", sz ) ) return 153;
	else if ( IsIn( "BF Injection", sz ) ) return 154;
	else if ( IsIn( "Hunter", sz ) ) return 155;
	else if ( IsIn( "Police", sz ) ) return 156;
	else if ( IsIn( "Enforcer", sz ) ) return 157;
	else if ( IsIn( "Securicar", sz ) ) return 158;
	else if ( IsIn( "Banshee", sz ) ) return 159;
	else if ( IsIn( "Predator", sz ) ) return 160;
	else if ( IsIn( "Bus", sz ) ) return 161;
	else if ( IsIn( "Rhino", sz ) ) return 162;
	else if ( IsIn( "Barracks OL", sz ) ) return 163;
	else if ( IsIn( "Hermes", sz ) ) return 204;
	else if ( IsIn( "Cuban Hermes", sz ) ) return 164;
	else if ( IsIn( "Helicopter", sz ) ) return 165;
	else if ( IsIn( "Angel", sz ) ) return 166;
	else if ( IsIn( "Coach", sz ) ) return 167;
	else if ( IsIn( "Cabbie", sz ) ) return 168;
	else if ( IsIn( "Stallion", sz ) ) return 169;
	else if ( IsIn( "Rumpo", sz ) ) return 170;
	else if ( IsIn( "RC Bandit", sz ) ) return 171;
	else if ( IsIn( "Romeros Hearse", sz ) ) return 172;
	else if ( IsIn( "Packer", sz ) ) return 173;
	else if ( IsIn( "Sentinel XS", sz ) ) return 174;
	else if ( IsIn( "Admiral", sz ) ) return 175;
	else if ( IsIn( "Squalo", sz ) ) return 176;
	else if ( IsIn( "Sea Sparrow", sz ) ) return 177;
	else if ( IsIn( "Pizza boy", sz ) ) return 178;
	else if ( IsIn( "Gang Burrito", sz ) ) return 179;
	else if ( IsIn( "Speeder", sz ) ) return 182;
	else if ( IsIn( "Reefer", sz ) ) return 183;
	else if ( IsIn( "Tropic", sz ) ) return 184;
	else if ( IsIn( "Flatbed", sz ) ) return 185;
	else if ( IsIn( "Yankee", sz ) ) return 186;
	else if ( IsIn( "Caddy", sz ) ) return 187;
	else if ( IsIn( "Zebra Cab", sz ) ) return 188;
	else if ( IsIn( "Top Fun", sz ) ) return 189;
	else if ( IsIn( "Skimmer", sz ) ) return 190;
	else if ( IsIn( "PCJ 600", sz ) ) return 191;
	else if ( IsIn( "Faggio", sz ) ) return 192;
	else if ( IsIn( "Freeway", sz ) ) return 193;
	else if ( IsIn( "Glendale", sz ) ) return 196;
	else if ( IsIn( "Oceanic", sz ) ) return 197;
	else if ( IsIn( "Sanchez", sz ) ) return 198;
	else if ( IsIn( "Sanchez", sz ) ) return 199;
	else if ( IsIn( "Patriot", sz ) ) return 200;
	else if ( IsIn( "Love Fist", sz ) ) return 201;
	else if ( IsIn( "Coast Guard", sz ) ) return 202;
	else if ( IsIn( "Dinghy", sz ) ) return 203;
	else if ( IsIn( "Sabre", sz ) ) return 205;
	else if ( IsIn( "Sabre Turbo", sz ) ) return 206;
	else if ( IsIn( "Phoenix", sz ) ) return 207;
	else if ( IsIn( "Walton", sz ) ) return 208;
	else if ( IsIn( "Regina", sz ) ) return 209;
	else if ( IsIn( "Comet", sz ) ) return 210;
	else if ( IsIn( "Deluxo", sz ) ) return 211;
	else if ( IsIn( "Burrito", sz ) ) return 212;
	else if ( IsIn( "Spand Express", sz ) ) return 213;
	else if ( IsIn( "Marquis", sz ) ) return 214;
	else if ( IsIn( "Baggage Handler", sz ) ) return 215;
	else if ( IsIn( "Kaufman Cab", sz ) ) return 216;
	else if ( IsIn( "Maverick", sz ) ) return 217;
	else if ( IsIn( "VCN Maverick", sz ) ) return 218;
	else if ( IsIn( "Rancher", sz ) ) return 219;
	else if ( IsIn( "FBI Rancher", sz ) ) return 220;
	else if ( IsIn( "Virgo", sz ) ) return 221;
	else if ( IsIn( "Greenwood", sz ) ) return 222;
	else if ( IsIn( "Cuban Jetmax", sz ) ) return 223;
	else if ( IsIn( "Hotring Racer 1", sz ) ) return 224;
	else if ( IsIn( "Sandking", sz ) ) return 225;
	else if ( IsIn( "Blista Compact", sz ) ) return 226;
	else if ( IsIn( "Police Maverick", sz ) ) return 227;
	else if ( IsIn( "Boxville", sz ) ) return 228;
	else if ( IsIn( "Benson", sz ) ) return 229;
	else if ( IsIn( "Mesa Grande", sz ) ) return 230;
	else if ( IsIn( "Hotring Racer 2", sz ) ) return 232;
	else if ( IsIn( "Hotring Racer 3", sz ) ) return 233;
	else if ( IsIn( "Bloodring Banger 1", sz ) ) return 234;
	else if ( IsIn( "Bloodring Banger 2", sz ) ) return 235;
	else if ( IsIn( "FBI Cheetah", sz ) ) return 236;
	else return 255;
}

// Skin ID to name function
// Credits for the skin list goes to Mattz

const char* CUtils::GetSkinName( unsigned char uc )
{
	switch ( uc )
	{
	case 0:
		return "Tommy Vercetti";
		break;

	case 1:
		return "Cop";
		break;

	case 2:
		return "Swat";
		break;

	case 3:
		return "FBI";
		break;

	case 4:
		return "Army";
		break;

	case 5:
		return "Paramedic";
		break;

	case 6:
		return "Fireman";
		break;

	case 7:
		return "Golf guy #1";
		break;

	case 9:
		return "Bum lady #1";
		break;

	case 10:
		return "Bum lady #2";
		break;

	case 11:
		return "Punk #1";
		break;

	case 12:
		return "Lawyer";
		break;

	case 13:
		return "Spanish lady #1";
		break;

	case 14:
		return "Spanish lady #2";
		break;

	case 15:
		return "Cool guy #1";
		break;

	case 16:
		return "Arabic guy";
		break;

	case 17:
		return "Beach lady #1";
		break;

	case 18:
		return "Beach lady #2";
		break;

	case 19:
		return "Beach guy #1";
		break;

	case 20:
		return "Beach guy #2";
		break;

	case 21:
		return "Office lady #1";
		break;

	case 22:
		return "Waitress #1";
		break;

	case 23:
		return "Food lady";
		break;

	case 24:
		return "Prostitue #1";
		break;

	case 25:
		return "Bum lady #2";
		break;

	case 26:
		return "Bum guy #1";
		break;

	case 27:
		return "Garbageman #1";
		break;

	case 28:
		return "Taxi driver #1";
		break;

	case 29:
		return "Hatian #1";
		break;

	case 30:
		return "Criminal #1";
		break;

	case 31:
		return "Hood lady";
		break;

	case 32:
		return "Granny #1";
		break;

	case 33:
		return "Business man #1";
		break;

	case 34:
		return "Church guy";
		break;

	case 35:
		return "Club lady";
		break;

	case 36:
		return "Church lady";
		break;

	case 37:
		return "Pimp";
		break;

	case 38:
		return "Beach lady #3";
		break;

	case 39:
		return "Beach guy #3";
		break;

	case 40:
		return "Beach lady #4";
		break;

	case 41:
		return "Beach guy #4";
		break;

	case 42:
		return "Business man #2";
		break;

	case 43:
		return "Prostitute #2";
		break;

	case 44:
		return "Bum lady #3";
		break;

	case 45:
		return "Bum guy #2";
		break;

	case 46:
		return "Hatian #2";
		break;

	case 47:
		return "Construction worker #1";
		break;

	case 48:
		return "Punk #2";
		break;

	case 49:
		return "Prostitute #2";
		break;

	case 50:
		return "Granny #2";
		break;

	case 51:
		return "Punk #3";
		break;

	case 52:
		return "Business man #3";
		break;

	case 53:
		return "Spanish lady #3";
		break;

	case 54:
		return "Spanish lady #4";
		break;

	case 55:
		return "Cool guy #2";
		break;

	case 56:
		return "Business man #4";
		break;

	case 57:
		return "Beach lady #5";
		break;

	case 58:
		return "Beach guy #5";
		break;

	case 59:
		return "Beach lady #6";
		break;

	case 60:
		return "Beach guy #6";
		break;

	case 61:
		return "Construction worker #2";
		break;

	case 62:
		return "Golf guy #2";
		break;

	case 63:
		return "Golf lady";
		break;

	case 64:
		return "Golf guy #3 ";
		break;

	case 65:
		return "Beach lady #7";
		break;

	case 66:
		return "Beach guy #7";
		break;

	case 67:
		return "Office lady #2";
		break;

	case 68:
		return "Business man #5";
		break;

	case 69:
		return "Business man #6";
		break;

	case 70:
		return "Prostitute #2";
		break;

	case 71:
		return "Bum lady #4";
		break;

	case 72:
		return "Bum guy #3";
		break;

	case 73:
		return "Spanish guy";
		break;

	case 74:
		return "Taxi driver #2";
		break;

	case 75:
		return "Gym lady";
		break;

	case 76:
		return "Gym guy";
		break;

	case 77:
		return "Skate lady";
		break;

	case 78:
		return "Skate guy";
		break;

	case 79:
		return "Shopper #1";
		break;

	case 80:
		return "Shopper #2";
		break;

	case 81:
		return "Tourist #1";
		break;

	case 82:
		return "Tourist #2";
		break;

	case 83:
		return "Cuban #1";
		break;

	case 84:
		return "Cuban #2";
		break;

	case 85:
		return "Hatian #3";
		break;

	case 86:
		return "Hatian #4";
		break;

	case 87:
		return "Shark #1";
		break;

	case 88:
		return "Shark #2";
		break;

	case 89:
		return "Diaz guy #1";
		break;

	case 90:
		return "Diaz guy #2";
		break;

	case 91:
		return "DBP security #1";
		break;

	case 92:
		return "DBP security #2";
		break;

	case 93:
		return "Biker #1";
		break;

	case 94:
		return "Biker #2";
		break;

	case 95:
		return "Vercetti guy #1";
		break;

	case 96:
		return "Vercetti guy #2";
		break;

	case 97:
		return "Undercover cop #1";
		break;

	case 98:
		return "Undercover cop #2";
		break;

	case 99:
		return "Undercover cop #3";
		break;

	case 100:
		return "Undercover cop #4";
		break;

	case 101:
		return "Undercover cop #5";
		break;

	case 102:
		return "Undercover cop #6";
		break;

	case 103:
		return "Rich guy";
		break;

	case 104:
		return "Cool guy #3";
		break;

	case 105:
		return "Prostitute #3";
		break;

	case 106:
		return "Prostitute #4";
		break;

	case 107:
		return "Love Fist #1";
		break;

	case 108:
		return "Ken Rosenburg";
		break;

	case 109:
		return "Candy Suxx";
		break;

	case 110:
		return "Hilary";
		break;

	case 111:
		return "Love Fist #2";
		break;

	case 112:
		return "Phil";
		break;

	case 113:
		return "Rockstar guy";
		break;

	case 114:
		return "Sonny";
		break;

	case 115:
		return "Lance";
		break;

	case 116:
		return "Mercades";
		break;

	case 117:
		return "Love Fist #3";
		break;

	case 118:
		return "Alex Srub";
		break;

	case 119:
		return "Lance (Cop)";
		break;

	case 120:
		return "Lance";
		break;

	case 121:
		return "Cortez";
		break;

	case 122:
		return "Love Fist #3";
		break;

	case 123:
		return "Columbian guy #1";
		break;

	case 124:
		return "Hilary (Robber)";
		break;

	case 125:
		return "Mercades";
		break;

	case 126:
		return "Cam";
		break;

	case 127:
		return "Cam (Robber)";
		break;

	case 128:
		return "Phil (One arm)";
		break;

	case 129:
		return "Phil (Robber)";
		break;

	case 130:
		return "Cool guy #4";
		break;

	case 131:
		return "Pizzaman";
		break;

	case 132:
		return "Taxi driver #1";
		break;

	case 133:
		return "Taxi driver #2";
		break;

	case 134:
		return "Sailor #1";
		break;

	case 135:
		return "Sailor #2";
		break;

	case 136:
		return "Sailor #3";
		break;

	case 137:
		return "Chef";
		break;

	case 138:
		return "Criminal #2";
		break;

	case 139:
		return "French guy";
		break;

	case 140:
		return "Garbageman #2";
		break;

	case 141:
		return "Hatian #5";
		break;

	case 142:
		return "Waitress #2";
		break;

	case 143:
		return "Sonny guy #1";
		break;

	case 144:
		return "Sonny guy #2";
		break;

	case 145:
		return "Sonny guy #3";
		break;

	case 146:
		return "Columbian guy #2";
		break;

	case 147:
		return "Thug #1";
		break;

	case 148:
		return "Beach guy #8";
		break;

	case 149:
		return "Garbageman #3";
		break;

	case 150:
		return "Garbageman #4";
		break;

	case 151:
		return "Garbageman #5";
		break;

	case 152:
		return "Tranny";
		break;

	case 153:
		return "Thug #5";
		break;

	case 154:
		return "SpandEx guy #1";
		break;

	case 155:
		return "SpandEx guy #2";
		break;

	case 156:
		return "Stripper #1";
		break;

	case 157:
		return "Stripper #2";
		break;

	case 158:
		return "Stripper #3";
		break;

	case 159:
		return "Store clerk";
		break;

	default:
		return "Unknown";
		break;
	}
}