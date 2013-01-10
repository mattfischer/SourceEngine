#include "World/Entity/Player.hpp"

namespace World {
namespace Entity {

static const char *ClassName = "player";

Player::Player()
: Point(ClassName)
{
}

DECLARE_ENTITY_CLASS(ClassName, Player);

}
}
