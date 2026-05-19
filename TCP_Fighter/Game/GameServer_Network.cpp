#include "GameServer.h"
#include "PacketDefine.h"
#include "../Core/Logger.h"
#include "../Core/Session.h"

void CGameServer::OnRecv(CSession* session)
{
    if (session == nullptr)
        return;

    while (true)
    {
        if (session->RecvBuffer.GetUseSize() < dfPACKET_HEADER_SIZE)
            return;

        std::uint8_t header[dfPACKET_HEADER_SIZE]{};
        const int peekRet = session->RecvBuffer.Peek(reinterpret_cast<char*>(header), dfPACKET_HEADER_SIZE);

        if (peekRet != dfPACKET_HEADER_SIZE)
            return;

        const std::uint8_t code = header[0];
        const std::uint8_t payloadSize = header[1];
        const std::uint8_t type = header[2];

        if (code != dfPACKET_CODE)
        {
            CLogger::Error("Invalid packet code. session=%llu, code=0x%02x", session->GetSessionId(), code);
            Disconnect(session);
            return;
        }

        const int packetSize = dfPACKET_HEADER_SIZE + static_cast<int>(payloadSize);

        if (packetSize <= dfPACKET_HEADER_SIZE || packetSize > dfPACKET_MAX_SIZE)
        {
            CLogger::Error("Invalid packet size. session=%llu, size=%d", session->GetSessionId(), packetSize);
            Disconnect(session);
            return;
        }

        if (session->RecvBuffer.GetUseSize() < packetSize)
            return;

        m_PacketBuffer.Clear();
        const int dequeueRet = session->RecvBuffer.Dequeue(m_PacketBuffer.GetWriteBufferPtr(), packetSize);

        if (dequeueRet != packetSize)
            return;

        m_PacketBuffer.MoveWritePos(packetSize);
        ProcessPacket(session, m_PacketBuffer);

        if (session->DisconnectPending)
            return;
    }
}

void CGameServer::ProcessPacket(CSession* session, CPacket& packet)
{
    if (session == nullptr || packet.GetDataSize() < dfPACKET_HEADER_SIZE)
        return;

    GameSession* gameSession = FindGameSession(session);

    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return;
    }

    const std::uint8_t* buffer = reinterpret_cast<const std::uint8_t*>(packet.GetBufferPtr());
    const std::uint8_t type = buffer[2];

    packet.MoveReadPos(dfPACKET_HEADER_SIZE);

    switch (type)
    {
    case dfPACKET_CS_MOVE_START:
        PacketProc_MoveStart(session, packet);
        break;

    case dfPACKET_CS_MOVE_STOP:
        PacketProc_MoveStop(session, packet);
        break;

    case dfPACKET_CS_ATTACK1:
        PacketProc_Attack1(session, packet);
        break;

    case dfPACKET_CS_ATTACK2:
        PacketProc_Attack2(session, packet);
        break;

    case dfPACKET_CS_ATTACK3:
        PacketProc_Attack3(session, packet);
        break;

    case dfPACKET_CS_SYNC:
        PacketProc_Sync(session, packet);
        break;

    default:
        CLogger::Error("Unknown packet type. session=%llu, type=%u", session->GetSessionId(), type);
        Disconnect(session);
        break;
    }
}
