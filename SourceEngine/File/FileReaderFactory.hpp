#ifndef FILE_FILE_READER_FACTORY_HPP
#define FILE_FILE_READER_FACTORY_HPP

#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

class FileReaderFactory : public IReaderFactory {
public:
	FileReaderFactory(const std::string &directory = "");

	virtual IReader *open(const std::string &name);
	virtual bool exists(const std::string &name);

private:
	std::string mDirectory;
};

}

#endif