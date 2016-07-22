#ifndef _NETRPC_H
#define _NETRPC_H

#include "Net.h"

void RegisterRPCs( RakServerInterface * );
void UnRegisterRPCs( RakServerInterface * );

void PlayerSync( Packet* pPacket );
void AimSync( Packet* pPacket );
void VehicleSync( Packet* pPacket );
void PassengerSync( Packet* pPacket );

#endif
