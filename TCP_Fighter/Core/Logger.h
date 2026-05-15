#pragma once

class CLogger
{
public:
    static void Info(const char* format, ...);
    static void Error(const char* format, ...);
};
