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
	glLoadIdentity();
	float scale = 10;
	glFrustum(-(float)width / (float)height * scale, (float)width / (float)height * scale, -scale, scale, 100, 10000);

	glEnable(GL_DEPTH_TEST);
	mX = mY = mZ = 0;
	mRotation = 0;
}

void Renderer::rotate(int amount)
{
	mRotation += 3 * amount;
}

void Renderer::move(int amount)
{
	mX += 100 * amount * sinf(-mRotation * 3.14f / 180);
	mZ += 100 * amount * cosf(-mRotation * 3.14f / 180);
};

void Renderer::rise(int amount)
{
	mY += amount * 100;
};

void Renderer::render()
{
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotatef(mRotation, 0, 1, 0);
	glTranslatef(mX, mY, mZ);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int i=0; i<mMap->numFaces(); i++) {
		const Map::Face &face = mMap->face(i);

		float gray = (float)(i * 43 % mMap->numFaces()) / mMap->numFaces();
		glColor3f(gray, gray, gray);
		glBegin(GL_POLYGON);
		for(int j=0; j<face.numVertices; j++) {
			Geo::Vector &vertex = face.vertices[j];
			glVertex3f(vertex.x(), vertex.y(), vertex.z());
		}
		glEnd();
	}
	glPopMatrix();
}
