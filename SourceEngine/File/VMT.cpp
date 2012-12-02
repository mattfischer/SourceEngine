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
{
	char *buffer = new char[reader->size() + 1];
	reader->read(buffer, reader->size());
	buffer[reader->size()] = '\0';

	std::string source(buffer);
	delete[] buffer;

	std::vector<std::string> lines = StringUtils::splitLines(source);
	mShader = StringUtils::trimSpaces(lines[0]);
	for(unsigned int i=2; i<lines.size(); i++) {
		std::string &line = lines[i];

		if(line == "}") {
			break;
		}

		std::string key;
		std::string value;

		size_t start;
		size_t end;

		if(line[0] == '\"') {
			start = 1;
			end = line.find('\"', start);
		} else {
			start = 0;
			end = line.find(" ");
		}

		key = line.substr(start, end - start);
		start = end + 1;

		while(line[start] == ' ') {
			start++;
		}

		if(line[start] == '\"') {
			start++;
			end = line.find('\"', start);
			value = line.substr(start, end - start);
		} else {
			value = line.substr(start);
		}

		mParameters[StringUtils::uppercase(key)] = value;
	}
}

const std::string &VMT::parameter(const std::string &parameter)
{
	return mParameters[StringUtils::uppercase(parameter)];
}

}