#include "Draw/BSPDrawer.hpp"

#include <GL/glew.h>

namespace Draw {

BSPDrawer::BSPDrawer(World::BSP *bsp, FaceDrawer *faceDrawer)
{
	mBsp = bsp;
	mFaceDrawer = faceDrawer;
}

void BSPDrawer::newFrame()
{
	mNumFrustumCulled = 0;
	mNumVisLeaves = 0;
}

void BSPDrawer::draw(int root, const Geo::Point &position, const Geo::Orientation &orientation)
{
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	glRotatef(orientation.yaw(), 0, 0, 1);
	glRotatef(orientation.pitch(), 0, -1, 0);
	glRotatef(orientation.roll(), -1, 0, 0);

	Geo::Transformation transformation = Geo::Transformation::translateRotate(position, orientation).inverse();

	Geo::Point cameraPosition = transformation * mCameraPosition;
	Geo::Frustum frustum = mFrustum * transformation;

	World::BSP::Leaf *cameraLeaf = mBsp->leafForPoint(root, cameraPosition);

	drawNode(mBsp->root(root), cameraLeaf, cameraPosition, frustum);

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void BSPDrawer::drawLeaf(World::BSP::Leaf *leaf, World::BSP::Leaf *cameraLeaf, const Geo::Point &cameraPosition, const Geo::Frustum &frustum)
{
	if(cameraLeaf && !mBsp->leafVisibleFrom(leaf, cameraLeaf)) {
		return;
	}

	mNumVisLeaves++;

	if(mFrustumCull && frustum.boxOutside(leaf->bbox)) {
		mNumFrustumCulled++;
		return;
	}

	for(int j=0; j<leaf->numFaces; j++) {
		mFaceDrawer->draw(leaf->faces[j], cameraPosition);
	}
	leaf->frameTag = mFrameTag;
}

void BSPDrawer::drawNode(World::BSP::Node *node, World::BSP::Leaf *cameraLeaf, const Geo::Point &cameraPosition, const Geo::Frustum &frustum)
{
	if(mFrustumCull && frustum.boxOutside(node->bbox)) {
		mNumFrustumCulled++;
		return;
	}

	for(int i=0; i<2; i++) {
		int start = node->plane.pointInFront(cameraPosition) ? 0 : 1;
		int j = (i + start) % 2;

		switch(node->children[j]->type) {
			case World::BSP::TreeItem::TypeNode:
				drawNode((World::BSP::Node*)node->children[j], cameraLeaf, cameraPosition, frustum);
				break;

			case World::BSP::TreeItem::TypeLeaf:
				drawLeaf((World::BSP::Leaf*)node->children[j], cameraLeaf, cameraPosition, frustum);
				break;
		}
	}
}

}