#ifndef WORLD_ENTITY_PROP_DYNAMIC_HPP
#define WORLD_ENTITY_PROP_DYNAMIC_HPP

#include "World/Entity/Point.hpp"

#include "World/BSP.hpp"

#include "Geo/BoxOriented.hpp"

namespace World {
class Model;

namespace Entity {
namespace Prop {

class Dynamic : public Point {
public:
	Dynamic(const Format::KeyValue::Section *section, World::Map *map);

	World::Model *model() { return mModel; }
	const Geo::BoxOriented &box() { return mBox; }

	World::BSP::Leaf *leaf() { return mLeaf; }

private:
	World::Model *mModel;
	Geo::BoxOriented mBox;
	World::BSP::Leaf *mLeaf;
};

}
}
}

#endif