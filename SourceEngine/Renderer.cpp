#include "Renderer.hpp"

#include "File/VMT.hpp"
#include "File/VTF.hpp"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <math.h>

Renderer::Renderer(Map *map, int width, int height)
{
	mMap = map;

	glMatrixMode(GL_PROJECTION_MATRIX);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glLoadIdentity();
	float scale = 10;
	gluPerspective(60, (float)width / (float)height, 50, 10000);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	mPosition = Geo::Vector(-8674, 1773, 37);
	mYaw = -90;
	mPitch = 0;

	mStartFrustum = Geo::Frustum(60, (float)width / (float)height);

	mFrustumCull = false;
	mUpdateFrustum = true;
}

void Renderer::rotate(int yaw, int pitch)
{
	mYaw += yaw;
	if(mYaw > 360) {
		mYaw -= 360;
	}

	if(mYaw < 0) {
		mYaw += 360;
	}

	mPitch += pitch;
	if(mPitch > 70) {
		mPitch = 70;
	}

	if(mPitch < -70) {
		mPitch = -70;
	}
}

void Renderer::move(int amount)
{
	float speed = 30;
	float angleRad = mYaw * 3.14f / 180;

	Geo::Vector delta(cosf(angleRad), sinf(angleRad), 0);
	mPosition = mPosition + speed * amount * delta;
};

void Renderer::rise(int amount)
{
	float speed = 30;

	mPosition = mPosition + speed * amount * Geo::Vector(0, 0, 1);
};

const Map::Leaf *findCameraLeaf(Map *map, const Geo::Vector &position)
{
	Map::BSPBase *cursor = map->rootNode();
	while(cursor->type == Map::BSPBase::TypeNode) {
		int child;
		Map::Node *node = (Map::Node*)cursor;

		if(node->plane.pointInFront(position)) {
			child = 0;
		} else {
			child = 1;
		}

		cursor = node->children[child];
	}

	return (Map::Leaf*)cursor;
}

static void renderFace(const Map::Face &face)
{
	glBindTexture(GL_TEXTURE_2D, face.texture->tex);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	for(int j=0; j<face.numVertices; j++) {
		Geo::Vector &vertex = face.vertices[j];
		float s = face.textureVertices[0][0] * vertex.x() + face.textureVertices[0][1] * vertex.y() + face.textureVertices[0][2] * vertex.z() + face.textureVertices[0][3];
		float t = face.textureVertices[1][0] * vertex.x() + face.textureVertices[1][1] * vertex.y() + face.textureVertices[1][2] * vertex.z() + face.textureVertices[1][3];

		if(face.texture->vtf) {
			glTexCoord2f(s / face.texture->vtf->width(), t / face.texture->vtf->height());
		}
		glVertex3f(-vertex.y(), vertex.z(), -vertex.x());
	}
	glEnd();
}

static void renderLeaf(const Map::Leaf *leaf, const Map::Leaf *cameraLeaf, const Geo::Frustum &frustum, bool cull)
{
	if(leaf->number == -1) {
		return;
	}

	if(cameraLeaf->visibleLeaves != 0 && !cameraLeaf->visibleLeaves[leaf->number]) {
		return;
	}

	if(cull && frustum.boxOutside(leaf->bbox)) {
		return;
	}

	for(int j=0; j<leaf->numFaces; j++) {
		renderFace(*leaf->faces[j]);
	}
}

static void renderNode(const Map::Node *node, const Map::Leaf *cameraLeaf, const Geo::Frustum &frustum, bool cull)
{
	if(cull && frustum.boxOutside(node->bbox)) {
		return;
	}

	for(int i=0; i<2; i++) {
		switch(node->children[i]->type) {
			case Map::BSPBase::TypeNode:
				renderNode((Map::Node*)node->children[i], cameraLeaf, frustum, cull);
				break;

			case Map::BSPBase::TypeLeaf:
				renderLeaf((Map::Leaf*)node->children[i], cameraLeaf, frustum, cull);
				break;
		}
	}
}

void Renderer::render()
{
	if(mUpdateFrustum) {
		mFrustum = mStartFrustum.rotateY(mPitch);
		mFrustum = mFrustum.rotateZ(mYaw);
		mFrustum = mFrustum.translate(mPosition);
	}

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotatef(mPitch, 1, 0, 0);
	glRotatef(-mYaw, 0, 1, 0);
	glTranslatef(mPosition.y(), -mPosition.z(), mPosition.x());

	const Map::Leaf *cameraLeaf = findCameraLeaf(mMap, mPosition);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderNode(mMap->rootNode(), cameraLeaf, mFrustum, mFrustumCull);
	glPopMatrix();
}
