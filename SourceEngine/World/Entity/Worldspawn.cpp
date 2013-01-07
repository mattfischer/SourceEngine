#include "World/Entity/Worldspawn.hpp"

namespace World {
namespace Entity {

WorldSpawn::WorldSpawn(const Format::KeyValue::Section *section)
: Base(section)
{
	if(section->hasParameter("skyname")) {
		mSkyname = section->parameter("skyname");
	}
}

}
}