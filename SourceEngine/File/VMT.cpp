#include "File/VMT.hpp"

#include "StringUtils.hpp"

#include <vector>

namespace File {

VMT *VMT::open(Space *space, const std::string &filename)
{
	VMT *ret = 0;
	File *file = space->open(filename);
	if(file) {
		ret = new VMT(file);
		delete file;
	}

	return ret;
}

VMT::VMT(File *file)
: mKeyValue(file, 0, file->size())
{
}

const std::string &VMT::shader()
{
	return mKeyValue.section(0).title();
}

bool VMT::hasParameter(const std::string &parameter)
{
	return mKeyValue.section(0).hasParameter(parameter);
}

const std::string &VMT::parameter(const std::string &parameter)
{
	return mKeyValue.section(0).parameter(parameter);
}

}