#ifndef WORLD_ENTITY_BASE_HPP
#define WORLD_ENTITY_BASE_HPP

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

private:
	std::string mClassname;
};

}
}

#endif