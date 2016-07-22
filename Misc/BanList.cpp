/*
	BanList.cpp
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

#include "BanList.h"
#include "Net.h"
#include "Utils.h"

void CBanList::Parse( char* sz )
{
	char szText[ 512 ] = { NULL };

	strncpy( szText, sz, 512 );

	if ( strlen( sz ) > 0 )
	{
		strncpy( szText, strtok( sz, "\n" ), 512 );

		if ( ( szText[ 0 ] != '#' ) && ( szText[ 0 ] != 0 ) )
		{
			if ( NetInterface ) NetInterface->AddToBanList( szText );
		}
	}
}

void CBanList::Reload( void )
{
	if ( NetInterface )
	{
		NetInterface->ClearBanList();

		CConfigLoader::LoadBans();
	}
}

void CBanList::Clear( void )
{
	if ( NetInterface ) NetInterface->ClearBanList();

	FILE* bans = fopen( "bans.lst", "r" );
	if ( bans )
	{
		fclose( bans );
		remove( "bans.lst" );
	}
	// Do file wiping stuff here
}

void CBanList::AddToBanList( const char* sz )
{
	if ( NetInterface ) NetInterface->AddToBanList( sz );

	FILE* bans = fopen( "bans.lst", "a+" );
	if ( bans )
	{
		char szBan[ 32 ] = { NULL };
		sprintf( szBan, "%s\n", sz );
		fwrite( szBan, 1, strlen( szBan ), bans );

		fclose( bans );
	}
}

void CBanList::RemoveFromBanList( const char* sz )
{
	if ( NetInterface ) NetInterface->RemoveFromBanList( sz );

	FILE* bans = fopen( "bans.lst", "a+" );
	FILE* TempBans = fopen( "bans.tmp", "a+" );
	if ( ( bans ) && ( TempBans ) )
	{
		char szLine[255] = { 0 };
		char szBan[ 32 ] = { NULL };
		char szIP[ 32 ] = { NULL };
		sprintf( szIP, "%s\n", sz );

		while ( fgets( szLine, 255, bans ) )
		{
			if ( ( strlen( szLine ) > 0 ) && ( szLine[ 0 ] != '\n' ) ) 
			{
				if ( strcmp( szLine, szIP ) ) 
				{
					sprintf( szBan, "%s", szLine );
					fwrite( szBan, 1, strlen( szBan ), TempBans );
				}
			}
		}

		fclose( bans );
		fclose( TempBans );

		remove( "bans.lst" );
		rename( "bans.tmp", "bans.lst" );
	}
}