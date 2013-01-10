#include "World/Entity/Worldspawn.hpp"

namespace World {
namespace Entity {

static const char *ClassName = "worldspawn";

WorldSpawn::WorldSpawn()
: Base(ClassName)
{
}

void WorldSpawn::init(const Format::KeyValue::Section *section, World::Map *map)
{
	Base::init(section, map);

	if(section->hasParameter("skyname")) {
		mSkyname = section->parameter("skyname");
	}
}

DECLARE_ENTITY_CLASS(ClassName, WorldSpawn);

}
}