#ifndef _ENTITY_H
#define _ENTITY_H

class CEntity
{
public:
	CEntity();
	~CEntity(void) {}

	float								GetX							( void )							{ return m_fX; }
	void								SetX							( float f )							{ m_fX = f; }

	float								GetY							( void )							{ return m_fY; }
	void								SetY							( float f )							{ m_fY = f; }

	float								GetZ							( void )							{ return m_fZ; }
	void								SetZ							( float f )							{ m_fZ = f; }

	float								GetRotation						( void )							{ return m_fRotation; }
	void								SetRotation						( float f )							{ m_fRotation = f; }

	bool								GetSpawned						( void )							{ return m_bSpawned; }
	void								SetSpawned						( bool b )							{ m_bSpawned = b; }

private:
	float								m_fX, m_fY, m_fZ;
	float								m_fRotation;

	bool								m_bSpawned;
};

#endif
