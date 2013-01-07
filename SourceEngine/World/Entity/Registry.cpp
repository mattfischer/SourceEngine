#include "World/Entity/Registry.hpp"

namespace World {
namespace Entity {

Registry *Registry::instance()
{
	static Registry inst;

	return &inst;
}

Base *Registry::create(const Format::KeyValue::Section *section, Map *map)
{
	const std::string &classname = section->parameter("classname");
	Base *ret = 0;
	if(mCreators.find(classname) != mCreators.end()) {
		ret = mCreators[classname]->create(section, map);
	}

	return ret;
}

void Registry::registerCreator(const std::string &classname, CreatorBase *creator)
{
	mCreators[classname] = creator;
}

}
}