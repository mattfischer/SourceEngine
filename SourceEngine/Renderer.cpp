#include "Renderer.hpp"

#include "File/VMT.hpp"
#include "File/VTF.hpp"

#include <windows.h>
#include <GL/gl.h>

#include <math.h>

Renderer::Renderer(Map *map, int width, int height)
{
	mMap = map;

	glMatrixMode(GL_PROJECTION_MATRIX);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glLoadIdentity();
	float scale = 10;
	glFrustum(-(float)width / (float)height * scale, (float)width / (float)height * scale, -scale, scale, 50, 10000);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	mX = -8674;
	mY = 1773;
	mZ = 37;
	mYaw = -90;
	mPitch = 0;
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
	mX += 30 * amount * sinf(mYaw * 3.14f / 180);
	mY += 30 * amount * cosf(mYaw * 3.14f / 180);
};

void Renderer::rise(int amount)
{
	mZ += amount * 30;
};

const Map::Leaf *findCameraLeaf(Map *map, float x, float y, float z)
{
	Map::BSPBase *cursor = map->rootNode();
	while(cursor->type == Map::BSPBase::TypeNode) {
		int child;
		Map::Node *node = (Map::Node*)cursor;

		Geo::Vector normal = node->plane.normal();
		if(x*normal.x() + y*normal.y() + z*normal.z() - node->plane.distance() > 0) {
			child = 0;
		} else {
			child = 1;
		}

		cursor = node->children[child];
	}

	return (Map::Leaf*)cursor;
}

void renderFace(const Map::Face &face)
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
		glVertex3f(vertex.x(), vertex.z(), -vertex.y());
	}
	glEnd();
}

void Renderer::render()
{
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotatef(mPitch, 1, 0, 0);
	glRotatef(mYaw, 0, 1, 0);
	glTranslatef(-mX, -mZ, mY);

	const Map::Leaf *cameraLeaf = findCameraLeaf(mMap, mX, mY, mZ);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int i=0; i<mMap->numLeaves(); i++) {
		const Map::Leaf &leaf = mMap->leaf(i);
		if(leaf.number == -1) {
			continue;
		}

		if(cameraLeaf->visibleLeaves == 0 || cameraLeaf->visibleLeaves[i]) {
			for(int j=0; j<leaf.numFaces; j++) {
				renderFace(*leaf.faces[j]);
			}
		}
	}
	glPopMatrix();
}
