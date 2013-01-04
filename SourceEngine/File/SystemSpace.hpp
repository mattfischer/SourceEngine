#ifndef FILE_SYSTEM_SPACE_HPP
#define FILE_SYSTEM_SPACE_HPP

#include "File/Space.hpp"

#include <string>

namespace File {

class SystemSpace : public Space {
public:
	SystemSpace(const std::string &directory = "");

	virtual File *open(const std::string &name);
	virtual bool exists(const std::string &name);

private:
	std::string mDirectory;
};

}

#endif