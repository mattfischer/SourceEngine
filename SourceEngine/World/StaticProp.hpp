#ifndef WORLD_STATIC_PROP_HPP
#define WORLD_STATIC_PROP_HPP

#include "File/Space.hpp"

#include "Format/BSP.hpp"
#include "Format/VHV.hpp"

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"
#include "Geo/BoxOriented.hpp"

#include "World/Model.hpp"
#include "World/ModelCache.hpp"
#include "World/BSP.hpp"

namespace World {

class StaticProp {
public:
	StaticProp(Format::BSP *file, int number, BSP *bsp, File::Space *space, ModelCache *modelCache);

	const Geo::Point &position() { return mPosition; }
	const Geo::Orientation &orientation() { return mOrientation; }
	const Geo::BoxOriented &box() { return mBox; }

	Model *model() { return mModel; }
	Format::VHV *vhv() { return mVhv; }

	size_t numLeaves() { return mNumLeaves; }
	BSP::Leaf *leaf(int leaf) { return mLeaves[leaf]; }

private:
	Geo::Point mPosition;
	Geo::Orientation mOrientation;
	Geo::BoxOriented mBox;

	Model *mModel;

	size_t mNumLeaves;
	BSP::Leaf **mLeaves;

	Format::VHV *mVhv;
};

}

#endif