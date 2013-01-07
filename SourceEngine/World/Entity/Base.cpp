#include "World/Entity/Base.hpp"

namespace World {
namespace Entity {

Base::Base(const Format::KeyValue::Section *section, Map *map)
{
	mClassname = section->parameter("classname");
}

Base::Base(const std::string &classname)
{
	mClassname = classname;
}

}
}