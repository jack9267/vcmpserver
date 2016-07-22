/*
	KeyBinds.cpp
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

	Edited: 16/3/2008

	--------------------------------------------------------------------
*/


#include "KeyBinds.h"


typedef enum __eKeyType
{
    KEY_ONFOOT,
    KEY_AIM,
    KEY_INVEHICLE,
	KEY_32BIT_PADDING
} eKeyType;

struct SBindableKeys
{
	char szKeyName[ 15 ];
	int iKeyValue;
	eKeyType keyType;
};



SBindableKeys bindableKeys[] =
{
	{ "aim",			KEY_ONFOOT_AIM,				KEY_ONFOOT },
	{ "nextwep",		KEY_ONFOOT_WEPNEXT,			KEY_ONFOOT },
	{ "prevwep",		KEY_ONFOOT_WEPPREV,			KEY_ONFOOT },
	{ "crouch",			KEY_ONFOOT_CROUCH,			KEY_ONFOOT },
	{ "sprint",			KEY_ONFOOT_SPRINT,			KEY_ONFOOT }, // also "zoomout"
	{ "jump",			KEY_ONFOOT_JUMP,			KEY_ONFOOT }, // also "zoomin"
	{ "right",			KEY_ONFOOT_RIGHT,			KEY_ONFOOT }, // also "aimright"
	{ "left",			KEY_ONFOOT_LEFT,			KEY_ONFOOT }, // also "aimleft"
	{ "backward",		KEY_ONFOOT_BACKWARD,		KEY_ONFOOT }, // also "aimdown"
	{ "forward",		KEY_ONFOOT_FORWARD,			KEY_ONFOOT }, // also "aimup"
	{ "fire",			KEY_AIM_FIRE,				KEY_AIM },
	{ "handbrake",		KEY_INVEHICLE_HANDBRAKE,	KEY_INVEHICLE },
	{ "veh_lookleft",	KEY_INVEHICLE_LOOKLEFT,		KEY_INVEHICLE },
	{ "veh_lookright",	KEY_INVEHICLE_LOOKRIGHT,	KEY_INVEHICLE },
	{ "leandown",		KEY_INVEHICLE_LEANDOWN,		KEY_INVEHICLE },
	{ "leanup",			KEY_INVEHICLE_LEANUP,		KEY_INVEHICLE },
	{ "horn",			KEY_INVEHICLE_HORN,			KEY_INVEHICLE },
	{ "veh_fire",		KEY_INVEHICLE_FIRE,			KEY_INVEHICLE },
	{ "throttle",		KEY_INVEHICLE_THROTTLE,		KEY_INVEHICLE },
	{ "brake",			KEY_INVEHICLE_BRAKE,		KEY_INVEHICLE },
	{ "veh_right",		KEY_INVEHICLE_RIGHT,		KEY_INVEHICLE },
	{ "veh_left",		KEY_INVEHICLE_LEFT,			KEY_INVEHICLE },
	{ "",				(eKeyType)INVALID_KEY,		(eKeyType)INVALID_KEY_TYPE }
	
};


//--------------------------------------------------

CKeyBind* CKeyBinds::m_KeyBinds[ BINDABLE_KEYS ] = { 0 };

//--------------------------------------------------

CKeyBind::CKeyBind( unsigned char ucKey )
{
	m_ucKey = ucKey;
	
	m_ucParams = 0;
}

//--------------------------------------------------

CKeyBind::~CKeyBind()
{
}

//--------------------------------------------------

void CKeyBind::SetParamInt( SQInteger i )
{
	m_params[ m_ucParams ].i = i;
	m_paramTypes[ m_ucParams ] = eInt;
	m_ucParams++;
}

//--------------------------------------------------

void CKeyBind::SetParamFloat( SQFloat f )
{
	m_params[ m_ucParams ].f = f;
	m_paramTypes[ m_ucParams ] = eFloat;
	m_ucParams++;
}

//--------------------------------------------------

void CKeyBind::SetParamString( const SQChar* sz )
{
	m_params[ m_ucParams ].sz = sz;
	m_paramTypes[ m_ucParams ] = eString;
	m_ucParams++;
}

//--------------------------------------------------

void CKeyBind::SetParamPointer( SQUserPointer p )
{
	m_params[ m_ucParams ].p = (SQUserPointer*)p;
	m_paramTypes[ m_ucParams ] = eUserData;
	m_ucParams++;
}

//--------------------------------------------------

void CKeyBind::Call( CPlayerPool* pPlayer )
{
	unsigned char uc = 0;

	CSquirrel* pScript = CSquirrelManager::Find( GetScript() );
	if ( pScript )
	{

		pScript->PushFunction( GetFunc() );
		pScript->PushPlayerPointer( pPlayer );
		
		if ( GetParamCount() )
		{
			while ( uc < GetParamCount() )
			{
				switch ( m_paramTypes[ uc ] )
				{
					case eInt:
						
						pScript->PushInteger( m_params[ uc ].i );
						break;

					case eFloat:
						
						pScript->PushFloat( m_params[ uc ].f );
						break;

					case eString:
						
						pScript->PushString( (char*)m_params[ uc ].sz );
						break;

					case eUserData:
						
						pScript->PushPointer( m_params[ uc ].p );
						break;

				}
				uc++;
			}
		}
		pScript->CallFunction();
	}
}

//--------------------------------------------------

CKeyBind* CKeyBinds::New( unsigned char ucKey )
{
	if ( ( ucKey < BINDABLE_KEYS ) && ( strcmp( bindableKeys[ ucKey ].szKeyName, "" ) ) )
	{
		if ( !m_KeyBinds[ ucKey ] )
		{
			CKeyBind* pBind = new CKeyBind( ucKey );
			m_KeyBinds[ ucKey ] = pBind;
			return pBind;
		}

	}
	return NULL;
}

//--------------------------------------------------

CKeyBind* CKeyBinds::Find( unsigned char ucKey )
{
	if ( m_KeyBinds[ ucKey ] )
	{
		CKeyBind* pBind = m_KeyBinds[ ucKey ];
		return pBind;
	}
	return NULL;
}

//--------------------------------------------------

bool CKeyBinds::Remove( unsigned char ucKey )
{
	CKeyBind* pBind = m_KeyBinds[ ucKey ];
		
	if ( pBind )
	{
		m_KeyBinds[ ucKey ] = NULL;
		delete pBind;

		return true;
	}

	return false;
}

//--------------------------------------------------

void CKeyBinds::PlayerKeySync( CPlayerPool* pPlayer, WORD wOldKeys, WORD wNewKeys )
{
	unsigned char ucKeyState; 

	for ( unsigned char i = ONFOOT_KEYS; i < ONFOOT_KEYS_END; i++ )
	{
		ucKeyState = KEY_STATE_CHANGED( bindableKeys[ i ].iKeyValue, wOldKeys, wNewKeys );
		if ( ucKeyState ) CallKeyBind( pPlayer, i, ucKeyState );
	}
}

//--------------------------------------------------

void CKeyBinds::AimKeySync( CPlayerPool* pPlayer, WORD wOldKeys, WORD wNewKeys )
{
	unsigned char ucKeyState; 

	ucKeyState = KEY_STATE_CHANGED( KEY_AIM_FIRE, wOldKeys, wNewKeys );
	if ( ucKeyState ) CallKeyBind( pPlayer, 10, ucKeyState );
}

//--------------------------------------------------

void CKeyBinds::VehicleKeySync( CPlayerPool* pPlayer, WORD wOldKeys, WORD wNewKeys )
{
	unsigned char ucKeyState; 

	for ( unsigned char i = VEHICLE_KEYS; i < VEHICLE_KEYS_END; i++ )
	{
		ucKeyState = KEY_STATE_CHANGED( bindableKeys[ i ].iKeyValue, wOldKeys, wNewKeys );
		if ( ucKeyState ) CallKeyBind( pPlayer, i, ucKeyState );
	}
}

//--------------------------------------------------

void CKeyBinds::CallKeyBind( CPlayerPool* pPlayer, unsigned char ucKeyCode, unsigned char ucState )
{
	CScripting::PushFunction( "onPlayerKeyStateChange" );
	CScripting::PushPlayerPointer( pPlayer );
	CScripting::PushInteger( ucKeyCode );
	CScripting::PushBool( ucState == 1 ? true : false );
	CScripting::CallFunction();

	CKeyBind* pBind = m_KeyBinds[ ucKeyCode ];
	
	if ( pBind )
	{
		if ( ( pBind->GetDown() && ucState == 1 ) || ( !pBind->GetDown() && ucState == 2 ) )
		{
			pBind->Call( pPlayer );
		}
	}
}

//--------------------------------------------------