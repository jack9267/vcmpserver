/*
	SQKeyBindFuncs.cpp
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


#include "SQKeyBindFuncs.h"
#include "KeyBinds.h"
#include "Pools.h"


_SQUIRRELDEF( CSQKeyBindFuncs::BindKey )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs > 2 )
	{
		SQInteger iKey = 0;
		sq_getinteger( v, 2, &iKey );

		SQBool bState = false;
		sq_getbool( v, 3, &bState );

		const SQChar* szFunction = NULL;
		sq_getstring( v, 4, &szFunction );

		unsigned char ucKey = (unsigned char)iKey;
		if ( ucKey < BINDABLE_KEYS )
		{
			if ( !CKeyBinds::Find( ucKey ) )
			{
				CKeyBind* pBind = CKeyBinds::New( ucKey );

				pBind->SetDown( bState );
				pBind->SetFunc( (char*)szFunction );

				SQInteger i = 0;
				SQFloat f = 0.0f;
				const SQChar* sz = NULL;
				SQUserPointer p = NULL;

				unsigned char uc1 = 0;

				for ( unsigned char uc = 5, uc1 = 0; uc <= iArgs; uc++ )
				{
					if ( uc - 5 < MAX_BIND_PARAMS )
					{
						switch( sq_gettype( v, uc ) )
						{
							case OT_INTEGER:	
							
								sq_getinteger( v, uc, &i );
								pBind->SetParamInt( i );

								break;

							case OT_FLOAT:
							
								sq_getfloat( v, uc, &f );
								pBind->SetParamFloat( f );

								break;

							case OT_STRING:

								sq_getstring( v, uc, &sz );
								pBind->SetParamString( sz );

								break;

							case OT_USERPOINTER:
							
								sq_getuserpointer( v, uc, &p );
								pBind->SetParamPointer( p );

								break;
						}
						uc1++;
					}
				}

				CSquirrel* pSquirrel = CSquirrelManager::Find( v );
				if ( pSquirrel )
				{
					pBind->SetScript( pSquirrel->GetID() );
				}

				sq_pushuserpointer( v, (SQUserPointer)pBind );
				return 1;
			}
		}
		
	}
	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQKeyBindFuncs::RemoveBind )
{
	SQInteger iArgs = sq_gettop( v );
	if ( iArgs == 2 )
	{
		SQInteger iKey = NULL;
		sq_getinteger( v, 2, &iKey );

		CKeyBind* pBind = CKeyBinds::Find( (unsigned char)iKey );

		if ( pBind )
		{			
			sq_pushbool( v, (SQBool)CKeyBinds::Remove( pBind->GetKey() ) );
			return 1;
		}

	}
	sq_pushbool( v, (SQBool)false );
	return 1;
}