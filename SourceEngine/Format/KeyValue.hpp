#ifndef FORMAT_KEY_VALUE_HPP
#define FORMAT_KEY_VALUE_HPP

#include "File/File.hpp"

#include "StringUtils.hpp"

#include <string>
#include <map>
#include <vector>

namespace Format {

class KeyValue {
public:
	KeyValue(File::File *file, int offset, int length);

	class Section {
	public:
		typedef std::map<std::string, std::string> StringMap;

		Section(const std::string &title, const StringMap &parameters)
			: mTitle(title), mParameters(parameters)
		{}

		const std::string &title() const { return mTitle; }
		bool hasParameter(const std::string &parameter) const;
		const std::string &parameter(const std::string &parameter) const;

	private:
		std::string mTitle;
		std::map<std::string, std::string> mParameters;
	};

	const Section &section(int section) { return mSections[section]; }
	size_t numSections() { return mSections.size(); }

private:
	std::vector<Section> mSections;
};

}

#endif