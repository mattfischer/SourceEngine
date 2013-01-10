#include "World/Entity/Worldspawn.hpp"

namespace World {
namespace Entity {

WorldSpawn::WorldSpawn(const Format::KeyValue::Section *section, World::Map *map)
: Base(section, map)
{
	if(section->hasParameter("skyname")) {
		mSkyname = section->parameter("skyname");
	}
}

WorldSpawn::WorldSpawn(const std::string &classname)
: Base(classname)
{
}

DECLARE_ENTITY_CLASS("worldspawn", WorldSpawn);

}
}