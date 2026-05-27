#include "GameServer.h"
#include "RPC/RPCCommon.h"
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
        const PayloadSizeType payloadSize = static_cast<PayloadSizeType>(header[1]);
        const PacketType type = static_cast<PacketType>(header[2]);

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

        CPacket packet(packetSize);
        const int dequeueRet = session->RecvBuffer.Dequeue(packet.GetWriteBufferPtr(), packetSize);

        if (dequeueRet != packetSize)
            return;

        packet.MoveWritePos(packetSize);
        ProcessPacket(session, packet);

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
    const PacketType type = static_cast<PacketType>(buffer[2]);

    packet.MoveReadPos(dfPACKET_HEADER_SIZE);

    const bool dispatched = CC2S_Stub::Dispatch(*this, session, packet, type);

    if (!dispatched || packet.GetReadSize() != 0)
    {
        CLogger::Error("Invalid C2S packet. session=%llu, type=%u, remain=%d",
            session->GetSessionId(),
            static_cast<unsigned int>(type),
            packet.GetReadSize());
        Disconnect(session);
    }
}
