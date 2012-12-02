#include "Renderer.hpp"

#include "File/VMT.hpp"
#include "File/VTF.hpp"

#include <windows.h>
#include <GL/gl.h>

#include <math.h>

Renderer::Renderer(File::BSP *bspFile, File::IReaderFactory *factory, int width, int height)
{
	mBspFile = bspFile;

	File::BSP::Model &model = mBspFile->models()[0];
	File::BSP::Face &face = mBspFile->faces()[model.firstFace];
	File::BSP::TexInfo &texInfo = mBspFile->texInfos()[face.texInfo];
	File::BSP::TexData &texData = mBspFile->texDatas()[texInfo.texdata];
	std::string textureName = mBspFile->texDataStringTable()[texData.nameStringTableID];
	File::VMT *vmt = new File::VMT(factory, textureName);
	File::VTF *vtf = new File::VTF(factory, vmt->parameters()[std::string("$basetexture")]);

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
	File::BSP::Model &model = mBspFile->models()[0];
	for(int i=0; i<model.numFaces; i++) {
		File::BSP::Face &face = mBspFile->faces()[model.firstFace + i];

		float gray = (float)(i * 43 % model.numFaces) / model.numFaces;
		glColor3f(gray, gray, gray);
		glBegin(GL_POLYGON);
		for(int j=0; j<face.numEdges; j++) {
			int surfEdge = mBspFile->surfEdges()[face.firstEdge + j];
			int vertex;
			if(surfEdge > 0) {
				vertex = mBspFile->edges()[surfEdge].v[0];
			} else {
				vertex = mBspFile->edges()[-surfEdge].v[1];
			}

			File::BSP::Vector &vector = mBspFile->vertices()[vertex];
			glVertex3f(vector.x, vector.y, vector.z);
		}
		glEnd();
	}
	glPopMatrix();
}
