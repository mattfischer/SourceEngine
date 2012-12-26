#ifndef WORLD_MAP_HPP
#define WORLD_MAP_HPP

#include "World/Face.hpp"
#include "World/Material.hpp"
#include "World/BSP.hpp"
#include "World/Entity.hpp"

#include "File/IReaderFactory.hpp"

#include <string>

namespace World {

class Map {
public:
	Map(File::IReaderFactory *factory, const std::string &filename);

	BSP *bsp() { return mBsp; }

	Entity *playerStart() { return mPlayerStart; }

	size_t numEntities() { return mNumEntities; }
	Entity *entity(int entity) { return mEntities[entity]; }

private:
	size_t mNumMaterials;
	Material **mMaterials;

	size_t mNumFaces;
	Face **mFaces;

	size_t mNumEntities;
	Entity **mEntities;

	BSP *mBsp;

	Entity *mPlayerStart;
};

}

#endif