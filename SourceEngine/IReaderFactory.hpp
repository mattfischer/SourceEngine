#ifndef IREADER_FACTORY_HPP
#define IREADER_FACTORY_HPP

#include <string>

class IReader;

class IReaderFactory {
public:
	virtual IReader *open(const std::string &name) = 0;
	virtual bool exists(const std::string &name) = 0;
};

#endif