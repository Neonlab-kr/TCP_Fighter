#pragma once

#include <cstdlib>
#include <cstring>

#define RINGBUFFER_PACKET_SIZE 16

class CRingBuffer
{
public:
    CRingBuffer(void);
    CRingBuffer(int iBufferSize);
    ~CRingBuffer(void);

    CRingBuffer(const CRingBuffer&) = delete;
    CRingBuffer& operator=(const CRingBuffer&) = delete;

    void Resize(int size);

    inline int GetBufferSize(void) const { return m_iBufferSize; }
    inline int GetUseSize(void) const { return m_iUseSize; }
    inline int GetFreeSize(void) const { return m_iBufferSize - m_iUseSize; }

    inline int Enqueue(const char* chpData, int iSize)
    {
        if (chpData == nullptr || iSize <= 0)
            return 0;

        int freeSize = GetFreeSize();
        if (freeSize <= 0)
            return 0;

        if (iSize > freeSize)
            iSize = freeSize;

        int directSize = DirectEnqueueSize();
        if (directSize > iSize)
            directSize = iSize;

        std::memcpy(m_chpBuffer + m_iRear, chpData, directSize);

        const int remainSize = iSize - directSize;
        if (remainSize > 0)
            std::memcpy(m_chpBuffer, chpData + directSize, remainSize);

        MoveRear(iSize);
        return iSize;
    }

    inline int Dequeue(char* chpDest, int iSize)
    {
        if (chpDest == nullptr || iSize <= 0)
            return 0;

        int useSize = GetUseSize();
        if (useSize <= 0)
            return 0;

        if (iSize > useSize)
            iSize = useSize;

        int directSize = DirectDequeueSize();
        if (directSize > iSize)
            directSize = iSize;

        std::memcpy(chpDest, m_chpBuffer + m_iFront, directSize);

        const int remainSize = iSize - directSize;
        if (remainSize > 0)
            std::memcpy(chpDest + directSize, m_chpBuffer, remainSize);

        MoveFront(iSize);
        return iSize;
    }

    inline int Peek(char* chpDest, int iSize) const
    {
        if (chpDest == nullptr || iSize <= 0)
            return 0;

        int useSize = GetUseSize();
        if (useSize <= 0)
            return 0;

        if (iSize > useSize)
            iSize = useSize;

        int directSize = DirectDequeueSize();
        if (directSize > iSize)
            directSize = iSize;

        std::memcpy(chpDest, m_chpBuffer + m_iFront, directSize);

        const int remainSize = iSize - directSize;
        if (remainSize > 0)
            std::memcpy(chpDest + directSize, m_chpBuffer, remainSize);

        return iSize;
    }

    inline void ClearBuffer(void)
    {
        m_iFront = 0;
        m_iRear = 0;
        m_iUseSize = 0;
    }

    inline int DirectEnqueueSize(void) const
    {
        int freeSize = GetFreeSize();
        if (freeSize <= 0)
            return 0;

        int directSize = m_iBufferSize - m_iRear;
        if (directSize > freeSize)
            directSize = freeSize;

        return directSize;
    }

    inline int DirectDequeueSize(void) const
    {
        int useSize = GetUseSize();
        if (useSize <= 0)
            return 0;

        int directSize = m_iBufferSize - m_iFront;
        if (directSize > useSize)
            directSize = useSize;

        return directSize;
    }

    inline int MoveRear(int iSize)
    {
        if (iSize <= 0)
            return 0;

        int freeSize = GetFreeSize();
        if (iSize > freeSize)
            iSize = freeSize;

        m_iRear += iSize;
        if (m_iRear >= m_iBufferSize)
            m_iRear -= m_iBufferSize;

        m_iUseSize += iSize;
        return iSize;
    }

    inline int MoveFront(int iSize)
    {
        if (iSize <= 0)
            return 0;

        int useSize = GetUseSize();
        if (iSize > useSize)
            iSize = useSize;

        m_iFront += iSize;
        if (m_iFront >= m_iBufferSize)
            m_iFront -= m_iBufferSize;

        m_iUseSize -= iSize;
        return iSize;
    }

    inline char* GetFrontBufferPtr(void) const { return m_chpBuffer == nullptr ? nullptr : m_chpBuffer + m_iFront; }
    inline char* GetRearBufferPtr(void) const { return m_chpBuffer == nullptr ? nullptr : m_chpBuffer + m_iRear; }

private:
    char* m_chpBuffer;
    int m_iBufferSize;
    int m_iFront;
    int m_iRear;
    int m_iUseSize;
};

class SendRingBuffer : public CRingBuffer
{
public:
    SendRingBuffer(void);
    SendRingBuffer(int iBufferSize);
};

class RecvRingBuffer : public CRingBuffer
{
public:
    RecvRingBuffer(void);
    RecvRingBuffer(int iBufferSize);
};
