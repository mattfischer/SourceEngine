#ifndef DRAW_FACE_DRAWER_HPP
#define DRAW_FACE_DRAWER_HPP

#include "Geo/Point.hpp"

#include "World/Face.hpp"

namespace Draw {

class FaceDrawer {
public:
	void newFrame();

	void setPosition(const Geo::Point &position) { mPosition = position; }

	bool drawTextures() { return mDrawTextures; }
	void setDrawTextures(bool drawTextures) { mDrawTextures = drawTextures; }

	bool drawLightmaps() { return mDrawLightmaps; }
	void setDrawLightmaps(bool drawLightmaps) { mDrawLightmaps = drawLightmaps; }

	void draw(World::Face *face);

	int numFacesDrawn() { return mNumFacesDrawn; }
	int numFacesCulled() { return mNumFacesCulled; }

private:
	Geo::Point mPosition;

	bool mDrawTextures;
	bool mDrawLightmaps;
	int mNumFacesDrawn;
	int mNumFacesCulled;
};

}

#endif