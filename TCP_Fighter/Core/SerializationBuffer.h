#pragma once

#include <cstdint>
#include <cstring>
#include <new>

class CPacket
{
public:
    enum en_PACKET
    {
        eBUFFER_DEFAULT = 1400
    };

    inline CPacket()
        : m_chpBuffer(m_chDefaultBuffer)
        , m_iBufferSize(eBUFFER_DEFAULT)
        , m_iWritePos(0)
        , m_iReadPos(0)
        , m_bError(false)
    {
    }

    inline explicit CPacket(int bufferSize)
        : m_chpBuffer(m_chDefaultBuffer)
        , m_iBufferSize(eBUFFER_DEFAULT)
        , m_iWritePos(0)
        , m_iReadPos(0)
        , m_bError(false)
    {
        if (bufferSize <= 0)
            return;

        if (bufferSize <= eBUFFER_DEFAULT)
        {
            m_iBufferSize = bufferSize;
            return;
        }

        char* dynamicBuffer = new (std::nothrow) char[bufferSize];
        if (dynamicBuffer == nullptr)
        {
            m_bError = true;
            return;
        }

        m_chpBuffer = dynamicBuffer;
        m_iBufferSize = bufferSize;
    }

    inline ~CPacket()
    {
        if (m_chpBuffer != m_chDefaultBuffer)
        {
            delete[] m_chpBuffer;
            m_chpBuffer = m_chDefaultBuffer;
        }
    }

    CPacket(const CPacket&) = delete;
    CPacket& operator=(const CPacket&) = delete;
    CPacket(CPacket&&) = delete;
    CPacket& operator=(CPacket&&) = delete;

    inline void Clear()
    {
        m_iWritePos = 0;
        m_iReadPos = 0;
        m_bError = false;
    }

    inline int GetBufferSize() const { return m_iBufferSize; }
    inline int GetDataSize() const { return m_iWritePos; }
    inline int GetReadSize() const { return m_iWritePos - m_iReadPos; }
    inline int GetWriteSize() const { return m_iBufferSize - m_iWritePos; }
    inline bool IsError() const { return m_bError; }
    inline bool IsDynamicBuffer() const { return m_chpBuffer != m_chDefaultBuffer; }

    inline char* GetBufferPtr() { return m_chpBuffer; }
    inline const char* GetBufferPtr() const { return m_chpBuffer; }
    inline char* GetReadBufferPtr() { return m_chpBuffer + m_iReadPos; }
    inline const char* GetReadBufferPtr() const { return m_chpBuffer + m_iReadPos; }
    inline char* GetWriteBufferPtr() { return m_chpBuffer + m_iWritePos; }
    inline const char* GetWriteBufferPtr() const { return m_chpBuffer + m_iWritePos; }

    inline int MoveWritePos(int size)
    {
        if (size <= 0)
            return 0;

        const int movableSize = size > GetWriteSize() ? GetWriteSize() : size;
        m_iWritePos += movableSize;

        if (movableSize != size)
            m_bError = true;

        return movableSize;
    }

    inline int MoveReadPos(int size)
    {
        if (size <= 0)
            return 0;

        const int movableSize = size > GetReadSize() ? GetReadSize() : size;
        m_iReadPos += movableSize;

        if (movableSize != size)
            m_bError = true;

        return movableSize;
    }

    inline int PutData(const char* src, int size)
    {
        if (src == nullptr || size <= 0)
            return 0;

        const int writeSize = size > GetWriteSize() ? GetWriteSize() : size;

        if (writeSize > 0)
        {
            std::memcpy(m_chpBuffer + m_iWritePos, src, writeSize);
            m_iWritePos += writeSize;
        }

        if (writeSize != size)
            m_bError = true;

        return writeSize;
    }

    inline int GetData(char* dest, int size)
    {
        if (dest == nullptr || size <= 0)
            return 0;

        const int readSize = size > GetReadSize() ? GetReadSize() : size;

        if (readSize > 0)
        {
            std::memcpy(dest, m_chpBuffer + m_iReadPos, readSize);
            m_iReadPos += readSize;
        }

        if (readSize != size)
        {
            if (dest != nullptr && size > 0)
                std::memset(dest, 0, static_cast<std::size_t>(size));

            m_bError = true;
        }

        return readSize;
    }

    inline CPacket& operator<<(std::int8_t value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(std::uint8_t value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(std::int16_t value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(std::uint16_t value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(std::int32_t value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(std::uint32_t value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(std::int64_t value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(std::uint64_t value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(float value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator<<(double value)
    {
        PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(std::int8_t& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(std::uint8_t& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(std::int16_t& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(std::uint16_t& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(std::int32_t& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(std::uint32_t& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(std::int64_t& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(std::uint64_t& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(float& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

    inline CPacket& operator>>(double& value)
    {
        GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(value)));
        return *this;
    }

private:
    char m_chDefaultBuffer[eBUFFER_DEFAULT]{};
    char* m_chpBuffer;
    int m_iBufferSize;
    int m_iWritePos;
    int m_iReadPos;
    bool m_bError;
};
