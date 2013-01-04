#ifndef FILE_MULTI_SPACE_HPP
#define FILE_MULTI_SPACE_HPP

#include "File/Space.hpp"

#include <vector>

namespace File {

class MultiSpace : public Space {
public:
	MultiSpace();

	void addSpace(Space *factory);

	virtual bool exists(const std::string &name);
	virtual File *open(const std::string &name);

private:
	std::vector<Space*> mSpaces;
};

}

#endif