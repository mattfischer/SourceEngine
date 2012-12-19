#ifndef FILE_MDL_HPP
#define FILE_MDL_HPP

#include "File/IReader.hpp"
#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

class MDL {
public:
	MDL(IReader *reader);

	static MDL *open(IReaderFactory *factory, const std::string &name);
};

}
#endif