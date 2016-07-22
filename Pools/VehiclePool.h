#ifndef _VEHICLEPOOL_H
#define _VEHICLEPOOL_H

#include "Main.h"

class CVehiclePool : public CEntity
{
public:
	CVehiclePool( unsigned char uc );
	CVehiclePool() { };
	~CVehiclePool(void);

	unsigned char						GetID							( void )							{ return m_ucID; }

	BYTE								GetModel						( void )							{ return m_bModel; }
	void								SetModel						( BYTE b )							{ m_bModel = b; }

	float								GetHealth						( void )							{ return m_fHealth; }
	void								SetHealth						( float f )							{ m_fHealth = f; }

	BYTE								GetColour1						( void )							{ return m_bColour1; }
	void								SetColour1						( BYTE b )							{ m_bColour1 = b; }

	BYTE								GetColour2						( void )							{ return m_bColour2; }
	void								SetColour2						( BYTE b )							{ m_bColour2 = b; }

	BYTE								GetDriver						( void )							{ return m_bDriver; }
	void								SetDriver						( BYTE b )							{ m_bDriver = b; }

	float								GetSpawnX						( void )							{ return m_fSpawnX; }
	void								SetSpawnX						( float f )							{ m_fSpawnX = f; }

	float								GetSpawnY						( void )							{ return m_fSpawnY; }
	void								SetSpawnY						( float f )							{ m_fSpawnY = f; }

	float								GetSpawnZ						( void )							{ return m_fSpawnZ; }
	void								SetSpawnZ						( float f )							{ m_fSpawnZ = f; }

	float								GetSpawnRotation				( void )							{ return m_fSpawnRotation; }
	void								SetSpawnRotation				( float f )							{ m_fSpawnRotation = f; }

	bool								GetLocked						( void )							{ return m_bLocked; }
	void								SetLocked						( bool b )							{ m_bLocked = b; }

	bool								GetRandomColour1				( void )							{ return m_bRandomColour1; }
	void								SetRandomColour1				( bool b )							{ m_bRandomColour1 = b; }

	bool								GetRandomColour2				( void )							{ return m_bRandomColour2; }
	void								SetRandomColour2				( bool b )							{ m_bRandomColour2 = b; }

	bool								GetBeenUsed						( void )							{ return m_bUsed; }
	void								SetBeenUsed						( bool b )							{ m_bUsed = b; }

	unsigned int						GetLastUsedTime					( void )							{ return m_uiLastUsed; }
	void								SetLastUsedTime					( unsigned int ui )					{ m_uiLastUsed = ui; }

private:
	unsigned char						m_ucID;

	BYTE								m_bModel;
	float								m_fHealth;

	BYTE								m_bColour1;
	BYTE								m_bColour2;

	BYTE								m_bDriver;

	float								m_fSpawnX, m_fSpawnY, m_fSpawnZ;
	float								m_fSpawnRotation;

	bool								m_bLocked;

	bool								m_bRandomColour1;
	bool								m_bRandomColour2;

	bool								m_bUsed;
	unsigned int						m_uiLastUsed;
};

class CVehiclePoolManager
{
public:
	static CVehiclePool*				New								( unsigned char uc );

	static CVehiclePool*				Find							( unsigned char uc );

	static bool							Remove							( CVehiclePool* p );
	static bool							Remove							( unsigned char uc );

	static void							RemoveAll						( void );

	static unsigned char				Count							( void )							{ return m_ucVehicles; }

private:
	static unsigned char				m_ucVehicles;
	static CVehiclePool*				m_VehiclePool[ MAX_VEHICLES ];
};

#endif
