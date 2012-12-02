#include "StringUtils.hpp"

namespace StringUtils {

std::string uppercase(const std::string &str)
{
	std::string ret(str);
	for(unsigned int i=0; i<ret.size(); i++) {
		ret[i] = toupper(str[i]);
	}
	return ret;
}

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

std::string trimSpaces(const std::string &string)
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

}