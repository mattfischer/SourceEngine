#ifndef FILE_VPK_SPACE_HPP
#define FILE_VPK_SPACE_HPP

#include "File/Space.hpp"
#include "File/VPK.hpp"

#include <string>
#include <fstream>

namespace File {

class VPKSpace : public Space {
public:
	VPKSpace(const std::string &filename);

	virtual File *open(const std::string &name);
	virtual bool exists(const std::string &name);

private:
	VPK mDirectory;
};

}

#endif