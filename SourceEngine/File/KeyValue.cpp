#include "File/KeyValue.hpp"

#include "StringUtils.hpp"

namespace File {

KeyValue::KeyValue(File *file, int offset, int length)
{
	char *buffer = new char[length + 1];
	file->seek(offset);
	file->read(buffer, length);
	buffer[length] = '\0';

	std::string source(buffer);
	delete[] buffer;

	std::vector<std::string> lines = StringUtils::splitLines(source);
	unsigned int ln = 0;
	while(ln < lines.size()) {
		std::string title;

		if(lines[ln] != "{") {
			title = StringUtils::trimSpaces(lines[ln++]);
		}

		std::map<std::string, std::string> parameters;
		int braceCount = 0;
		while(true) {
			std::string &line = lines[ln++];

			if(line == "{") {
				braceCount++;
				continue;
			}

			else if(line == "}") {
				braceCount--;
				if(braceCount == 0) {
					break;
				}
				continue;
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

			parameters[StringUtils::uppercase(key)] = value;
		}

		mSections.push_back(Section(title, parameters));
	}
}

bool KeyValue::Section::hasParameter(const std::string &parameter) const
{
	return mParameters.find(StringUtils::uppercase(parameter)) != mParameters.end();
}

const std::string &KeyValue::Section::parameter(const std::string &parameter) const
{
	return mParameters.find(StringUtils::uppercase(parameter))->second;
}

}