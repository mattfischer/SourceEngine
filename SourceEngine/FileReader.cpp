#include "FileReader.hpp"

FileReader::FileReader(const std::string& filename)
: mFile(filename.c_str(), std::ios_base::in | std::ios_base::binary)
{
}

void FileReader::read(char *buffer, int size)
{
	mFile.read(buffer, size);
}

void FileReader::seek(int pos)
{
	mFile.seekg(pos);
}
