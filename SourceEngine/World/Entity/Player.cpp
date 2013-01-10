#include "World/Entity/Player.hpp"

namespace World {
namespace Entity {

Player::Player(const Format::KeyValue::Section *section, Map *map)
: Point(section, map)
{
}

Player::Player(const std::string &classname)
: Point(classname)
{
}

DECLARE_ENTITY_CLASS("player", Player);

}
}
