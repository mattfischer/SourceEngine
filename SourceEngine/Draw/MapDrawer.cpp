#include "Draw/MapDrawer.hpp"

#include "Geo/Transformation.hpp"

namespace Draw {

MapDrawer::MapDrawer(World::Map *map, const Geo::Frustum &startFrustum)
{
	mMap = map;
	mStartFrustum = startFrustum;

	mFaceDrawer = new FaceDrawer;
	mBspDrawer = new BSPDrawer(map->bsp(), mFaceDrawer);
	mModelDrawer = new ModelDrawer;
}

void MapDrawer::draw(const Geo::Point &position, const Geo::Orientation &orientation)
{
	Geo::Transformation transformation = Geo::Transformation::translate(position);
	transformation = transformation * Geo::Transformation::rotateZ(orientation.yaw());
	transformation = transformation * Geo::Transformation::rotateY(-orientation.pitch());

	Geo::Frustum frustum = mStartFrustum * transformation;

	if(mUpdateFrustum) {
		mFrustum = frustum;
		mFaceDrawer->setPosition(position);
		mBspDrawer->setPosition(position);
		mBspDrawer->setFrustum(frustum);
	}

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotatef(-orientation.pitch(), 1, 0, 0);
	glRotatef(-orientation.yaw(), 0, 1, 0);
	glTranslatef(position.y(), -position.z(), position.x());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mBspDrawer->draw();

	for(unsigned int i=0; i<mMap->numEntities(); i++) {
		World::Entity *entity = mMap->entity(i);
		if(entity->model()) {
			if(!mFrustum.boxOutside(entity->box())) {
				mModelDrawer->draw(entity->model(), entity->position(), entity->orientation());
			}
		}
	}
	glPopMatrix();
}

}