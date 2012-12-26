#ifndef WORLD_MODEL_CACHE_HPP
#define WORLD_MODEL_CACHE_HPP

#include "File/IReaderFactory.hpp"

#include "World/Model.hpp"

#include <string>
#include <map>

namespace World {

class ModelCache {
public:
	ModelCache(File::IReaderFactory *factory);

	Model *open(const std::string &filename);

private:
	File::IReaderFactory *mFactory;
	std::map<std::string, Model*> mMap;
};

}

#endif