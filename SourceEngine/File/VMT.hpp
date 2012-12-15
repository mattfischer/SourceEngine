#ifndef FILE_VMT_HPP
#define FILE_VMT_HPP

#include "File/IReaderFactory.hpp"
#include "File/KeyValue.hpp"

#include <string>

namespace File {

class VMT {
public:
	VMT(IReader *reader);

	const std::string &shader();
	bool hasParameter(const std::string &parameter);
	const std::string &parameter(const std::string &parameter);

	static VMT *open(IReaderFactory *factory, const std::string &name);

private:
	KeyValue mKeyValue;
};

}

#endif