/*
	SQIniFuncs.cpp
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

#include "SQIniFuncs.h"
#include "Utils.h"

_SQUIRRELDEF( CSQIniFuncs::ReadString )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szValue, szBuf );

		const char* szReturn = iniFile.getStringValue( szBuf );

		sq_pushstring( v, (SQChar*)szReturn, -1 );

		return 1;
	}

	sq_pushstring( v, "", -1 );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::ReadInteger )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szValue, szBuf );

		int iRtn = iniFile.getIntValue( szBuf );

		sq_pushinteger( v, (SQInteger)iRtn );

		return 1;
	}

	sq_pushinteger( v, 0 );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::ReadNumber )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szValue, szBuf );

		double dReturn = iniFile.getDoubleValue( szBuf );

		sq_pushfloat( v, (SQFloat)dReturn );

		return 1;
	}

	sq_pushfloat( v, 0.0 );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::ReadBool )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szValue, szBuf );

		bool bReturn = iniFile.getBoolValue( szBuf );

		sq_pushbool( v, (SQBool)bReturn );

		return 1;
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::WriteString )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 5 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szName = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szName );
		sq_getstring( v, 5, &szValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szName, szBuf );

		char szBuf2[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szValue, szBuf2 );

		iniFile.setStringValue( szBuf, szBuf2 );

		iniFile.save( (char*)szFile );

		sq_pushbool( v, true );

		return 1;
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::WriteInteger )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 5 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szName = NULL;
		SQInteger iValue = 0;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szName );
		sq_getinteger( v, 5, &iValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szName, szBuf );

		iniFile.setIntValue( szBuf, (int)iValue );

		iniFile.save( (char*)szFile );

		sq_pushbool( v, true );

		return 1;
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::WriteNumber )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 5 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szName = NULL;
		SQFloat fValue = 0;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szName );
		sq_getfloat( v, 5, &fValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szName, szBuf );

		iniFile.setDoubleValue( szBuf, (double)fValue );

		iniFile.save( (char*)szFile );

		sq_pushbool( v, true );

		return 1;
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::WriteBool )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 5 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szName = NULL;
		SQBool bValue = 0;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szName );
		sq_getbool( v, 5, &bValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szName, szBuf );

		iniFile.setBoolValue( szBuf, (bool)bValue );

		iniFile.save( (char*)szFile );

		sq_pushbool( v, true );

		return 1;
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::RemoveValue )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 4 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );
		sq_getstring( v, 4, &szValue );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		char szBuf[ 256 ] = { NULL };
		CUtils::FixIniBug( (char*)szValue, szBuf );

		iniFile.deleteData( szBuf );

		iniFile.save( (char*)szFile );

		sq_pushbool( v, true );
		return 1;
	}

	sq_pushbool( v, false );
	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::CountSection )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.setSection( (char*)szSection );

		int iReturn = iniFile.getNumDataMembers();

		sq_pushinteger( v, (SQInteger)iReturn );

		return 1;
	}

	sq_pushinteger( v, 0 );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::DeleteSection )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 3 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );
		sq_getstring( v, 3, &szSection );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.deleteSection( (char*)szSection );

		iniFile.save( (char*)szFile );

		sq_pushbool( v, true );

		return 1;
	}

	sq_pushbool( v, false );

	return 1;
}

_SQUIRRELDEF( CSQIniFuncs::Clear )
{
	SQInteger iArgs = sq_gettop( v );

	if ( iArgs == 2 )
	{
		const SQChar* szFile = NULL;
		const SQChar* szSection = NULL;
		const SQChar* szValue = NULL;

		sq_getstring( v, 2, &szFile );

		Config iniFile;

		iniFile.load( (char*)szFile );

		iniFile.clear();

		iniFile.save( (char*)szFile );

		sq_pushbool( v, true );

		return 1;
	}

	sq_pushbool( v, false );

	return 1;
}