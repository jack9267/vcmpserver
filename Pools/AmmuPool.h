#ifndef _AMMUPOOL_H
#define _AMMUPOOL_H

#include "Main.h"

#ifndef MAX_AMMUS
	#define MAX_AMMUS 10
#endif

class CAmmuPool
{
public:
	CAmmuPool( unsigned char uc );
	~CAmmuPool(void);

	unsigned char						GetID							( void )							{ return m_ucID; }

	float								GetX							( void )							{ return m_fX; }
	void								SetX							( float f )							{ m_fX = f; }

	float								GetY							( void )							{ return m_fY; }
	void								SetY							( float f )							{ m_fY = f; }

	float								GetZ							( void )							{ return m_fZ; }
	void								SetZ							( float f )							{ m_fZ = f; }

	float								GetCameraX						( void )							{ return m_fCameraX; }
	void								SetCameraX						( float f )							{ m_fCameraX = f; }

	float								GetCameraY						( void )							{ return m_fCameraY; }
	void								SetCameraY						( float f )							{ m_fCameraY = f; }

	float								GetCameraZ						( void )							{ return m_fCameraZ; }
	void								SetCameraZ						( float f )							{ m_fCameraZ = f; }

	float								GetCameraAngleX					( void )							{ return m_fCameraAngleX; }
	void								SetCameraAngleX					( float f )							{ m_fCameraAngleX = f; }

	float								GetCameraAngleY					( void )							{ return m_fCameraAngleY; }
	void								SetCameraAngleY					( float f )							{ m_fCameraAngleY = f; }

	float								GetCameraAngleZ					( void )							{ return m_fCameraAngleZ; }
	void								SetCameraAngleZ					( float f )							{ m_fCameraAngleZ = f; }

	float								GetWeaponX						( void )							{ return m_fWeaponX; }
	void								SetWeaponX						( float f )							{ m_fWeaponX = f; }

	float								GetWeaponY						( void )							{ return m_fWeaponY; }
	void								SetWeaponY						( float f )							{ m_fWeaponY = f; }

	float								GetWeaponZ						( void )							{ return m_fWeaponZ; }
	void								SetWeaponZ						( float f )							{ m_fWeaponZ = f; }

	unsigned int						GetWeapon						( BYTE b )							{ return m_uiWeapon[ b ]; }
	void								SetWeapon						( BYTE b, unsigned int uiWeapon )	{ m_uiWeapon[ b ] = uiWeapon; }

	unsigned int						GetWeaponCost					( BYTE b )							{ return m_uiWeaponCost[ b ]; }
	void								SetWeaponCost					( BYTE b, unsigned int ui )			{ m_uiWeaponCost[ b ] = ui; }

	bool								GetWeaponEnabled				( BYTE b )							{ return m_bWeaponEnabled[ b ]; }
	void								SetWeaponEnabled				( BYTE b, bool bEnabled )			{ m_bWeaponEnabled[ b ] = bEnabled; }

private:
	unsigned char						m_ucID;

	float								m_fX, m_fY, m_fZ;
	float								m_fCameraX, m_fCameraY, m_fCameraZ;
	float								m_fCameraAngleX, m_fCameraAngleY, m_fCameraAngleZ;

	float								m_fWeaponX, m_fWeaponY, m_fWeaponZ;

	unsigned int						m_uiWeapon[ 4 ];
	unsigned int						m_uiWeaponCost[ 4 ];
	bool								m_bWeaponEnabled[ 4 ];
};

// The Manager
class CAmmuPoolManager
{
public:
	static CAmmuPool*					New								( unsigned char uc );

	static CAmmuPool*					Find							( unsigned char uc );

	static bool							Remove							( CAmmuPool* p );
	static bool							Remove							( unsigned char uc );

	static void							RemoveAll						( void );

	static unsigned char				Count							( void )							{ return m_ucAmmunations; }

	static void							ProcessAll						( void );

private:
	static unsigned char				m_ucAmmunations;
	static CAmmuPool*					m_AmmuPool[ MAX_AMMUS ];
};

#endif
