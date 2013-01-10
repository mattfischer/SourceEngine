#include "World/Entity/Point.hpp"

#include "StringUtils.hpp"

namespace World {
namespace Entity {

Point::Point(const std::string &classname)
: Base(classname)
{
}

void Point::init(const Format::KeyValue::Section *section, Map *map)
{
	Base::init(section, map);

	if(section->hasParameter("origin")) {
		const std::string &position = section->parameter("origin");
		std::vector<std::string> posParts = StringUtils::split(position, " ");
		float x = (float)atof(posParts[0].c_str());
		float y = (float)atof(posParts[1].c_str());
		float z = (float)atof(posParts[2].c_str());
		mPosition = Geo::Point(x, y, z);
	}

	if(section->hasParameter("angles")) {
		const std::string &angles = section->parameter("angles");
		std::vector<std::string> angleParts = StringUtils::split(angles, " ");
		float pitch = (float)atof(angleParts[0].c_str());
		float yaw = (float)atof(angleParts[1].c_str());
		float roll = (float)atof(angleParts[2].c_str());
		mOrientation = Geo::Orientation(pitch, yaw, roll);
	}
}

}
}