#include "World/Entity/Info/PlayerStart.hpp"

namespace World {
namespace Entity {
namespace Info {

static const char *ClassName = "info_player_start";

PlayerStart::PlayerStart()
: Point(ClassName)
{
}

DECLARE_ENTITY_CLASS(ClassName, PlayerStart);

}
}
}