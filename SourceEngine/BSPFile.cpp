#include "BSPFile.hpp"

#include <fstream>
#include <vector>

struct Lump {
    int offset;
    int length;
    int version;
    char fourCC[4];
};

#define NUM_LUMPS 64

struct Header {
    int magic;
    int version;
    Lump lumps[NUM_LUMPS];
    int revision;
};

BSPFile::BSPFile(const std::string &filename)
{
    std::ifstream file(filename.c_str());

    Header header;
    file.read((char*)&header, sizeof(header));
}