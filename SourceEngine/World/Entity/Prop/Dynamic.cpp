#include "World/Entity/Prop/Dynamic.hpp"

#include "World/Map.hpp"

namespace World {
namespace Entity {
namespace Prop {

static const char *ClassName = "prop_dynamic";

Dynamic::Dynamic()
: Point(ClassName)
{
}

void Dynamic::init(const Format::KeyValue::Section *section, World::Map *map)
{
	Point::init(section, map);

	mModel = 0;
	if(section->hasParameter("model")) {
		const std::string &fileName = section->parameter("model");
		mModel = map->modelCache()->open(fileName);
	}

	if(mModel) {
		Geo::Transformation transformation = Geo::Transformation::translateRotate(position(), orientation());
		mBox = transformation * mModel->box();
	}

	mLeaf = map->bsp()->leafForPoint(0, position());
}

DECLARE_ENTITY_CLASS(ClassName, Dynamic);

}
}
}
