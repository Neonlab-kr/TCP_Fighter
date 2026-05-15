#include "RingBuffer.h"

CRingBuffer::CRingBuffer(void)
{
    m_chpBuffer = nullptr;
    m_iBufferSize = 0;
    m_iFront = 0;
    m_iRear = 0;
    m_iUseSize = 0;
}

CRingBuffer::CRingBuffer(int iBufferSize)
{
    m_chpBuffer = nullptr;
    m_iBufferSize = 0;
    m_iFront = 0;
    m_iRear = 0;
    m_iUseSize = 0;
    Resize(iBufferSize);
}

CRingBuffer::~CRingBuffer(void)
{
    if (m_chpBuffer != nullptr)
    {
        free(m_chpBuffer);
        m_chpBuffer = nullptr;
    }

    m_iBufferSize = 0;
    m_iFront = 0;
    m_iRear = 0;
    m_iUseSize = 0;
}

void CRingBuffer::Resize(int size)
{
    if (size <= 0)
        return;

    char* newBuffer = static_cast<char*>(malloc(size));
    if (newBuffer == nullptr)
        return;

    int copySize = 0;

    if (m_chpBuffer != nullptr)
    {
        copySize = m_iUseSize;
        if (copySize > size)
            copySize = size;

        if (copySize > 0)
        {
            int firstSize = m_iBufferSize - m_iFront;
            if (firstSize > copySize)
                firstSize = copySize;

            std::memcpy(newBuffer, m_chpBuffer + m_iFront, firstSize);

            const int secondSize = copySize - firstSize;
            if (secondSize > 0)
                std::memcpy(newBuffer + firstSize, m_chpBuffer, secondSize);
        }

        free(m_chpBuffer);
    }

    m_chpBuffer = newBuffer;
    m_iBufferSize = size;
    m_iFront = 0;
    m_iRear = copySize;

    if (m_iRear == m_iBufferSize)
        m_iRear = 0;

    m_iUseSize = copySize;
}

SendRingBuffer::SendRingBuffer(void)
    : CRingBuffer()
{
}

SendRingBuffer::SendRingBuffer(int iBufferSize)
    : CRingBuffer(iBufferSize)
{
}

RecvRingBuffer::RecvRingBuffer(void)
    : CRingBuffer()
{
}

RecvRingBuffer::RecvRingBuffer(int iBufferSize)
    : CRingBuffer(iBufferSize)
{
}
