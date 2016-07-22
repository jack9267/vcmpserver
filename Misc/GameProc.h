#ifndef _GAMEPROC_H
#define _GAMEPROC_H

class CGameProc
{
public:
	static void								PickupRespawn							( void );
	static void								VehicleRespawn							( void );

	static void								TimeAndWeatherSync						( void );

private:
	static unsigned int						m_uiLastTimeSync;
	static unsigned int						m_uiLastWeatherSync;
};

#endif
