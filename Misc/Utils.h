#ifndef _UTILS_H
#define _UTILS_H

#include "Main.h"

struct AreaPoints
{
	float x, y;

	AreaPoints( void ) : x( 0.0f ), y( 0.0f ) {};
	AreaPoints( float fX, float fY ) : x( fX ), y( fY ) {};
};

class CUtils
{
public:
	static unsigned char			NumTok						( char* szText, char* szDelimiter );
	static bool						IsIn						( const char* sz1, const char* sz2 );

	static bool						IsNum						( const char* sz );

	static float					DistanceFromPoint			( float fX1, float fY1, float fX2, float fY2 );
	static void						RandomString				( char* sz );
	static void						FixIniBug					( const char* sz, char* szBuffer );

	static bool						InPoly						( float fX, float fY,const unsigned int uiPoints, const AreaPoints fPoints[] );

	// Stolen from the DLL
	static bool						PointInArea					( float fX, float fY, const double fArea[ 8 ] );

	static const char*				GetDeathReason				( BYTE b );
	static const char*				GetVehicleName				( BYTE b );
	static const char*				GetAreaName					( float fX, float fY );

	static BYTE						GetWeaponID					( const char* sz );
	static BYTE						GetVehicleModel				( const char* sz );

	static const char*				GetSkinName					( unsigned char uc );
};

#endif
