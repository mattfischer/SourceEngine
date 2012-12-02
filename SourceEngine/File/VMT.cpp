#include "File/VMT.hpp"

#include <vector>

namespace File {

std::vector<std::string> splitLines(const std::string &string)
{
	std::vector<std::string> strings;
	size_t start = 0;
	size_t end = 0;
	while(end != string.npos) {
		end = string.find_first_of("\r\n", start);
		std::string line;
		if(end == string.npos) {
			line = string.substr(start);
		} else {
			line = string.substr(start, end - start);
		}

		if(line != "") {
			strings.push_back(line);
		}
		start = end + 1;
	}

	return strings;
}

std::string trimString(const std::string &string)
{
	size_t start = string.find_first_not_of(" ");
	size_t end = string.find_last_not_of(" ");
	std::string ret;

	if(end == string.npos) {
		ret = string.substr(start);
	} else {
		ret = string.substr(start, end - start + 1);
	}

	return ret;
}

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

	std::vector<std::string> lines = splitLines(source);
	mShader = trimString(lines[0]);
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

		mParameters[key] = value;
	}
}

}