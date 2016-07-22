/*
 * KeyBinds.h
 * Edited: 16/3/2008
 */


#ifndef _KEYBINDS_H
#define _KEYBINDS_H

//#include "SquirrelMain.h"
#include "Pools.h"


#define KEY_ONFOOT_AIM				1
#define KEY_ONFOOT_WEPNEXT			2
#define KEY_ONFOOT_WEPPREV			4
#define KEY_ONFOOT_CROUCH			256
#define KEY_ONFOOT_SPRINT			1024
#define KEY_ONFOOT_JUMP				2048
#define KEY_ONFOOT_RIGHT			4096
#define KEY_ONFOOT_LEFT				8192
#define KEY_ONFOOT_BACKWARD			16384
#define KEY_ONFOOT_FORWARD			32768
#define KEY_AIM_FIRE				512
#define KEY_INVEHICLE_HANDBRAKE		1
#define KEY_INVEHICLE_LOOKLEFT		2
#define KEY_INVEHICLE_LOOKRIGHT		4
#define KEY_INVEHICLE_LEANDOWN		8
#define KEY_INVEHICLE_LEANUP		16
#define KEY_INVEHICLE_HORN			256
#define KEY_INVEHICLE_FIRE			512
#define KEY_INVEHICLE_THROTTLE		1024
#define KEY_INVEHICLE_BRAKE			2048
#define KEY_INVEHICLE_RIGHT			4096
#define KEY_INVEHICLE_LEFT			8192

enum eKeyIDs
{
	ONFOOT_AIM = 0,
	ONFOOT_WEPNEXT,
	ONFOOT_WEPPREV,
	ONFOOT_CROUCH,
	ONFOOT_SPRINT,
	ONFOOT_JUMP,
	ONFOOT_RIGHT,
	ONFOOT_LEFT,
	ONFOOT_BACKWARD,
	ONFOOT_FORWARD,
	AIM_FIRE,
	INVEHICLE_HANDBRAKE,
	INVEHICLE_LOOKLEFT,
	INVEHICLE_LOOKRIGHT,
	INVEHICLE_LEANDOWN,
	INVEHICLE_LEANUP,
	INVEHICLE_HORN,
	INVEHICLE_FIRE,
	INVEHICLE_THROTTLE,
	INVEHICLE_BRAKE,
	INVEHICLE_RIGHT,
	INVEHICLE_LEFT
};

#define BINDABLE_KEYS				22
#define MAX_BIND_PARAMS				25

#define ONFOOT_KEYS					0  // Starting point of on foot keys
#define VEHICLE_KEYS				11 // Starting point of vehicle keys
#define ONFOOT_KEYS_END				11
#define VEHICLE_KEYS_END			22

#define INVALID_KEY					0
#define INVALID_KEY_ID				255
#define INVALID_KEY_TYPE			-1


// 0: no change, 1: key pressed down, 2: key lifted
#define KEY_STATE_CHANGED( key, oldKeys, newKeys )	( (oldKeys & key) != (newKeys & key) ? ( (oldKeys & key) ? 2 : 1 ) : 0 )


union pBindParams
{
	SQInteger i;
	SQFloat f;
	SQUserPointer* p;
	const SQChar* sz;
};


class CKeyBind
{
public:
	CKeyBind( unsigned char ucKey );
	~CKeyBind();

	unsigned char			GetKey					( void )							{ return m_ucKey; }
	void					SetKey					( unsigned char uc )				{ m_ucKey = uc; }

	char*					GetFunc					( void )							{ return m_szFunc; }
	void					SetFunc					( const char* szFunc )					{ strncpy( m_szFunc, szFunc, 128 ); }

	bool					GetDown					( void )							{ return m_bDown; }
	void					SetDown					( bool bDown )						{ m_bDown = bDown; }

	void					SetParamInt				( SQInteger i );
	void					SetParamFloat			( SQFloat f );
	void					SetParamString			( const SQChar* sz );
	void					SetParamPointer			( SQUserPointer p );

	unsigned char			GetParamCount			( void )							{ return m_ucParams; }

	unsigned char			GetScript				( void )							{ return m_ucScript; }
	void					SetScript				( unsigned char uc )				{ m_ucScript = uc; }

	void					Call					( CPlayerPool* pPlayer );

private:
	unsigned int m_ucKey;
	bool m_bDown;
	char m_szFunc[ 128 ];
	unsigned char m_ucParams;
	unsigned char m_ucScript;

	pBindParams m_params[ MAX_BIND_PARAMS ];
	enumTypes m_paramTypes[ MAX_BIND_PARAMS ];
};


class CKeyBinds
{
public:
	static CKeyBind*	New					( unsigned char ucKey );
	static bool			Remove				( unsigned char ucKey );

	static CKeyBind*	Find				( unsigned char ucKey );

	static void			PlayerKeySync		( CPlayerPool* pPlayer, WORD wOldKeys, WORD wNewKeys );
	static void			AimKeySync			( CPlayerPool* pPlayer, WORD wOldKeys, WORD wNewKeys );
	static void			VehicleKeySync		( CPlayerPool* pPlayer, WORD wOldKeys, WORD wNewKeys );

	static void			CallKeyBind			( CPlayerPool* pPlayer, unsigned char ucKeyCode, unsigned char ucState );

private:
	static CKeyBind*	m_KeyBinds[ BINDABLE_KEYS ];
};

#endif