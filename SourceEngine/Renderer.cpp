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
	glFrustum(-(float)width / (float)height * scale, (float)width / (float)height * scale, -scale, scale, 100, 10000);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	mX = -8674;
	mY = 1773;
	mZ = 37;
	mRotation = -90;
}

void Renderer::rotate(int amount)
{
	mRotation += 3 * amount;
}

void Renderer::move(int amount)
{
	mX += 100 * amount * sinf(mRotation * 3.14f / 180);
	mY += 100 * amount * cosf(mRotation * 3.14f / 180);
};

void Renderer::rise(int amount)
{
	mZ += amount * 100;
};

const File::BSP::Leaf &findCameraLeaf(File::BSP *bsp, float x, float y, float z)
{
	int nodeNum = bsp->model(0).headNode;
	while(nodeNum >= 0) {
		int child;
		const File::BSP::Node &node = bsp->node(nodeNum);
		const File::BSP::Plane &plane = bsp->plane(node.planeNum);

		if(x*plane.normal.x + y*plane.normal.y + z*plane.normal.z - plane.dist > 0) {
			child = 0;
		} else {
			child = 1;
		}

		nodeNum = node.children[child];
	}

	return bsp->leaf(-nodeNum - 1);
}

void renderFace(const Map::Face &face, File::BSP *bsp)
{
	const File::BSP::TexInfo &texInfo = bsp->texInfo(face.texInfo);

	glBindTexture(GL_TEXTURE_2D, face.texture->tex);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	for(int j=0; j<face.numVertices; j++) {
		Geo::Vector &vertex = face.vertices[j];
		float s = texInfo.textureVecs[0][0] * vertex.x() + texInfo.textureVecs[0][1] * vertex.y() + texInfo.textureVecs[0][2] * vertex.z() + texInfo.textureVecs[0][3];
		float t = texInfo.textureVecs[1][0] * vertex.x() + texInfo.textureVecs[1][1] * vertex.y() + texInfo.textureVecs[1][2] * vertex.z() + texInfo.textureVecs[1][3];

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
	glRotatef(mRotation, 0, 1, 0);
	glTranslatef(-mX, -mZ, mY);

	const File::BSP::Leaf &cameraLeaf = findCameraLeaf(mMap->bsp(), mX, mY, mZ);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(true) { //cameraLeaf.cluster == -1) {
		for(int i=0; i<mMap->numFaces(); i++) {
			const Map::Face &face = mMap->face(i);
			renderFace(face, mMap->bsp());
		}
	} else {
		for(int i=0; i<mMap->bsp()->numLeaves(); i++) {
			const File::BSP::Leaf &leaf = mMap->bsp()->leaf(i);
			if(leaf.cluster == -1) {
				continue;
			}

			if(mMap->bsp()->clusterVisibleFrom(cameraLeaf.cluster, leaf.cluster)) {
				for(int j=0; j<leaf.numLeafFaces; j++) {
					int leafFace = leaf.firstLeafFace + j;
					if(leafFace < mMap->bsp()->numLeafFaces()) {
						int faceNum = mMap->bsp()->leafFace(leafFace);
						if(faceNum < mMap->numFaces()) {
							const Map::Face &face = mMap->face(faceNum);
							renderFace(face, mMap->bsp());
						}
					}
				}
			}
		}
	}
	glPopMatrix();
}
