#ifndef FORMAT_VMT_HPP
#define FORMAT_VMT_HPP

#include "File/Space.hpp"

#include "Format/KeyValue.hpp"

#include <string>

namespace Format {

class VMT {
public:
	VMT(File::File *file);

	const std::string &shader();
	bool hasParameter(const std::string &parameter);
	const std::string &parameter(const std::string &parameter);

	static VMT *open(File::Space *space, const std::string &filename);

private:
	KeyValue mKeyValue;
};

}

#endif