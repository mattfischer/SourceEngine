#include "World/StaticProp.hpp"

#include <sstream>

namespace World {

StaticProp::StaticProp(Format::BSP *file, int number, BSP *bsp, File::Space *space, ModelCache *modelCache)
{
	const Format::BSP::StaticProp &fileStaticProp = file->staticProp(number);
	const std::string &fileName = file->staticPropName(fileStaticProp.propType);

	mModel = modelCache->open(fileName);

	mNumLeaves = fileStaticProp.leafCount;
	mLeaves = new BSP::Leaf*[mNumLeaves];
	for(unsigned int i=0; i<mNumLeaves; i++) {
		mLeaves[i] = bsp->leaf(file->staticPropLeaf(fileStaticProp.firstLeaf + i));
	}

	mPosition = Geo::Point(fileStaticProp.origin.x, fileStaticProp.origin.y, fileStaticProp.origin.z);
	mOrientation = Geo::Orientation(fileStaticProp.angles.x, fileStaticProp.angles.y, fileStaticProp.angles.z);

	if(mModel) {
		Geo::Transformation transformation = Geo::Transformation::translate(mPosition);
		transformation = transformation * Geo::Transformation::rotateZ(mOrientation.yaw());
		transformation = transformation * Geo::Transformation::rotateX(mOrientation.pitch());
		transformation = transformation * Geo::Transformation::rotateY(mOrientation.roll());

		mBox = transformation * mModel->box();
	}

	std::stringstream ss;
	ss << "sp_hdr_" << number << ".vhv";
	mVhv = Format::VHV::open(space, ss.str());
}

}