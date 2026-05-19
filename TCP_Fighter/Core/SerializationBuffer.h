#pragma once

#include <cstdint>
#include <cstring>

class CPacket
{
public:
    enum en_PACKET
    {
        eBUFFER_DEFAULT = 1400
    };

    inline CPacket()
        : m_iBufferSize(eBUFFER_DEFAULT), m_iWritePos(0), m_iReadPos(0), m_bError(false)
    {
    }

    inline explicit CPacket(int bufferSize)
        : m_iBufferSize(bufferSize), m_iWritePos(0), m_iReadPos(0), m_bError(false)
    {
        if (m_iBufferSize <= 0)
            m_iBufferSize = eBUFFER_DEFAULT;

        if (m_iBufferSize > eBUFFER_DEFAULT)
            m_iBufferSize = eBUFFER_DEFAULT;
    }

    ~CPacket() = default;

    CPacket(const CPacket&) = delete;
    CPacket& operator=(const CPacket&) = delete;

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

    inline char* GetBufferPtr() { return m_chBuffer; }
    inline const char* GetBufferPtr() const { return m_chBuffer; }
    inline char* GetReadBufferPtr() { return m_chBuffer + m_iReadPos; }
    inline const char* GetReadBufferPtr() const { return m_chBuffer + m_iReadPos; }
    inline char* GetWriteBufferPtr() { return m_chBuffer + m_iWritePos; }
    inline const char* GetWriteBufferPtr() const { return m_chBuffer + m_iWritePos; }

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
            std::memcpy(m_chBuffer + m_iWritePos, src, writeSize);
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
            std::memcpy(dest, m_chBuffer + m_iReadPos, readSize);
            m_iReadPos += readSize;
        }

        if (readSize != size)
            m_bError = true;

        return readSize;
    }

    inline CPacket& operator<<(std::uint8_t value) { WritePrimitive(value); return *this; }
    inline CPacket& operator<<(char value) { WritePrimitive(value); return *this; }
    inline CPacket& operator<<(short value) { WritePrimitive(value); return *this; }
    inline CPacket& operator<<(std::uint16_t value) { WritePrimitive(value); return *this; }
    inline CPacket& operator<<(int value) { WritePrimitive(value); return *this; }
    inline CPacket& operator<<(std::uint32_t value) { WritePrimitive(value); return *this; }
#if defined(_WIN32)
    inline CPacket& operator<<(long value) { WritePrimitive(value); return *this; }
#endif
    inline CPacket& operator<<(float value) { WritePrimitive(value); return *this; }
    inline CPacket& operator<<(std::int64_t value) { WritePrimitive(value); return *this; }
    inline CPacket& operator<<(double value) { WritePrimitive(value); return *this; }

    inline CPacket& operator>>(std::uint8_t& value) { ReadPrimitive(value); return *this; }
    inline CPacket& operator>>(char& value) { ReadPrimitive(value); return *this; }
    inline CPacket& operator>>(short& value) { ReadPrimitive(value); return *this; }
    inline CPacket& operator>>(std::uint16_t& value) { ReadPrimitive(value); return *this; }
    inline CPacket& operator>>(int& value) { ReadPrimitive(value); return *this; }
    inline CPacket& operator>>(std::uint32_t& value) { ReadPrimitive(value); return *this; }
#if defined(_WIN32)
    inline CPacket& operator>>(long& value) { ReadPrimitive(value); return *this; }
#endif
    inline CPacket& operator>>(float& value) { ReadPrimitive(value); return *this; }
    inline CPacket& operator>>(std::int64_t& value) { ReadPrimitive(value); return *this; }
    inline CPacket& operator>>(double& value) { ReadPrimitive(value); return *this; }

private:
    template <typename T>
    inline void WritePrimitive(const T& value)
    {
        const int written = PutData(reinterpret_cast<const char*>(&value), static_cast<int>(sizeof(T)));
        if (written != static_cast<int>(sizeof(T)))
            m_bError = true;
    }

    template <typename T>
    inline void ReadPrimitive(T& value)
    {
        const int read = GetData(reinterpret_cast<char*>(&value), static_cast<int>(sizeof(T)));
        if (read != static_cast<int>(sizeof(T)))
        {
            value = T{};
            m_bError = true;
        }
    }

private:
    char m_chBuffer[eBUFFER_DEFAULT];
    int m_iBufferSize;
    int m_iWritePos;
    int m_iReadPos;
    bool m_bError;
};
