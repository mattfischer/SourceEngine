#ifndef MULTI_READER_FACTORY_HPP
#define MULTI_READER_FACTORY_HPP

#include "SharedPointer.hpp"
#include "IReaderFactory.hpp"

#include <vector>

class MultiReaderFactory : public IReaderFactory {
public:
	MultiReaderFactory();

	void addFactory(sp<IReaderFactory> factory);

	virtual bool exists(const std::string &name);
	virtual sp<IReader> open(const std::string &name);

private:
	std::vector< sp<IReaderFactory> > mFactories;
};

#endif