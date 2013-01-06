#include "Draw/MapDrawer.hpp"

#include "Geo/Transformation.hpp"

#include <GL/glew.h>

namespace Draw {

MapDrawer::MapDrawer(World::Map *map, const Geo::Frustum &startFrustum)
{
	mMap = map;
	mStartFrustum = startFrustum;
	mFrameTag = 0;

	mFaceDrawer = new FaceDrawer;
	mBspDrawer = new BSPDrawer(map->bsp(), mFaceDrawer);
	mModelDrawer = new ModelDrawer;
}

void MapDrawer::draw(const Geo::Point &position, const Geo::Orientation &orientation)
{
	bool drawEntities = true;

	Geo::Frustum frustum = mStartFrustum * Geo::Transformation::translateRotate(position, orientation);

	mBspDrawer->newFrame();
	mFaceDrawer->newFrame();

	if(mUpdateFrustum) {
		mFrustum = frustum;
		mBspDrawer->setCameraPosition(position);
		mBspDrawer->setFrustum(frustum);
	}

	if(!mMap->bsp()->leafForPoint(0, position)->visibleLeaves) {
		mFaceDrawer->setDrawLightmaps(false);
		mFaceDrawer->setDrawTextures(false);
		drawEntities = false;
	}

	mFrameTag++;
	mBspDrawer->setFrameTag(mFrameTag);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotatef(-orientation.pitch(), 0, -1, 0);
	glRotatef(-orientation.yaw(), 0, 0, 1);
	glTranslatef(-position.x(), -position.y(), -position.z());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mBspDrawer->draw(0, Geo::Point(0, 0, 0), Geo::Orientation(0, 0, 0));

	if(drawEntities) {
		for(unsigned int i=0; i<mMap->numEntities(); i++) {
			World::Entity *entity = mMap->entity(i);
			if(entity->model()) {
				if(entity->leaf()->frameTag == mFrameTag && !mFrustum.boxOutside(entity->box())) {
					mModelDrawer->draw(entity->model(), entity->position(), entity->orientation(), 0);
				}
			}

			if(entity->bspRoot() != 0) {
				mBspDrawer->draw(entity->bspRoot(), entity->position(), entity->orientation());
			}
		}

		for(unsigned int i=0; i<mMap->numStaticProps(); i++) {
			World::StaticProp *staticProp = mMap->staticProp(i);

			for(unsigned int j=0; j<staticProp->numLeaves(); j++) {
				if(staticProp->leaf(j)->frameTag == mFrameTag && !mFrustum.boxOutside(staticProp->box())) {
					mModelDrawer->draw(staticProp->model(), staticProp->position(), staticProp->orientation(), staticProp->vhv());
				}
				break;
			}
		}
	}

	glPopMatrix();
}

}