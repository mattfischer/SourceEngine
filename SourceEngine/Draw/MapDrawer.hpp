#ifndef DRAW_MAP_DRAWER_HPP
#define DRAW_MAP_DRAWER_HPP

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"
#include "Geo/Frustum.hpp"

#include "World/Map.hpp"

#include "Draw/BSPDrawer.hpp"
#include "Draw/FaceDrawer.hpp"
#include "Draw/ModelDrawer.hpp"

namespace Draw {

class MapDrawer {
public:
	MapDrawer(World::Map *map, const Geo::Frustum &frustum);

	void draw(const Geo::Point &position, const Geo::Orientation &orientation);

	void setUpdateFrustum(bool updateFrustum) { mUpdateFrustum = updateFrustum; }
	bool updateFrustum() { return mUpdateFrustum; }

	Draw::ModelDrawer *modelDrawer() { return mModelDrawer; }
	Draw::BSPDrawer *bspDrawer() { return mBspDrawer; }
	Draw::FaceDrawer *faceDrawer() { return mFaceDrawer; }

private:
	World::Map *mMap;

	Geo::Frustum mStartFrustum;

	bool mUpdateFrustum;

	Geo::Frustum mFrustum;

	int mFrameTag;

	Draw::ModelDrawer *mModelDrawer;
	Draw::BSPDrawer *mBspDrawer;
	Draw::FaceDrawer *mFaceDrawer;
};
}
#endif