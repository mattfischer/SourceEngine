#ifndef WORLD_ENTITY_POINT_HPP
#define WORLD_ENTITY_POINT_HPP

#include "World/Entity/Base.hpp"

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"

namespace World {
namespace Entity {

class Point : public Base {
public:
	Point(const Format::KeyValue::Section *section);
	Point(const std::string &classname);

	const Geo::Point &position() { return mPosition; }
	void setPosition(const Geo::Point &position) { mPosition = position; }

	const Geo::Orientation &orientation() { return mOrientation; }
	void setOrientation(const Geo::Orientation &orientation) { mOrientation = orientation; }

private:
	Geo::Point mPosition;
	Geo::Orientation mOrientation;
};

}
}
#endif