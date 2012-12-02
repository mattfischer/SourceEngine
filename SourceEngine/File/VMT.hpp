#ifndef FILE_VMT_HPP
#define FILE_VMT_HPP

#include "File/IReaderFactory.hpp"

#include <string>
#include <map>

namespace File {

class VMT {
public:
	VMT(IReaderFactory *factory, const std::string &name);

	typedef std::map<std::string, std::string> StringToStringMap;

	std::string &shader() { return mShader; }
	StringToStringMap &parameters() { return mParameters; }

private:
	std::string mShader;
	StringToStringMap mParameters;
};

}

#endif