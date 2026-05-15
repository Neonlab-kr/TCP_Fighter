#pragma once

#include <string>
#include <vector>

class CFileReader
{
public:
    static bool ReadText(const std::string& path, std::string& outText);
    static bool ReadBinary(const std::string& path, std::vector<unsigned char>& outData);
};
