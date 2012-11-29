#ifndef BSPFILE_HPP
#define BSPFILE_HPP

#include <fstream>
#include <string>

class BSPFile {
public:
    BSPFile(const std::string &filename);

private:
	std::ifstream mFile;
};

#endif