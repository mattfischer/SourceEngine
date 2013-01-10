#ifndef WORLD_MAP_HPP
#define WORLD_MAP_HPP

#include "World/Face.hpp"
#include "World/Material.hpp"
#include "World/BSP.hpp"
#include "World/Skybox.hpp"
#include "World/ModelCache.hpp"
#include "World/Entity/Base.hpp"
#include "World/Entity/Player.hpp"
#include "World/Entity/Prop/Static.hpp"
#include "World/Entity/Worldspawn.hpp"

#include "File/Space.hpp"

#include <string>

namespace World {

class Map {
public:
	Map(File::Space *space, const std::string &filename);

	BSP *bsp() { return mBsp; }

	Entity::Player *player() { return mPlayer; }

	size_t numEntities() { return mNumEntities; }
	Entity::Base *entity(int entity) { return mEntities[entity]; }

	size_t numStaticProps() { return mNumStaticProps; }
	Entity::Prop::Static *staticProp(int staticProp) { return mStaticProps[staticProp]; }

	Entity::WorldSpawn *worldSpawn() { return mWorldSpawn; }

	Skybox *skybox() { return mSkybox; }

	ModelCache *modelCache() { return mModelCache; }

	File::Space *space() { return mSpace; }

private:
	size_t mNumMaterials;
	Material **mMaterials;

	size_t mNumFaces;
	Face **mFaces;

	size_t mNumEntities;
	Entity::Base **mEntities;

	size_t mNumStaticProps;
	Entity::Prop::Static **mStaticProps;

	BSP *mBsp;

	Entity::Player *mPlayer;

	Entity::WorldSpawn *mWorldSpawn;
	Skybox *mSkybox;

	ModelCache *mModelCache;

	File::Space *mSpace;
};

}

#endif