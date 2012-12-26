#ifndef DRAW_BSP_DRAWER_HPP
#define DRAW_BSP_DRAWER_HPP

#include "World/BSP.hpp"

#include "Draw/FaceDrawer.hpp"

#include "Geo/Point.hpp"
#include "Geo/Frustum.hpp"

namespace Draw {

class BSPDrawer {
public:
	BSPDrawer(World::BSP *bsp, FaceDrawer *faceDrawer);

	void setPosition(const Geo::Point &position);
	void setFrustum(const Geo::Frustum &frustum);

	void draw();

	void setFrustumCull(bool frustumCull) { mFrustumCull = frustumCull; }
	bool frustumCull() { return mFrustumCull; }

	int numFrustumCulled() { return mNumFrustumCulled; }
	int numVisLeaves() { return mNumVisLeaves; }

private:
	void drawLeaf(const World::BSP::Leaf *leaf);
	void drawNode(const World::BSP::Node *node);

	World::BSP *mBsp;

	Draw::FaceDrawer *mFaceDrawer;

	World::BSP::Leaf *mCameraLeaf;

	Geo::Point mPosition;
	Geo::Frustum mFrustum;

	bool mFrustumCull;
	int mNumFrustumCulled;

	int mNumVisLeaves;
};

}

#endif