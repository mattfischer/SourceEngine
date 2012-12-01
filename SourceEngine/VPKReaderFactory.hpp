#ifndef VPK_READER_FACTORY_HPP
#define VPK_READER_FACTORY_HPP

#include "IReaderFactory.hpp"
#include "VPKDirectory.hpp"

#include <string>
#include <fstream>

class VPKReaderFactory : public IReaderFactory {
public:
	VPKReaderFactory(const std::string &filename);

	virtual sp<IReader> open(const std::string &name);
	virtual bool exists(const std::string &name);

private:
	VPKDirectory mDirectory;
};
#endif