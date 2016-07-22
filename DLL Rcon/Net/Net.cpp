#include "Net.h"
#include "ConfigLoader.h"

RakClientInterface* CNet::m_pRak = NULL;

void CNet::Start(void)
{
	m_pRak = RakNetworkFactory::GetRakClientInterface();

	if (m_pRak)
	{
		m_pRak->SetPassword( CConfig::GetPassword() );
		if (m_pRak->Connect(CConfig::GetIP(), CConfig::GetPort(), 0, 0, 5))
		{
			m_pRak->StartOccasionalPing();
			CallSignal("vcmp.server.connecting", "%s %i %s %s", CConfig::GetIP(), CConfig::GetPort(), CConfig::GetPassword(), CConfig::GetNickname());
		}
	}
}

void CNet::Stop(void)
{
	if (m_pRak)
	{
		m_pRak->Disconnect(100);
		RakNetworkFactory::DestroyRakClientInterface( m_pRak );
		m_pRak = 0;
	}
}

void CNet::ClearPools(void)
{
	CRconPoolManager::RemoveAll();
	CPlayerPoolManager::RemoveAll();
	CPickupPoolManager::RemoveAll();
	CVehiclePoolManager::RemoveAll();
	CAmmuPoolManager::RemoveAll();
}

void CNet::Listen(void)
{
	if (m_pRak)
	{
		Packet* pPacket = 0;
		while (pPacket = m_pRak->Receive())
		{
			switch(pPacket->data[0])
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
					CallSignal( "vcmp.server.connected", "1" );
					CMiscSendPackets::RconJoin(CConfig::GetNickname());
					break;

				case ID_DISCONNECTION_NOTIFICATION:
					CallSignal( "vcmp.server.disconnected", "0" );
					ClearPools();
					break;

				case ID_CONNECTION_LOST:
					CallSignal( "vcmp.server.disconnected", "1" );
					ClearPools();
					break;

				case ID_CONNECTION_BANNED:
					CallSignal( "vcmp.server.disconnected", "2" );
					ClearPools();
					break;

				case ID_NO_FREE_INCOMING_CONNECTIONS:
					CallSignal( "vcmp.server.disconnected", "3" );
					ClearPools();
					break;

				case ID_INVALID_PASSWORD:
					CallSignal( "vcmp.server.disconnected", "4" );
					ClearPools();
					break;

				case ID_MISC_PACKET:
					CMiscRecvPackets::Parse( pPacket );
					break;

				case ID_PLAYER_PACKET:
					CPlayerRecvPackets::Parse( pPacket );
					break;

				case ID_VEHICLE_PACKET:
					CVehicleRecvPackets::Parse( pPacket );
					break;

				case ID_GAME_PACKET:
					CGameRecvPackets::Parse( pPacket );
					break;

				case ID_PICKUP_PACKET:
					CPickupRecvPackets::Parse( pPacket );
					break;
			}

			m_pRak->DeallocatePacket( pPacket );
		}
	}
}