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
	Base(const std::string &classname);

	virtual void init(const Format::KeyValue::Section *section, Map *map);

	const std::string &classname() { return mClassname; }

	static Base *create(const std::string &classname)
	{
		return Registry::instance()->create(classname);
	}

	static Base *create(const Format::KeyValue::Section *section, Map *map)
	{
		Base *ret = create(section->parameter("classname"));
		if(ret) {
			ret->init(section, map);
		}

		return ret;
	}

private:
	std::string mClassname;
};

}
}

#endif