#ifndef WORLD_MAP_HPP
#define WORLD_MAP_HPP

#include "World/Face.hpp"
#include "World/Material.hpp"
#include "World/BSP.hpp"
#include "World/Entity.hpp"
#include "World/StaticProp.hpp"
#include "World/Skybox.hpp"

#include "File/Space.hpp"

#include <string>

namespace World {

class Map {
public:
	Map(File::Space *space, const std::string &filename);

	BSP *bsp() { return mBsp; }

	Entity *playerStart() { return mPlayerStart; }

	size_t numEntities() { return mNumEntities; }
	Entity *entity(int entity) { return mEntities[entity]; }

	size_t numStaticProps() { return mNumStaticProps; }
	StaticProp *staticProp(int staticProp) { return mStaticProps[staticProp]; }

	Entity *worldspawn() { return mEntities[0]; }

private:
	size_t mNumMaterials;
	Material **mMaterials;

	size_t mNumFaces;
	Face **mFaces;

	size_t mNumEntities;
	Entity **mEntities;

	size_t mNumStaticProps;
	StaticProp **mStaticProps;

	BSP *mBsp;

	Entity *mPlayerStart;

	Skybox *mSkybox;
};

}

#endif