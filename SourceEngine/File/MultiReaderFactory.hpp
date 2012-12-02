#ifndef FILE_MULTI_READER_FACTORY_HPP
#define FILE_MULTI_READER_FACTORY_HPP

#include "File/IReaderFactory.hpp"

#include <vector>

namespace File {

class MultiReaderFactory : public IReaderFactory {
public:
	MultiReaderFactory();

	void addFactory(IReaderFactory *factory);

	virtual bool exists(const std::string &name);
	virtual IReader *open(const std::string &name);

private:
	std::vector<IReaderFactory*> mFactories;
};

}

#endif