#ifndef DRAW_MODEL_DRAWER_HPP
#define DRAW_MODEL_DRAWER_HPP

#include "Format/VHV.hpp"

#include "World/Model.hpp"

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"

namespace Draw {

class ModelDrawer {
public:
	void draw(World::Model *model, const Geo::Point &position, const Geo::Orientation &orientation, Format::VHV *vhv);

	void setDrawTextures(bool drawTextures) { mDrawTextures = drawTextures; }

private:
	bool mDrawTextures;
};

}

#endif