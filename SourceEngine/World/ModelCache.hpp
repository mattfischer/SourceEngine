#ifndef WORLD_MODEL_CACHE_HPP
#define WORLD_MODEL_CACHE_HPP

#include "File/Space.hpp"

#include "World/Model.hpp"

#include <string>
#include <map>

namespace World {

class ModelCache {
public:
	ModelCache(File::Space *factory);

	Model *open(const std::string &filename);

private:
	File::Space *mSpace;
	std::map<std::string, Model*> mMap;
};

}

#endif