#ifndef WORLD_ENTITY_PROP_STATIC_HPP
#define WORLD_ENTITY_PROP_STATIC_HPP

#include "World/Entity/Point.hpp"
#include "World/BSP.hpp"

#include "Format/BSP.hpp"
#include "Format/VHV.hpp"

#include "Geo/BoxOriented.hpp"

namespace World {

class Map;
class Model;

namespace Entity {
namespace Prop {

class Static : public World::Entity::Point {
public:
	Static(Format::BSP *file, int number, World::Map *map);
	Static(const std::string &classname);

	size_t numLeaves() { return mNumLeaves; }
	World::BSP::Leaf *leaf(int leaf) { return mLeaves[leaf]; }

	World::Model *model() { return mModel; }

	const Geo::BoxOriented &box() { return mBox; }

	Format::VHV::Header *vhv() { return mVhv; }

private:
	World::Model *mModel;
	Geo::BoxOriented mBox;

	size_t mNumLeaves;
	BSP::Leaf **mLeaves;

	Format::VHV::Header *mVhv;
};

}
}
}

#endif