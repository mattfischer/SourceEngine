#ifndef DRAW_BSP_DRAWER_HPP
#define DRAW_BSP_DRAWER_HPP

#include "World/BSP.hpp"

#include "Draw/FaceDrawer.hpp"

#include "Geo/Point.hpp"
#include "Geo/Orientation.hpp"
#include "Geo/Frustum.hpp"

namespace Draw {

class BSPDrawer {
public:
	BSPDrawer(World::BSP *bsp, FaceDrawer *faceDrawer);

	void newFrame();

	void setCameraPosition(const Geo::Point &cameraPosition) { mCameraPosition = cameraPosition; }
	void setFrustum(const Geo::Frustum &frustum) { mFrustum = frustum; }
	void setFrameTag(int frameTag) { mFrameTag = frameTag; }

	void draw(int root, const Geo::Point &position, const Geo::Orientation &orientation);

	void setFrustumCull(bool frustumCull) { mFrustumCull = frustumCull; }
	bool frustumCull() { return mFrustumCull; }

	int numFrustumCulled() { return mNumFrustumCulled; }
	int numVisLeaves() { return mNumVisLeaves; }

private:
	void drawLeaf(World::BSP::Leaf *leaf, World::BSP::Leaf *cameraLeaf, const Geo::Point &cameraPosition, const Geo::Frustum &frustum);
	void drawNode(World::BSP::Node *node, World::BSP::Leaf *cameraLeaf, const Geo::Point &cameraPosition, const Geo::Frustum &frustum);

	World::BSP *mBsp;

	Draw::FaceDrawer *mFaceDrawer;

	Geo::Point mCameraPosition;
	Geo::Frustum mFrustum;

	bool mFrustumCull;
	int mNumFrustumCulled;

	int mNumVisLeaves;

	int mFrameTag;
};

}

#endif