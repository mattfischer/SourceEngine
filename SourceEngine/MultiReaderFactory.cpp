#include "MultiReaderFactory.hpp"

MultiReaderFactory::MultiReaderFactory()
{
}

void MultiReaderFactory::addFactory(sp<IReaderFactory> factory)
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

sp<IReader> MultiReaderFactory::open(const std::string &name)
{
	sp<IReader> ret;
	for(unsigned int i=0; i<mFactories.size(); i++) {
		ret = mFactories[i]->open(name);
		if(ret) {
			break;
		}
	}

	return ret;
}