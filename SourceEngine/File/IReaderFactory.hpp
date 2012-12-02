#ifndef FILE_IREADER_FACTORY_HPP
#define FILE_IREADER_FACTORY_HPP

#include "File/IReader.hpp"

#include <string>

namespace File {

class IReaderFactory {
public:
	virtual IReader *open(const std::string &name) = 0;
	virtual bool exists(const std::string &name) = 0;
};

}

#endif