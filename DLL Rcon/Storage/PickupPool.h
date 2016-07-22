#ifndef _PICKUPPOOL_H
#define _PICKUPPOOL_H

#include "Main.h"

class CPickupPool : public CEntity
{
public:
	CPickupPool( unsigned int ui );
	~CPickupPool(void);

	unsigned int						GetID							( void )							{ return m_uiID; }

	unsigned int						GetModel						( void )							{ return m_uiModel; }
	void								SetModel						( unsigned int ui )					{ m_uiModel = ui; }

	unsigned char						GetSpawnForPlayer				( void )							{ return m_ucSpawnFor; }
	void								SetSpawnForPlayer				( unsigned char uc )				{ m_ucSpawnFor = uc; }

	bool								GetSpawned						( void )							{ return m_bSpawned; }
	void								SetSpawned						( bool b )							{ m_bSpawned = b; }

	bool								GetOneTime						( void )							{ return m_bOneTime; }
	void								SetOneTime						( bool b )							{ m_bOneTime = b; }

	unsigned int						GetRespawnTime					( void )							{ return m_uiRespawnTime; }
	void								SetRespawnTime					( unsigned int ui )					{ m_uiRespawnTime = ui; }

	unsigned int						GetPickedUpTime					( void )							{ return m_uiPickedUpTime; }
	void								SetPickedUpTime					( unsigned int ui )					{ m_uiPickedUpTime = ui; }

private:
	unsigned int						m_uiID;

	unsigned int						m_uiModel;
	unsigned char						m_ucSpawnFor;

	bool								m_bSpawned;
	bool								m_bOneTime;

	unsigned int						m_uiRespawnTime;
	unsigned int						m_uiPickedUpTime;
};

class CPickupPoolManager
{
public:
	static CPickupPool*					New								( unsigned int ui );

	static CPickupPool*					Find							( unsigned int ui );

	static bool							Remove							( CPickupPool* p );
	static bool							Remove							( unsigned int ui );

	static void							RemoveAll						( void );

	static unsigned int					Count							( void )							{ return m_uiPickups; }

private:
	static unsigned int					m_uiPickups;
	static CPickupPool*					m_PickupPool[ MAX_PICKUPS ];
};

#endif
