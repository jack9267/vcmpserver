/*
	SQLiteFuncs.cpp
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

	Edited: 23/3/2009

	--------------------------------------------------------------------
*/

#include "SQLiteFuncs.h"

_SQUIRRELDEF( CSQLiteFuncs::Connect )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		const SQChar* szFile = NULL;
		sq_getstring( v, 2, &szFile );

		sqlite3* pDatabase = NULL;
		unsigned char uc = sqlite3_open( (char*)szFile, &pDatabase );

		if ( uc == SQLITE_OK )
		{
			sq_pushuserpointer( v, pDatabase );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQLiteFuncs::Close )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		sqlite3* pDatabase = (sqlite3*)pPointer;
		unsigned char uc = sqlite3_close( pDatabase );

		if ( uc == SQLITE_OK )
		{
			pDatabase = NULL;

			sq_pushbool( v, true );
			return 1;
		}
	}

	sq_pushbool( v, false );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQLiteFuncs::Query )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		const SQChar* szQuery = NULL;
		sq_getstring( v, 3, &szQuery );

		sqlite3* pDatabase = (sqlite3*)pPointer;
		sqlite3_stmt* pQuery = NULL;

		const char* szTail = NULL;
		unsigned char uc = sqlite3_prepare( pDatabase, (char*)szQuery, -1, &pQuery, &szTail );

		if ( uc == SQLITE_OK )
		{
			uc = sqlite3_step( pQuery );
			
			if ( uc == SQLITE_ERROR || uc == SQLITE_MISUSE )//|| uc == SQLITE_BUSY )
			{
				// Query failed
				sqlite3_finalize( pQuery );

				sq_pushnull( v );
				return 1;
			}

			if ( uc == SQLITE_OK )
			{
				// Done query that does not return anything
				sqlite3_finalize( pQuery );

				sq_pushnull( v );
				return 1;
			}

			sq_pushuserpointer( v, pQuery );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQLiteFuncs::GetNextRow )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		sqlite3_stmt* pQuery = (sqlite3_stmt*)pPointer;
		
		if ( pQuery )
		{
			unsigned char uc = sqlite3_step( pQuery );

			// Case finished/problems/doing stuff
			if ( uc == SQLITE_DONE || uc == SQLITE_ERROR || uc == SQLITE_MISUSE || uc == SQLITE_BUSY )
			{
				sqlite3_finalize( pQuery );
				sq_pushnull( v );
				return 1;
			}

			// Case row(s) left
			sq_pushuserpointer( v, pQuery );
			return 1;
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQLiteFuncs::GetRowColumns )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		sqlite3_stmt* pQuery = (sqlite3_stmt*)pPointer;

		if ( pQuery )
		{
			sq_pushinteger( v, sqlite3_column_count( pQuery ) );
			return 1;
		}
	}

	sq_pushinteger( v, 0 );
	return 1;
}

//--------------------------------------------------

_SQUIRRELDEF( CSQLiteFuncs::GetColumnResult )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		SQUserPointer pPointer = NULL;
		sq_getuserpointer( v, 2, &pPointer );

		SQInteger iColumn = NULL;
		sq_getinteger( v, 3, &iColumn );

		sqlite3_stmt* pQuery = (sqlite3_stmt*)pPointer;
		if ( pQuery )
		{
			switch ( sqlite3_column_type( pQuery, iColumn ) )
			{
			case SQLITE_INTEGER:
				sq_pushinteger( v, (SQInteger)sqlite3_column_int( pQuery, iColumn ) );
				return 1;

			case SQLITE_FLOAT:
				sq_pushfloat( v, (SQFloat)sqlite3_column_double( pQuery, iColumn ) );
				return 1;

			case SQLITE_TEXT:
				sq_pushstring( v, (const SQChar*)sqlite3_column_text( pQuery, iColumn ), -1 );
				return 1;

			default:
				sq_pushnull( v );
				return 1;
			}
		}
	}

	sq_pushnull( v );
	return 1;
}

//--------------------------------------------------