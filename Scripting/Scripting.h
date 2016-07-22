#ifndef _SCRIPTING_H
#define _SCRIPTING_H

#include "SquirrelMain.h"

struct ScriptCall
{
	char szFunction[ 64 ];
	ScriptTypes pType[ 32 ];
	ScriptParams pParams[ 32 ];
	unsigned int uiParams;
};

class CScripting
{
public:
	static void							PushFunction					( const char* sz );

	static void							PushInteger						( int i );
	static void							PushFloat						( float f );
	static void							PushString						( const char* sz );
	static void							PushBool						( bool b );
	static void							PushPointer						( void* p );
	static void							PushPlayerPointer				( CPlayerPool* p );
	static void							PushVehiclePointer				( CVehiclePool* p );
	static void							PushPickupPointer				( CPickupPool* p );
	static void							PushNull						( void );

	static bool							CallFunction					( void );

private:
	static ScriptCall*					m_pScriptCall;
};

#endif
