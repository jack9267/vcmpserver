#include "Utils.h"
#include <math.h>

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

unsigned char Utils::NumTok( char* szText, char* szDelimiter )
{
	char* szLocalText = new char [ strlen ( szText ) + 1 ];
	memset( szLocalText, '\0', strlen(szText) + 1 );
	memcpy( szLocalText, szText, strlen( szText ) );
	char* szTokens = strtok( szLocalText, szDelimiter );
	unsigned char i = 1;
	while ( szTokens != NULL ) {
		szTokens = strtok( NULL, szDelimiter );
		if ( szTokens != NULL ) i++;
	}
	delete [] szLocalText;
	return i;
}

bool Utils::IsNum ( char* sz )
{
	char szSet [ 15 ] = "1234567890";
	if ( strspn( sz, szSet ) == strlen( sz ) )
	{
		return true;
	}
	return false;
}

bool Utils::IsIn( char* sz1, char* sz2 ) 
{ 
	char* szReturn;
	int result = 0; 
	sz1 = _strlwr( _strdup( sz1 ) ); 
	sz2 = _strlwr( _strdup( sz2 ) ); 
	szReturn = strstr( sz1, sz2 );
	result = szReturn - sz1 + 1; 
	if( szReturn != NULL ) return true;
	else return false; 
}

float Utils::CalcSpeed( float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2 )
{
	float fX = ( fX1 - fX2 );
	float fY = ( fY1 - fY2 );
	float fZ = ( fZ1 - fZ2 );
	float fSpeed = ( ( fX * fX ) + ( fY * fY ) + ( fZ * fZ ) );
	return sqrt( fSpeed );
}

bool Utils::PointInArea( float fX, float fY, const double fArea[ 8 ] )
{
	if ( ( ( fX > fArea[ 0 ] ) && ( fX < fArea[ 4 ] ) ) && ( ( fY > fArea[5] ) && ( fY < fArea[ 1 ] ) ) ) return true;
	return false;
}

char* Utils::Strip( char* szText )
{
	BYTE b = 0;
	while ( b < NumTok( szText, "$" ) )
	{
		if ( b == 0 ) strcpy( szText, strtok( szText, "$" ) );
		else strcpy( szText, strtok( 0, "$" ) );
		b++;
	}
	return szText;
}

const char* Utils::GetPartReason( BYTE b )
{
	switch ( b )
	{
	case 0:
		return "Timeout";
		break;

	case 1:
		return "Quit";
		break;

	case 2:
		return "Kicked";
		break;

	case 3:
		return "Banned";
		break;

	default:
		return "Unknown";
		break;
	}
}

const char* Utils::GetDeathReason( BYTE b )
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

const char* Utils::GetVehicleName( BYTE b )
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

const char* Utils::GetAreaName( float fX, float fY )
{
	BYTE b = 0;
	while ( b < 14 )
	{
		if ( PointInArea( fX, fY, fAreas[ b ] ) ) return szAreaNames[ b ];
		b++;
	}
	return "Vice City";
}

BYTE Utils::GetWeaponID( char* sz )
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


BYTE Utils::GetVehicleModel( char* sz )
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