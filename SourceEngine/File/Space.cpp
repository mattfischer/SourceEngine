#include "File/Space.hpp"

namespace File {

char *Space::read(const std::string &name)
{
	char *ret = 0;
	File *file = open(name);

	if(file) {
		ret = new char[file->size()];
		file->read(ret, file->size());
		delete file;
	}

	return ret;
}

}