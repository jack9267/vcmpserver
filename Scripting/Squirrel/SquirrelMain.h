#ifndef _SQUIRREL_H
#define _SQUIRREL_H

#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdaux.h>
#include <sqstdblob.h>
#include "SquirrelObjects.h"
#include "sqplus.h"
#include "sqlite3.h"
#include "tinyxml.h"

#ifdef SQUNICODE
#define scvprintf vwprintf
#else
#define scvprintf vprintf
#endif

#ifndef _SQUIRRELDEF
#define _SQUIRRELDEF(x) SQInteger x( HSQUIRRELVM v )
#endif

using namespace SqPlus;

#ifndef MAX_SCRIPTS
	#define MAX_SCRIPTS 50
#endif

void Print( HSQUIRRELVM v, const SQChar *s, ... );
void Error( HSQUIRRELVM v, const SQChar *s, ... );

class CPlayerPool;
class CVehiclePool;
class CPickupPool;
class CSquirrelTimers;
class CSquirrelSockets;

typedef enum _ScriptTypes
{
	ST_STRING,
	ST_INTEGER,
	ST_FLOAT,
	ST_BOOL,
	ST_POINTER,
	ST_PLAYER,
	ST_VEHICLE,
	ST_PICKUP,
	ST_SOCKET,
	ST_TIMER,
	ST_NULL
} ScriptTypes;

typedef union _ScriptParams
{
	char szString[ 256 ];
	void* p;
	int i;
	float f;
	bool b;
} ScriptParams;

struct SquirrelRetn
{
	ScriptTypes pType;
	ScriptParams pValue;
};

struct Vector
{
	float x,y,z;
	Vector() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {};
	Vector( float fX, float fY, float fZ ) : x( fX ), y( fY ), z( fZ ) {}

	bool operator==( Vector const& src ) const
	{
		return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
	}

	bool operator!=( Vector const& src ) const
	{
		return ( src.x != x ) || ( src.y != y ) || ( src.z != z );
	}

	bool operator>( Vector const& src ) const
	{
		return ( src.x > x ) && ( src.y > y ) && ( src.z > z );
	}

	bool operator<( Vector const& src ) const
	{
		return ( src.x < x ) && ( src.y < y ) && ( src.z < z );
	}

	Vector operator+( const Vector& v )
	{
		Vector result;
		result.x = ( x + v.x ); result.y = ( y + v.y ); result.z = ( z + v.z );
		return result;
	}

	Vector operator-( const Vector& v )
	{
		Vector result;
		result.x = ( x - v.x ); result.y = ( y - v.y );	result.z = ( z - v.z );
		return result;
	}

	Vector operator+( const float& v )
	{
		Vector result;
		result.x = ( x + v ); result.y = ( y + v );	result.z = ( z + v );
		return result;
	}

	Vector operator-( const float& v )
	{
		Vector result;
		result.x = ( x - v ); result.y = ( y - v );	result.z = ( z - v );
		return result;
	}

	Vector operator*( float fl )
	{
		Vector result;
		result.x = ( x * fl ); result.y = ( y * fl ); result.z = ( z * fl );
		return result;
	}

	Vector operator*( Vector const& v )
	{
		Vector result;
		result.x = ( x * v.x ); result.y = ( y * v.y );	result.z = ( z * v.z );
		return result;
	}

	Vector operator/( float fl )
	{
		float oofl = 1.0f / fl;
		Vector result;
		result.x = ( x * oofl ); result.y = ( y * oofl); result.z = ( z * oofl);
		return result;
	}

	Vector operator/( Vector const& v )
	{
		Vector result;
		result.x = ( x / v.x ); result.y = ( y / v.y );	result.z = ( z / v.z );
		return result;
	}

	Vector operator+( const Vector& v )	const
	{
		Vector result;
		result.x = ( x + v.x ); result.y = ( y + v.y ); result.z = ( z + v.z );
		return result;
	}

	Vector operator-( const Vector& v ) const
	{
		Vector result;
		result.x = ( x - v.x ); result.y = ( y - v.y );	result.z = ( z - v.z );
		return result;
	}

	Vector operator*( float fl ) const
	{
		Vector result;
		result.x = ( x * fl ); result.y = ( y * fl ); result.z = ( z * fl );
		return result;
	}

	bool IsZero ( void ) const
	{
		return ( x == 0.0f && y == 0.0f && z == 0.0f );
	}
};
 
_DECL_CLASS( Vector );
_DECL_NATIVE_CONSTRUCTION( Vector, Vector );

_DECL_CLASS( Player );
_DECL_NATIVE_CONSTRUCTION( Player, CPlayerPool );
_DECL_CLASS( Vehicle );
_DECL_NATIVE_CONSTRUCTION( Vehicle, CVehiclePool );
_DECL_CLASS( Pickup );
_DECL_NATIVE_CONSTRUCTION( Pickup, CPickupPool );
_DECL_CLASS( Timer );
_DECL_NATIVE_CONSTRUCTION( Timer, CSquirrelTimers );
_DECL_CLASS( Socket );
_DECL_NATIVE_CONSTRUCTION( Socket, CSquirrelSockets );
_DECL_CLASS( XmlNode );
_DECL_NATIVE_CONSTRUCTION( XmlNode, TiXmlNode );
_DECL_CLASS( XmlDocument );
_DECL_NATIVE_CONSTRUCTION( XmlDocument, TiXmlDocument );
_DECL_CLASS( XmlElement );
_DECL_NATIVE_CONSTRUCTION( XmlElement, TiXmlElement );


class CSquirrel
{
public:
	CSquirrel( unsigned char uc, const char* sz );
	~CSquirrel(void);

	unsigned char							GetID					( void )						{ return m_ucID; }
	void									SetID					( unsigned char uc )			{ m_ucID = uc; }

	char*									GetScriptName			( void )						{ return m_szFilename; }
	void									SetScriptName			( const char* sz )					{ strncpy( m_szFilename, sz, 32 ); }

	HSQUIRRELVM								GetVM					( void )						{ return m_pVM; }

	struct SquirrelVMSys					GetVMSys				( void )						{ return m_pVMSys; }

	CSquirrelObjects*						GetObjects				( void )						{ return m_pObjects; }

	bool									GetContinueProcessing	( void )						{ return m_bContinueProcessing; }
	void									SetContinueProcessing	( bool b )						{ m_bContinueProcessing = b; }

	void									PushFunction			( const char* sz );

	void									PushInteger				( int i );
	void									PushFloat				( float f );
	void									PushString				( const char* sz );
	void									PushBool				( bool b );
	void									PushPointer				( void* p );
	void									PushPlayerPointer		( CPlayerPool* p );
	void									PushVehiclePointer		( CVehiclePool* p );
	void									PushPickupPointer		( CPickupPool* p );
	void									PushSocketPointer		( CSquirrelSockets* p );
	void									PushNull				( void );

	bool									CallFunction			( void );
	SquirrelRetn							CallFunctionAndRetn		( void );

	void									ProcessQueue			( void );

private:
	unsigned char							m_ucID;
	char									m_szFilename[ 32 ];

	bool									m_bWaiting;
	bool									m_bExecuting;
	bool									m_bContinueProcessing;
	unsigned char							m_ucParams;
	int										m_iOrigStack;

	struct SquirrelVMSys					m_pVMSys;
	HSQUIRRELVM								m_pVM;

	CSquirrelObjects*						m_pObjects;
};

class CSquirrelManager
{
public:
	static CSquirrel*						New						( const char* sz );
	static CSquirrel*						New						( unsigned char uc, const char* sz );

	static CSquirrel*						Find					( unsigned char uc );
	static CSquirrel*						Find					( const char* sz );
	static CSquirrel*						Find					( HSQUIRRELVM v );

	static bool								Remove					( CSquirrel* p );
	static bool								Remove					( unsigned char uc );
	static bool								Remove					( const char* sz );

	static void								RemoveAll				( void );

	static unsigned char					Count					( void )						{ return m_ucScripts; }

	static unsigned char					FindFreeID				( void );

	// Objects System
	static void								RemoveTimer				( CSquirrelTimers* p );

private:
	static unsigned char					m_ucScripts;
	static CSquirrel*						m_Scripts[ MAX_SCRIPTS ];
};

#endif
