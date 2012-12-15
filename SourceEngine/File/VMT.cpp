#include "File/VMT.hpp"

#include "StringUtils.hpp"

#include <vector>

namespace File {

VMT *VMT::open(IReaderFactory *factory, const std::string &name)
{
	VMT *ret = 0;
	std::string filename = "materials/" + name + ".vmt";
	IReader *reader = factory->open(filename);
	if(reader) {
		ret = new VMT(reader);
		delete reader;
	}

	return ret;
}

VMT::VMT(IReader *reader)
: mKeyValue(reader, 0, reader->size())
{
}

const std::string &VMT::shader()
{
	return mKeyValue.section(0).title();
}

const std::string &VMT::parameter(const std::string &parameter)
{
	return mKeyValue.section(0).parameter(parameter);
}

}