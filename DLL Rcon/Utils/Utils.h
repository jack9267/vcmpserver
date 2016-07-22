#ifndef _UTILS_H
#define _UTILS_H

#include "Main.h"

class Utils
{
public:
	static unsigned char						NumTok							( char* szText, char* szDelim );
	static bool									IsNum							( char* sz );
	static bool									IsIn							( char* sz1, char* sz2 );
	static char*								Strip							( char* szText );

	static float								CalcSpeed						( float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2 );
	static bool									PointInArea						( float fX, float fY, const double fArea[ 8 ] );

	static const char*							GetPartReason					( BYTE b );
	static const char*							GetDeathReason					( BYTE b );
	static const char*							GetVehicleName					( BYTE b );
	static const char*							GetAreaName						( float fX, float fY );

	static BYTE									GetWeaponID						( char* sz );
	static BYTE									GetVehicleModel					( char* sz );
};

#endif