#ifndef FILE_VMT_HPP
#define FILE_VMT_HPP

#include "File/IReaderFactory.hpp"

#include <string>
#include <map>

namespace File {

class VMT {
public:
	VMT(IReaderFactory *factory, const std::string &name);

	const std::string &shader() { return mShader; }
	typedef std::map<std::string, std::string> StringToStringMap;
	const StringToStringMap &parameters() { return mParameters; }

private:
	std::string mShader;
	StringToStringMap mParameters;
};

}

#endif