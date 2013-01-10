#ifndef WORLD_ENTITY_PLAYER_HPP
#define WORLD_ENTITY_PLAYER_HPP

#include "World/Entity/Point.hpp"

namespace World {
namespace Entity {

class Player : public Point {
public:
	Player(const Format::KeyValue::Section *section, Map *map);
	Player(const std::string &classname);
};

}
}
#endif