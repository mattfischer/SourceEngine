#ifndef FILE_VMT_HPP
#define FILE_VMT_HPP

#include "File/IReaderFactory.hpp"

#include <string>
#include <map>

namespace File {

class VMT {
public:
	VMT(IReader *reader);

	const std::string &shader() { return mShader; }
	const std::string &parameter(const std::string &parameter) { return mParameters[parameter]; }

	static VMT *open(IReaderFactory *factory, const std::string &name);

private:
	std::string mShader;
	std::map<std::string, std::string> mParameters;
};

}

#endif