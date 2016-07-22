#include "Net.h"

unsigned int CGameSettings::m_uiLastTimeSync = RakNet::GetTime();

void CGameSettings::TimeUpdate( void )
{
	unsigned int uiTime = RakNet::GetTime();

	if ( CGameSettings::GetTimeRate() )
	{
		if ( ( uiTime - m_uiLastTimeSync ) >= (unsigned int)( ( 1000 / 8 ) * CGameSettings::GetTimeRate() ) )
		{
			unsigned char ucHour = CGameSettings::GetHour(), ucMinute = CGameSettings::GetMinute();

			if ( ucMinute == 59 ) ucMinute = 0;
			else ucMinute++;

			CGameSettings::SetMinute( ucMinute );

			m_uiLastTimeSync = uiTime;
		}
	}
}