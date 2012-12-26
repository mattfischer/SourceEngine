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

	World::BSP::TreeItem *cursor = mBsp->rootNode();
	while(cursor->type == World::BSP::TreeItem::TypeNode) {
		int child;
		World::BSP::Node *node = (World::BSP::Node*)cursor;

		if(node->plane.pointInFront(position)) {
			child = 0;
		} else {
			child = 1;
		}

		cursor = node->children[child];
	}

	mCameraLeaf = (World::BSP::Leaf*)cursor;
}

void BSPDrawer::setFrustum(const Geo::Frustum &frustum)
{
	mFrustum = frustum;
}

void BSPDrawer::draw()
{
	mNumFrustumCulled = 0;
	mNumVisLeaves = 0;

	drawNode(mBsp->rootNode());
}

void BSPDrawer::drawLeaf(const World::BSP::Leaf *leaf)
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
}

void BSPDrawer::drawNode(const World::BSP::Node *node)
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