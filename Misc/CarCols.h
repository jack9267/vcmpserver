#ifndef _CARCOLS_H
#define _CARCOLS_H

class CCarCols
{
public:
	CCarCols( unsigned char ucModel );
	~CCarCols(void);

	unsigned char							GetModel								( void )								{ return m_ucModel; }

	void									AddColour								( unsigned char ucCol1, unsigned char ucCol2 );

	///<summary>Generated a random colour ID. This MUST be called before asking for a random colour</summary>
	void									GenerateRandCol							( void );

	unsigned char							RandomColour1							( void );
	unsigned char							RandomColour2							( void );

private:
	unsigned char							m_ucModel;

	unsigned char							m_ucColours;

	unsigned char							m_ucRandCol;

	unsigned char							m_ucCol1[ 50 ];
	unsigned char							m_ucCol2[ 50 ];
};

class CCarColManager
{
public:
	static CCarCols*						New										( unsigned char ucModel );

	static CCarCols*						Find									( unsigned char ucModel );

	static bool								Remove									( CCarCols* p );
	static bool								Remove									( unsigned char uc );

	static void								RemoveAll								( void );

	static unsigned char					Count									( void )								{ return m_ucCols; }

private:
	static unsigned char					m_ucCols;
	static CCarCols*						m_CarCols[ 110 ]; // Should be set to ammount of vehicles
};

#endif
