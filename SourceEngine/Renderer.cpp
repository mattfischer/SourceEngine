#include "Renderer.hpp"

#include "BSPFile.hpp"

#include <windows.h>
#include <GL/gl.h>

#include <math.h>

Renderer::Renderer(BSPFile *bspFile, int width, int height)
{
	mBspFile = bspFile;

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
	for(int i=0; i<mBspFile->model()->numPolys; i++) {
		BSPFile::Poly *poly = &mBspFile->model()->polys[i];
		glBegin(GL_POLYGON);
		glColor3f(poly->gray, poly->gray, poly->gray);
		for(int j=0; j<poly->numPoints; j++) {
			glVertex3f(poly->points[j].x, poly->points[j].y, poly->points[j].z);
		}
		glEnd();
	}
	glPopMatrix();
}
