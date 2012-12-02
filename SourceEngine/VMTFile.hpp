#ifndef VMT_FILE_HPP
#define VMT_FILE_HPP

#include "IReaderFactory.hpp"

#include <string>
#include <map>

class VMTFile {
public:
	VMTFile(IReaderFactory *factory, const std::string &name);

	const std::string &shader() { return mShader; }
	typedef std::map<std::string, std::string> StringToStringMap;
	const StringToStringMap &parameters() { return mParameters; }

private:
	std::string mShader;
	StringToStringMap mParameters;
};
#endif