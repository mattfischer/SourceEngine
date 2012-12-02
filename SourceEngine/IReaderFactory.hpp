#ifndef IREADER_FACTORY_HPP
#define IREADER_FACTORY_HPP

#include "IReader.hpp"

#include <string>

class IReaderFactory {
public:
	virtual IReader *open(const std::string &name) = 0;
	virtual bool exists(const std::string &name) = 0;
};

#endif