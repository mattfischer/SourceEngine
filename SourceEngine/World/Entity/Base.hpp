#ifndef WORLD_ENTITY_BASE_HPP
#define WORLD_ENTITY_BASE_HPP

#include "World/Entity/Registry.hpp"

#include "Format/KeyValue.hpp"

#include <string>

namespace World {
class Map;

namespace Entity {

class Base {
public:
	Base(const Format::KeyValue::Section *section, Map *map);
	Base(const std::string &classname);

	const std::string &classname() { return mClassname; }

	static Base *create(const Format::KeyValue::Section *section, Map *map)
	{
		return Registry::instance()->create(section, map);
	}

	static Base *create(const std::string &classname)
	{
		return Registry::instance()->create(classname);
	}

private:
	std::string mClassname;
};

}
}

#endif