#include "Draw/BSPDrawer.hpp"

#include <GL/glew.h>

namespace Draw {

BSPDrawer::BSPDrawer(World::BSP *bsp, FaceDrawer *faceDrawer)
{
	mBsp = bsp;
	mFaceDrawer = faceDrawer;
	mCameraLeaf = 0;
}

void BSPDrawer::newFrame()
{
	mNumFrustumCulled = 0;
	mNumVisLeaves = 0;
}

void BSPDrawer::setPosition(const Geo::Point &position)
{
	mPosition = position;
	mCameraLeaf = mBsp->leafForPoint(0, mPosition);
}

void BSPDrawer::setFrustum(const Geo::Frustum &frustum)
{
	mFrustum = frustum;
}

void BSPDrawer::setFrameTag(int frameTag)
{
	mFrameTag = frameTag;
}

void BSPDrawer::draw(int root, const Geo::Point &position, const Geo::Orientation &orientation)
{
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	glRotatef(orientation.yaw(), 0, 0, 1);
	glRotatef(orientation.pitch(), 0, -1, 0);
	glRotatef(orientation.roll(), -1, 0, 0);

	World::BSP::Leaf *cameraLeaf;
	if(root == 0) {
		cameraLeaf = mCameraLeaf;
	} else {
		cameraLeaf = 0;
	}

	drawNode(mBsp->root(root), cameraLeaf);

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void BSPDrawer::drawLeaf(World::BSP::Leaf *leaf, World::BSP::Leaf *cameraLeaf)
{
	if(cameraLeaf && !mBsp->leafVisibleFrom(leaf, cameraLeaf)) {
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

void BSPDrawer::drawNode(World::BSP::Node *node, World::BSP::Leaf *cameraLeaf)
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
				drawNode((World::BSP::Node*)node->children[j], cameraLeaf);
				break;

			case World::BSP::TreeItem::TypeLeaf:
				drawLeaf((World::BSP::Leaf*)node->children[j], cameraLeaf);
				break;
		}
	}
}

}