#include "File/MultiSpace.hpp"

namespace File {

MultiSpace::MultiSpace()
{
}

void MultiSpace::addSpace(Space *space)
{
	mSpaces.push_back(space);
}

bool MultiSpace::exists(const std::string &name)
{
	bool ret = false;
	for(unsigned int i=0; i<mSpaces.size(); i++) {
		if(mSpaces[i]->exists(name)) {
			ret = true;
			break;
		}
	}

	return ret;
}

File *MultiSpace::open(const std::string &name)
{
	File *file;
	for(unsigned int i=0; i<mSpaces.size(); i++) {
		file = mSpaces[i]->open(name);
		if(file) {
			break;
		}
	}

	return file;
}

}