#ifndef FILE_SPACE_HPP
#define FILE_SPACE_HPP

#include "File/File.hpp"

#include <string>

namespace File {

class Space {
public:
	virtual File *open(const std::string &name) = 0;
	virtual bool exists(const std::string &name) = 0;

	char *read(const std::string &name);
};

}

#endif