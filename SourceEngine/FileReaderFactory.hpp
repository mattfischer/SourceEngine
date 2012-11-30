#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include "IReaderFactory.hpp"

#include <string>

class FileReaderFactory : public IReaderFactory {
public:
	FileReaderFactory(const std::string &directory = "");

	virtual IReader *open(const std::string &name);
	virtual bool exists(const std::string &name);

private:
	std::string mDirectory;
};

#endif