#ifndef MULTI_READER_FACTORY_HPP
#define MULTI_READER_FACTORY_HPP

#include "SharedPointer.hpp"
#include "IReaderFactory.hpp"

#include <vector>

class MultiReaderFactory : public IReaderFactory {
public:
	MultiReaderFactory();

	void addFactory(IReaderFactory *factory);

	virtual bool exists(const std::string &name);
	virtual IReader *open(const std::string &name);

private:
	std::vector<IReaderFactory*> mFactories;
};

#endif