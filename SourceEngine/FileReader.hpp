#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include "IReader.hpp"

#include <string>
#include <fstream>

class FileReader : public IReader {
public:
	FileReader(const std::string &filename);

	virtual void read(char *buffer, int size);
	virtual void seek(int pos);

private:
	std::ifstream mFile;
};

#endif