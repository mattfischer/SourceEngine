#ifndef WORLD_ENTITY_REGISTRY_HPP
#define WORLD_ENTITY_REGISTRY_HPP

#include "Format/KeyValue.hpp"

#include <string>
#include <map>

namespace World {
class Map;

namespace Entity {

class Base;

class Registry {
public:
	class CreatorBase {
	public:
		virtual Base *create(const Format::KeyValue::Section *section, World::Map *map) = 0;
		virtual Base *create(const std::string &classname) = 0;
	};

	static Registry *instance();

	Base *create(const Format::KeyValue::Section *section, World::Map *map);
	Base *create(const std::string &classname);

	void registerCreator(const std::string &classname, CreatorBase *creator);
	
	template<typename T>
	class Creator : public CreatorBase {
	public:
		Creator(const std::string &classname)
		{
			Registry::instance()->registerCreator(classname, this);
		}

		Base *create(const Format::KeyValue::Section *section, Map *map)
		{
			return new T(section, map);
		}

		Base *create(const std::string &classname)
		{
			return new T(classname);
		}
	};

private:
	std::map<std::string, CreatorBase*> mCreators;
};

}
}

#define DECLARE_ENTITY_CLASS(classname, cls) \
	static World::Entity::Registry::Creator<cls> creator(classname);

#endif