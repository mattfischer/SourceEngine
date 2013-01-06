#ifndef WORLD_ENTITY_HPP
#define WORLD_ENTITY_HPP

#include "File/Space.hpp"

#include "Format/BSP.hpp"

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"
#include "Geo/BoxOriented.hpp"

#include "World/ModelCache.hpp"
#include "World/Model.hpp"
#include "World/BSP.hpp"

#include <string>

namespace World {

class Entity {
public:
	Entity(Format::BSP *file, int number, BSP *bsp, ModelCache *modelCache);
	Entity(const std::string &classname, const Geo::Point &position, const Geo::Orientation &orientation);

	const Geo::Point &position() { return mPosition; }
	void setPosition(const Geo::Point &position) { mPosition = position; }

	const Geo::Orientation &orientation() { return mOrientation; }
	void setOrientation(const Geo::Orientation &orientation) { mOrientation = orientation; }

	World::Model *model() { return mModel; }

	const std::string &classname() { return mClassname; }

	const Geo::BoxOriented &box() { return mBox; }

	BSP::Leaf *leaf() { return mLeaf; }

	int bspRoot() { return mBspRoot; }

	const std::string &skyname() { return mSkyname; }

private:
	World::Model *mModel;
	Geo::Point mPosition;
	Geo::Orientation mOrientation;

	std::string mClassname;

	Geo::BoxOriented mBox;

	BSP::Leaf *mLeaf;

	int mBspRoot;

	std::string mSkyname;
};

}

#endif