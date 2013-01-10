#include "World/Entity/Base.hpp"

namespace World {
namespace Entity {

Base::Base(const std::string &classname)
{
	mClassname = classname;
}

void Base::init(const Format::KeyValue::Section *section, Map *map)
{
}

}
}