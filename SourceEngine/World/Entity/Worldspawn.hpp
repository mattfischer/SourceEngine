#ifndef WORLD_ENTITY_WORLDSPAWN_HPP
#define WORLD_ENTITY_WORLDSPAWN_HPP

#include "World/Entity/Base.hpp"

namespace World {
namespace Entity {

class WorldSpawn : public Base {
public:
	WorldSpawn();

	virtual void init(const Format::KeyValue::Section *section, Map *map);

	const std::string &skyname() { return mSkyname; }

private:
	std::string mSkyname;
};

}
}

#endif