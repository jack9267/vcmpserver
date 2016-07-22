#ifndef _BANLIST_H
#define _BANLIST_H

#include "Main.h"

class CBanList
{
public:
	static void							Parse							( char* sz );
	static void							Reload							( void );

	/// \summary This will clear the internal ban list aswell as the ban file
	static void							Clear							( void );

	static void							AddToBanList					( const char* sz );
	static void							RemoveFromBanList				( const char* sz );
};

#endif
