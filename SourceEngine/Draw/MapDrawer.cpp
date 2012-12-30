#include "Draw/MapDrawer.hpp"

#include "Geo/Transformation.hpp"

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

	Geo::Transformation transformation = Geo::Transformation::translate(position);
	transformation = transformation * Geo::Transformation::rotateZ(orientation.yaw());
	transformation = transformation * Geo::Transformation::rotateY(-orientation.pitch());

	Geo::Frustum frustum = mStartFrustum * transformation;

	mBspDrawer->newFrame();
	mFaceDrawer->newFrame();

	if(mUpdateFrustum) {
		mFrustum = frustum;
		mFaceDrawer->setPosition(position);
		mBspDrawer->setPosition(position);
		mBspDrawer->setFrustum(frustum);
	}

	if(!mBspDrawer->cameraLeaf()->visibleLeaves) {
		mFaceDrawer->setDrawLightmaps(false);
		mFaceDrawer->setDrawTextures(false);
		drawEntities = false;
	}

	mFrameTag++;
	mBspDrawer->setFrameTag(mFrameTag);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotatef(-orientation.pitch(), 1, 0, 0);
	glRotatef(-orientation.yaw(), 0, 1, 0);
	glTranslatef(position.y(), -position.z(), position.x());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mBspDrawer->draw(0, Geo::Point(0, 0, 0), Geo::Orientation(0, 0, 0));

	if(drawEntities) {
		for(unsigned int i=0; i<mMap->numEntities(); i++) {
			World::Entity *entity = mMap->entity(i);
			if(entity->model()) {
				if(entity->leaf()->frameTag == mFrameTag && !mFrustum.boxOutside(entity->box())) {
					mModelDrawer->draw(entity->model(), entity->position(), entity->orientation());
				}
			}

			if(entity->bspRoot() != 0) {
				Geo::Transformation bspTransform = Geo::Transformation::translate(-1 * entity->position());
				Geo::Frustum bspFrustum = frustum * bspTransform;
				Geo::Point bspPosition = bspTransform * position;

				mBspDrawer->setFrustum(bspFrustum);
				mBspDrawer->setPosition(bspPosition);
				mBspDrawer->draw(entity->bspRoot(), entity->position(), entity->orientation());
			}
		}
	}

	glPopMatrix();
}

}