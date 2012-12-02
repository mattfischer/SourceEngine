#ifndef FILE_VPK_READER_FACTORY_HPP
#define FILE_VPK_READER_FACTORY_HPP

#include "File/IReaderFactory.hpp"
#include "File/VPK.hpp"

#include <string>
#include <fstream>

namespace File {

class VPKReaderFactory : public IReaderFactory {
public:
	VPKReaderFactory(const std::string &filename);

	virtual IReader *open(const std::string &name);
	virtual bool exists(const std::string &name);

private:
	VPK mDirectory;
};

}

#endif