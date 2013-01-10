#ifndef WORLD_ENTITY_INFO_PLAYER_START_HPP
#define WORLD_ENTITY_INFO_PLAYER_START_HPP

#include "World/Entity/Point.hpp"

namespace World {
namespace Entity {
namespace Info {

class PlayerStart : public Point {
public:
	PlayerStart(const Format::KeyValue::Section *section, Map *map);
	PlayerStart(const std::string &classname);
};

}
}
}

#endif