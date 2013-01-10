#include "Draw/MapDrawer.hpp"

#include "Geo/Transformation.hpp"

#include "World/Entity/Prop/Dynamic.hpp"

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
	mSkyboxDrawer = new SkyboxDrawer;
}

void MapDrawer::draw(const Geo::Point &position, const Geo::Orientation &orientation)
{
	bool drawEntities = true;

	Geo::Transformation transformation = Geo::Transformation::translateRotate(position, orientation);
	Geo::Frustum frustum = mStartFrustum * transformation;

	mBspDrawer->newFrame();
	mFaceDrawer->newFrame();

	if(mUpdateFrustum) {
		mFrustum = frustum;
		mBspDrawer->setCameraPosition(position);
		mBspDrawer->setFrustum(frustum);
		mModelDrawer->setCameraPosition(position);
	}

	if(!mMap->bsp()->leafForPoint(0, position)->visibleLeaves) {
		mFaceDrawer->setDrawLightmaps(false);
		mFaceDrawer->setDrawTextures(false);
		drawEntities = false;
	}

	mFrameTag++;
	mBspDrawer->setFrameTag(mFrameTag);

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotatef(-orientation.pitch(), 0, -1, 0);
	glRotatef(-orientation.yaw(), 0, 0, 1);
	mSkyboxDrawer->draw(mMap->skybox());
	glPopMatrix();

	glPushMatrix();

	glMultMatrixf(transformation.inverseMatrix().transpose().elements());
	glClear(GL_DEPTH_BUFFER_BIT);

	mBspDrawer->draw(0, Geo::Point(0, 0, 0), Geo::Orientation(0, 0, 0));

	if(drawEntities) {
		for(unsigned int i=0; i<mMap->numEntities(); i++) {
			World::Entity::Base *base = mMap->entity(i);

			if(!base) {
				continue;
			}

			if(base->classname() == "prop_dynamic") {
				World::Entity::Prop::Dynamic *entity = (World::Entity::Prop::Dynamic*)base;

				if(entity->model()) {
					if(entity->leaf()->frameTag == mFrameTag && !mFrustum.boxOutside(entity->box())) {
						mModelDrawer->draw(entity->model(), entity->position(), entity->orientation(), 0);
					}
				}
			}
		}

		for(unsigned int i=0; i<mMap->numStaticProps(); i++) {
			World::Entity::Prop::Static *staticProp = mMap->staticProp(i);

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