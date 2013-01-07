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

DECLARE_ENTITY_CLASS("worldspawn", WorldSpawn);

}
}