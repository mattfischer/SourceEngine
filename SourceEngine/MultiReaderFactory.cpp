#include "MultiReaderFactory.hpp"

MultiReaderFactory::MultiReaderFactory()
{
}

void MultiReaderFactory::addFactory(IReaderFactory *factory)
{
	mFactories.push_back(factory);
}

bool MultiReaderFactory::exists(const std::string &name)
{
	bool ret = false;
	for(unsigned int i=0; i<mFactories.size(); i++) {
		if(mFactories[i]->exists(name)) {
			ret = true;
			break;
		}
	}

	return ret;
}

IReader *MultiReaderFactory::open(const std::string &name)
{
	IReader *ret = 0;
	for(unsigned int i=0; i<mFactories.size(); i++) {
		ret = mFactories[i]->open(name);
		if(ret) {
			break;
		}
	}

	return ret;
}