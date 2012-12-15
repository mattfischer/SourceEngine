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
	return split(string, "\r\n");
}

std::vector<std::string> split(const std::string &string, const char *chars)
{
	std::vector<std::string> strings;
	size_t start = 0;
	size_t end = 0;
	while(end != string.npos) {
		end = string.find_first_of(chars, start);
		std::string substr;
		if(end == string.npos) {
			substr = string.substr(start);
		} else {
			substr = string.substr(start, end - start);
		}

		if(substr != "") {
			strings.push_back(substr);
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