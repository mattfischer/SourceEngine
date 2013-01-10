#ifndef DRAW_MODEL_DRAWER_HPP
#define DRAW_MODEL_DRAWER_HPP

#include "Format/VHV.hpp"

#include "World/Model.hpp"

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"

namespace Draw {

class ModelDrawer {
public:
	void draw(World::Model *model, const Geo::Point &position, const Geo::Orientation &orientation, Format::VHV::Header *vhv);

	void setDrawTextures(bool drawTextures) { mDrawTextures = drawTextures; }
	void setCameraPosition(const Geo::Point &cameraPosition) { mCameraPosition = cameraPosition; }

private:
	bool mDrawTextures;
	Geo::Point mCameraPosition;
};

}

#endif