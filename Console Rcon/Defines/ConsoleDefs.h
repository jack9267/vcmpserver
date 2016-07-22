#ifndef _CONSOLEDEFS_H
#define _CONSOLEDEFS_H

#ifdef WIN32
// This is Windows
#define ConsoleColour(c) if(c) { SetConsoleTextAttribute ( GetStdHandle ( STD_OUTPUT_HANDLE ), c ); }

#define ConsoleWhite	ConsoleColour ( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY )
#define ConsoleGrey		ConsoleColour ( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE )

#define ConsoleRed		ConsoleColour ( FOREGROUND_RED | FOREGROUND_INTENSITY )
#define ConsoleGreen	ConsoleColour ( FOREGROUND_GREEN | FOREGROUND_INTENSITY )
#define ConsoleBlue		ConsoleColour ( FOREGROUND_BLUE | FOREGROUND_INTENSITY )
#define ConsoleYellow	ConsoleColour ( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY )
#define ConsoleCyan		ConsoleColour ( FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY )
#define ConsoleMagenta	ConsoleColour ( FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY )

#define ConsoleDarkGreen	ConsoleColour ( FOREGROUND_GREEN )

#else
// This is Linux 
#define ConsoleWhite	printf( "\033[0;1m" );
#define ConsoleGrey		printf( "\033[37;22m" );

#define ConsoleRed		printf( "\033[31;1m" );
#define ConsoleGreen	printf( "\033[32;1m" );
#define ConsoleBlue		printf( "\033[34;1m" );
#define ConsoleYellow	printf( "\033[33;1m" );
#define ConsoleCyan		printf( "\033[36;1m" );
#define ConsoleMagenta	printf( "\033[34;1m" );

#define ConsoleDarkGreen	printf( "\033[32;22m" );

#endif

#define MAX_CONSOLELINE_LEN 256

enum CONSOLE_COLOURS
{
	COLOUR_WHITE,
	COLOUR_GREY,
	COLOUR_RED,
	COLOUR_GREEN,
	COLOUR_BLUE,
	COLOUR_YELLOW,
	COLOUR_CYAN,
	COLOUR_MAGENTA,
	COLOUR_DARKGREEN
};

#endif
