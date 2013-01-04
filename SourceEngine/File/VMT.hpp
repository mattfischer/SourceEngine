#ifndef FILE_VMT_HPP
#define FILE_VMT_HPP

#include "File/Space.hpp"
#include "File/KeyValue.hpp"

#include <string>

namespace File {

class VMT {
public:
	VMT(File *file);

	const std::string &shader();
	bool hasParameter(const std::string &parameter);
	const std::string &parameter(const std::string &parameter);

	static VMT *open(Space *space, const std::string &filename);

private:
	KeyValue mKeyValue;
};

}

#endif