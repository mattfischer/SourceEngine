#include "Draw/BSPDrawer.hpp"

namespace Draw {

BSPDrawer::BSPDrawer(World::BSP *bsp, FaceDrawer *faceDrawer)
{
	mBsp = bsp;
	mFaceDrawer = faceDrawer;
	mCameraLeaf = 0;
}

void BSPDrawer::setPosition(const Geo::Point &position)
{
	mPosition = position;
	mCameraLeaf = mBsp->leafForPoint(mPosition);
}

void BSPDrawer::setFrustum(const Geo::Frustum &frustum)
{
	mFrustum = frustum;
}

void BSPDrawer::setFrameTag(int frameTag)
{
	mFrameTag = frameTag;
}

void BSPDrawer::draw()
{
	mNumFrustumCulled = 0;
	mNumVisLeaves = 0;

	drawNode(mBsp->rootNode());
}

void BSPDrawer::drawLeaf(World::BSP::Leaf *leaf)
{
	if(leaf->number == -1) {
		return;
	}

	if(mCameraLeaf && !mBsp->leafVisibleFrom(leaf, mCameraLeaf)) {
		return;
	}

	mNumVisLeaves++;

	if(mFrustumCull && mFrustum.boxOutside(leaf->bbox)) {
		mNumFrustumCulled++;
		return;
	}

	for(int j=0; j<leaf->numFaces; j++) {
		mFaceDrawer->draw(leaf->faces[j]);
	}
	leaf->frameTag = mFrameTag;
}

void BSPDrawer::drawNode(World::BSP::Node *node)
{
	if(mFrustumCull && mFrustum.boxOutside(node->bbox)) {
		mNumFrustumCulled++;
		return;
	}

	for(int i=0; i<2; i++) {
		int start = node->plane.pointInFront(mPosition) ? 0 : 1;
		int j = (i + start) % 2;

		switch(node->children[j]->type) {
			case World::BSP::TreeItem::TypeNode:
				drawNode((World::BSP::Node*)node->children[j]);
				break;

			case World::BSP::TreeItem::TypeLeaf:
				drawLeaf((World::BSP::Leaf*)node->children[j]);
				break;
		}
	}
}

}