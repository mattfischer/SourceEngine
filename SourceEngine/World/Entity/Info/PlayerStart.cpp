#include "World/Entity/Info/PlayerStart.hpp"

namespace World {
namespace Entity {
namespace Info {

PlayerStart::PlayerStart(const Format::KeyValue::Section *section, Map *map)
: Point(section, map)
{
}

DECLARE_ENTITY_CLASS("info_player_start", PlayerStart);

}
}
}