#pragma once

#include <memory>
#include <cstdint>

#include "../Core/SelectServer.h"
#include "GameSession.h"
#include "GameData.h"
#include "PacketDefine.h"
#include "../Core/SerializationBuffer.h"

class CGameServer : public CSelectServer
{
public:
    CGameServer();
    ~CGameServer();

    bool Init(const char* configPath);
    void Run();
    void Stop();

protected:
    void OnAccept(CSession* session) override;
    void OnRelease(CSession* session) override;
    void OnRecv(CSession* session) override;
    void OnError(int errorCode) override;

private:
    void FixedUpdateGame();
    void Render();
    void ProcessPacket(CSession* session, CPacket& packet);

    bool PacketProc_MoveStart(CSession* session, CPacket& packet);
    bool PacketProc_MoveStop(CSession* session, CPacket& packet);
    bool PacketProc_Attack1(CSession* session, CPacket& packet);
    bool PacketProc_Attack2(CSession* session, CPacket& packet);
    bool PacketProc_Attack3(CSession* session, CPacket& packet);
    bool PacketProc_Sync(CSession* session, CPacket& packet);

    inline GameSession* FindGameSession(CSession* session)
    {
        return session == nullptr ? nullptr : static_cast<GameSession*>(session->UserData);
    }

    inline const GameSession* FindGameSession(const CSession* session) const
    {
        return session == nullptr ? nullptr : static_cast<const GameSession*>(session->UserData);
    }

    void StartMove(GameSession& gameSession, std::uint8_t direction, short clientX, short clientY, CSession* requestSession);
    void StopMoveRequest(GameSession& gameSession, std::uint8_t direction, short clientX, short clientY, CSession* requestSession);
    void UpdateMove(GameSession& gameSession);
    void StopMove(GameSession& gameSession, bool sendPacket);
    void SendMoveStop(int id, std::uint8_t direction, short x, short y);
    void SendSync(int id, short x, short y, CSession* targetSession);

    void RequestAttack(GameSession& attacker, std::uint8_t attackType, std::uint8_t direction, short clientX, short clientY, CSession* requestSession);
    void ProcessAttack(GameSession& attacker, std::uint8_t attackType, const AttackGameData& attackData);
    void SendAttackPacket(std::uint8_t attackType, int id, std::uint8_t direction, short x, short y, CSession* exceptSession);
    void SendDamage(int attackId, int damageId, std::uint8_t damageHp);
    void ProcessDeadPlayer(CSession* session);

    inline const AttackGameData* GetAttackData(std::uint8_t attackType) const
    {
        switch (attackType)
        {
        case dfPACKET_CS_ATTACK1:
            return &m_GameData.Attack1;

        case dfPACKET_CS_ATTACK2:
            return &m_GameData.Attack2;

        case dfPACKET_CS_ATTACK3:
            return &m_GameData.Attack3;

        default:
            return nullptr;
        }
    }

    inline bool IsTimeBefore(std::uint32_t currentMs, std::uint32_t targetMs) const
    {
        return static_cast<int>(currentMs - targetMs) < 0;
    }

    inline bool IsAttackLocked(const GameSession& gameSession, std::uint32_t nowMs) const
    {
        return gameSession.AttackLockUntilTimeMs != 0 && IsTimeBefore(nowMs, gameSession.AttackLockUntilTimeMs);
    }

    inline bool ValidatePayloadSize(const CPacket& packet, int expectedPayloadSize) const
    {
        return packet.GetReadSize() == expectedPayloadSize;
    }

    inline bool ValidateDirection(std::uint8_t direction) const
    {
        return direction <= dfPACKET_MOVE_DIR_LD;
    }

    inline bool IsClientPositionInErrorRange(const GameSession& gameSession, short clientX, short clientY) const
    {
        int diffX = static_cast<int>(clientX) - static_cast<int>(gameSession.X);
        int diffY = static_cast<int>(clientY) - static_cast<int>(gameSession.Y);

        if (diffX < 0)
            diffX = -diffX;

        if (diffY < 0)
            diffY = -diffY;

        return diffX <= m_GameData.Move.ErrorRange && diffY <= m_GameData.Move.ErrorRange;
    }

    inline short ClampX(short x) const
    {
        if (x <= m_GameData.World.MoveLeft)
            return static_cast<short>(m_GameData.World.MoveLeft + 1);

        if (x >= m_GameData.World.MoveRight)
            return static_cast<short>(m_GameData.World.MoveRight - 1);

        return x;
    }

    inline short ClampY(short y) const
    {
        if (y <= m_GameData.World.MoveTop)
            return static_cast<short>(m_GameData.World.MoveTop + 1);

        if (y >= m_GameData.World.MoveBottom)
            return static_cast<short>(m_GameData.World.MoveBottom - 1);

        return y;
    }

    inline std::uint32_t GetClientFrameMs() const
    {
        if (m_Config.ClientFPS <= 0)
            return 20;

        const std::uint32_t frameMs = static_cast<std::uint32_t>(1000 / m_Config.ClientFPS);
        return frameMs == 0 ? 1 : frameMs;
    }

private:
    bool m_Running;
    int m_NextPlayerId;
    int m_MaxGameSession;
    int m_ActiveGameSessionCount;
    ServerConfig m_Config;
    GameData m_GameData;
    std::unique_ptr<GameSession[]> m_GameSessions;
    std::unique_ptr<CSession*[]> m_DeadSessionBuffer;
    CPacket m_PacketBuffer;
    CPacket m_SendPacketBuffer;
    std::uint32_t m_CurrentTimeMs;
    std::uint32_t m_LastLoopTimeMs;
    std::uint32_t m_FixedUpdateAccumulatorMs;
    std::uint32_t m_ClientFrameMs;
};
