#ifndef _COREDEFS_H
#define _COREDEFS_H

#define MAX_PLAYERS 50
#define MAX_VEHICLES 180
#define MAX_PICKUPS 500
#define MAX_SPAWNS 100

#define MAX_NAME_LEN 24
#define MAX_IP_LEN 16
#define MAX_PASS_LEN 32
#define MAX_SERVNAME_LEN 128
#define MAX_GAMEMODE_LEN 64

#define MAX_TEXT_LEN 256

#pragma pack( push, 1 )
typedef struct _VECTOR {
	float X,Y,Z;
} VECTOR, *PVECTOR;
#pragma pack(pop)

#pragma pack( push, 1 )
typedef struct _C_VECTOR1 { // compressed normalised rotation vector
	short X,Y,Z;
} C_VECTOR1;
#pragma pack(pop)

typedef struct _VECTOR_PAD {
	float X,Y,Z,pad;
} VECTOR_PAD, *PVECTOR_PAD;

typedef struct _MATRIX4X4 {
	VECTOR_PAD vLookRight;
	VECTOR_PAD vLookUp;
	VECTOR_PAD vLookAt;
	VECTOR_PAD vPos;
} MATRIX4X4, *PMATRIX4X4;

#pragma pack( push, 1 )
typedef struct _ONFOOT_SYNC_DATA
{
	WORD wKeys; //0-2
	VECTOR vecPos; //2-14
	float fRotation; //14-18
	BYTE byteHealth; //18-19
	BYTE byteArmour; //19-20
	BYTE byteAction; //20-21
	BYTE byteThing1; //21-22
	BYTE byteCurrentWeapon; //22-23
	int iCash; //23-27
	BYTE bytePadding[ 12 ]; //27-39
} ONFOOT_SYNC_DATA;
#pragma pack(pop)

#pragma pack( push, 1 )
typedef struct _AIM_SYNC_DATA
{
	VECTOR vecAimf1; //0-12
	//VECTOR vecAimf2; //12-24
	VECTOR vecAimPos; //12-24
	BYTE bytePadding[ 4 ];//24-28
} AIM_SYNC_DATA;
#pragma pack(pop)

#pragma pack( push, 1 )
typedef struct _INCAR_SYNC_DATA
{
	WORD wKeys; // 0-2
	C_VECTOR1 cvecRoll; //2-8
	C_VECTOR1 cvecDirection;//8-14
	VECTOR vecPos; //14-26
	VECTOR vecMoveSpeed; //26-38
	float fHealth; //38-42
	BYTE bytePlayerHealth; //42-43
	BYTE bytePlayerArmour; //43-44
	BYTE bytePadding[ 11 ]; //44-56
} INCAR_SYNC_DATA;
#pragma pack(pop)

#endif
