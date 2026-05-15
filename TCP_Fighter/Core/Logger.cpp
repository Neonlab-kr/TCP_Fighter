#include "Logger.h"

#include <Windows.h>
#include <cstdio>
#include <cstdarg>
#include <ctime>

static void FormatLog(char* outBuffer, int outBufferSize, const char* level, const char* format, va_list args)
{
    if (outBuffer == nullptr || outBufferSize <= 0)
        return;

    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now);

    char timeText[32] = {};
    std::strftime(timeText, sizeof(timeText), "%H:%M:%S", &localTime);

    char message[1024] = {};
    vsnprintf_s(message, sizeof(message), _TRUNCATE, format, args);

    snprintf(
        outBuffer,
        outBufferSize,
        "[%s][%s] %s\n",
        timeText,
        level,
        message
    );
}

static void PrintLog(const char* level, const char* format, va_list args)
{
    char finalBuffer[1200] = {};
    FormatLog(finalBuffer, sizeof(finalBuffer), level, format, args);

    OutputDebugStringA(finalBuffer);
}

void CLogger::Info(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    PrintLog("INFO", format, args);
    va_end(args);
}

void CLogger::Error(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    PrintLog("ERROR", format, args);
    va_end(args);
}
