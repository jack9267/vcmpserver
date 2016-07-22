#ifndef _SPAWNPOOL_H
#define _SPAWNPOOL_H

#include "Main.h"

class CSpawnPool
{
public:
	CSpawnPool( unsigned char uc );
	~CSpawnPool(void);

	unsigned char						GetID							( void )							{ return m_ucID; }

	unsigned char						GetSkin							( void )							{ return m_ucSkin; }
	void								SetSkin							( unsigned char uc )				{ m_ucSkin = uc; }

	unsigned char						GetTeam							( void )							{ return m_ucTeam; }
	void								SetTeam							( unsigned char uc )				{ m_ucTeam = uc; }

	float								GetX							( void )							{ return m_fX; }
	void								SetX							( float f )							{ m_fX = f; }

	float								GetY							( void )							{ return m_fY; }
	void								SetY							( float f )							{ m_fY = f; }

	float								GetZ							( void )							{ return m_fZ; }
	void								SetZ							( float f )							{ m_fZ = f; }

	float								GetAngle						( void )							{ return m_fAngle; }
	void								SetAngle						( float f )							{ m_fAngle = f; }

	unsigned int						GetWeapon						( unsigned char uc )				{ return m_iWeapons[ uc ]; }
	void								SetWeapon						( unsigned char uc, unsigned int ui )	{ m_iWeapons[ uc ] = ui; }

	unsigned int						GetWeaponAmmo					( unsigned char uc )				{ return m_iWeaponAmmo[ uc ]; }
	void								SetWeaponAmmo					( unsigned char uc, unsigned int ui )	{ m_iWeaponAmmo[ uc ] = ui; }


private:
	unsigned char						m_ucID;

	unsigned char						m_ucSkin;
	unsigned char						m_ucTeam;

	float								m_fX, m_fY, m_fZ;
	float								m_fAngle;

	unsigned int						m_iWeapons[ 3 ];
	unsigned int						m_iWeaponAmmo[ 3 ];
};

// The Manager
class CSpawnPoolManager
{
public:
	static CSpawnPool*					New								( unsigned char uc );

	static CSpawnPool*					Find							( unsigned char uc );
	static CSpawnPool*					FindTeam						( unsigned char uc );

	static bool							Remove							( CSpawnPool* p );
	static bool							Remove							( unsigned char uc );

	static void							RemoveAll						( void );

	static unsigned char				Count							( void )							{ return m_ucSpawns; }

	static void							ProcessAll						( void );

private:
	static unsigned char				m_ucSpawns;
	static CSpawnPool*					m_SpawnPool[ MAX_SPAWNS ];
};

#endif
