#ifndef FILE_VVD_HPP
#define FILE_VVD_HPP

#include "File/IReader.hpp"
#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

class VVD {
public:
	VVD(IReader *reader);

	static VVD *open(IReaderFactory *factory, const std::string &name);
};

}
#endif