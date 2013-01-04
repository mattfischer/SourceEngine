#ifndef FILE_ZIP_SPACE_HPP
#define FILE_ZIP_SPACE_HPP

#include "File/Space.hpp"

#include <string>
#include <map>

namespace File {

class ZipSpace : public Space {
public:
	ZipSpace(const unsigned char *data, size_t size);

	virtual File *open(const std::string &name);
	virtual bool exists(const std::string &name);

private:
	std::map<std::string, int> mOffsetMap;
	const unsigned char *mData;
};

}

#endif