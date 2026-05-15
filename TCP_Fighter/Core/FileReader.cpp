#include "FileReader.h"
#include <fstream>

bool CFileReader::ReadText(const std::string& path, std::string& outText)
{
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open())
        return false;

    file.seekg(0, std::ios::end);
    std::streamoff size = file.tellg();

    if (size < 0)
        return false;

    file.seekg(0, std::ios::beg);
    outText.resize(static_cast<size_t>(size));

    if (size == 0)
        return true;

    file.read(&outText[0], size);
    return file.good();
}

bool CFileReader::ReadBinary(const std::string& path, std::vector<unsigned char>& outData)
{
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open())
        return false;

    file.seekg(0, std::ios::end);
    std::streamoff size = file.tellg();

    if (size < 0)
        return false;

    file.seekg(0, std::ios::beg);
    outData.resize(static_cast<size_t>(size));

    if (size == 0)
        return true;

    file.read(reinterpret_cast<char*>(outData.data()), size);
    return file.good();
}
