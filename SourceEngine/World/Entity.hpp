#ifndef WORLD_ENTITY_HPP
#define WORLD_ENTITY_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"

#include "World/ModelCache.hpp"
#include "World/Model.hpp"

#include <string>

namespace World {

class Entity {
public:
	Entity(File::BSP *file, int number, ModelCache *modelCache);

	const Geo::Point &position() { return mPosition; }
	const Geo::Orientation &orientation() { return mOrientation; }

	World::Model *model() { return mModel; }

	const std::string &classname() { return mClassname; }

private:
	World::Model *mModel;
	Geo::Point mPosition;
	Geo::Orientation mOrientation;

	std::string mClassname;
};

}

#endif