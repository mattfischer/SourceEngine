#include "World/Entity/Prop/Static.hpp"

#include "World/Map.hpp"
#include "World/Model.hpp"

#include <sstream>

namespace World {
namespace Entity {
namespace Prop {

Static::Static(Format::BSP *file, int number, World::Map *map)
: Point("prop_static")
{
	const Format::BSP::StaticProp &fileStaticProp = file->staticProp(number);

	setPosition(Geo::Point(fileStaticProp.origin.x, fileStaticProp.origin.y, fileStaticProp.origin.z));
	setOrientation(Geo::Orientation(fileStaticProp.angles.x, fileStaticProp.angles.y, fileStaticProp.angles.z));

	mNumLeaves = fileStaticProp.leafCount;
	mLeaves = new BSP::Leaf*[mNumLeaves];
	for(unsigned int i=0; i<mNumLeaves; i++) {
		mLeaves[i] = map->bsp()->leaf(file->staticPropLeaf(fileStaticProp.firstLeaf + i));
	}

	const std::string &fileName = file->staticPropName(fileStaticProp.propType);

	mModel = map->modelCache()->open(fileName);

	if(mModel) {
		Geo::Transformation transformation = Geo::Transformation::translateRotate(position(), orientation());
		mBox = transformation * mModel->box();
	}

	std::stringstream ss;
	ss << "sp_hdr_" << number << ".vhv";
	mVhv = Format::VHV::open(map->space(), ss.str());
}

}
}
}