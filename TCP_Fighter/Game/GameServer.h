#pragma once

#include <memory>
#include <cstdint>

#include "../Core/AsyncSelectServer.h"
#include "GameSession.h"
#include "GameData.h"

class CGameServer : public CAsyncSelectServer
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
    void ProcessPacket(CSession* session, const char* packet, int packetSize);

    inline GameSession* FindGameSession(CSession* session)
    {
        return session == nullptr ? nullptr : static_cast<GameSession*>(session->UserData);
    }

    inline const GameSession* FindGameSession(const CSession* session) const
    {
        return session == nullptr ? nullptr : static_cast<const GameSession*>(session->UserData);
    }

    void UpdateMove(GameSession& gameSession);
    void StopMove(GameSession& gameSession, bool sendPacket);
    void SendMoveStop(const GameSession& gameSession);
    void SendSync(const GameSession& gameSession, CSession* targetSession);

    void ProcessAttack(GameSession& attacker, std::uint8_t attackType);
    bool IsHitByAttack(const GameSession& attacker, const GameSession& target, short rangeX, short rangeY) const;
    void SendDamage(const GameSession& attacker, const GameSession& damaged);
    void ProcessDeadPlayer(CSession* session);

    const AttackGameData* GetAttackData(std::uint8_t attackType) const;
    bool IsAttackLocked(const GameSession& gameSession) const;

    bool ValidatePacketSize(const char* packet, int packetSize, int expectedSize);
    bool ValidateDirection(std::uint8_t direction) const;
    bool IsClientPositionInErrorRange(const GameSession& gameSession, short clientX, short clientY) const;

    short ClampX(short x) const;
    short ClampY(short y) const;
    std::uint32_t GetClientTickMs() const;
    int GetActivePlayerCount() const;

private:
    bool m_Running;
    int m_NextPlayerId;
    int m_MaxGameSession;
    int m_ActiveGameSessionCount;
    ServerConfig m_Config;
    GameData m_GameData;
    std::unique_ptr<GameSession[]> m_GameSessions;
    std::unique_ptr<CSession*[]> m_DeadSessionBuffer;
    unsigned __int64 m_LastLoopTickMs;
    unsigned __int64 m_FixedUpdateAccumulatorMs;
    unsigned __int64 m_FixedTick;
    std::uint32_t m_ClientTickMs;
};
