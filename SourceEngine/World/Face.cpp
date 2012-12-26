#include "World/Face.hpp"

#include <math.h>

namespace World {

Face::Face(File::BSP *file, int number, Material *materials[])
{
	const File::BSP::Face &fileFace = file->face(number);
	const File::BSP::TexInfo &texInfo = file->texInfo(fileFace.texInfo);

	int textureWidth = 1;
	int textureHeight = 1;

	if(fileFace.lightOfs == -1) {
		mLightMap = 0;
	} else {
		glGenTextures(1, &mLightMap);
		glBindTexture(GL_TEXTURE_2D, mLightMap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		int lightmapWidth = fileFace.lightmapTextureSizeInLuxels[0] + 1;
		int lightmapHeight = fileFace.lightmapTextureSizeInLuxels[1] + 1;

		textureWidth = 1;
		while(textureWidth < lightmapWidth) {
			textureWidth *= 2;
		}

		textureHeight = 1;
		while(textureHeight < lightmapHeight) {
			textureHeight *= 2;
		}

		const unsigned char *lightMap = file->lighting(fileFace.lightOfs);
		unsigned char *data = new unsigned char[textureWidth * textureHeight * 4];
		for(int y=0; y<textureHeight; y++) {
			for(int x=0; x<textureWidth; x++) {
				signed char exp = lightMap[(y * lightmapWidth + x) * 4 + 3];
				float scale = pow(2.0f, exp) * 20;
				for(int c=0; c<3; c++) {
					float color = lightMap[(y * lightmapWidth + x) * 4 + c] / 255.0f;
					color *= scale;
					if(color < 0) color = 0;
					if(color > 1) color = 1;
					data[(y * textureWidth + x) * 4 + c] = (unsigned char)(color * 255.0f);
				}
				data[(y * textureWidth + x) * 4 + 3] = 0xff;
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		delete[] data;
	}

	const File::BSP::Plane filePlane = file->plane(fileFace.planeNum);
	mPlane = Geo::Plane(Geo::Vector(filePlane.normal.x, filePlane.normal.y, filePlane.normal.z), filePlane.dist);
	mMaterial = materials[texInfo.texdata];
	mGray = (rand() % 255) / 255.0f;
	mNumVertices = fileFace.numEdges;
	mVertices = new Geo::Point[mNumVertices];
	mTextureCoordinates = new Geo::Coordinate[mNumVertices];
	mLightmapCoordinates = new Geo::Coordinate[mNumVertices];

	for(int i=0; i<mNumVertices; i++) {
		int surfEdge = file->surfEdge(fileFace.firstEdge + i);
		int vertIndex;
		float u;
		float v;

		if(surfEdge > 0) {
			vertIndex = file->edge(surfEdge).v[0];
		} else {
			vertIndex = file->edge(-surfEdge).v[1];
		}
		const File::BSP::Vector &fileVector = file->vertex(vertIndex);

		Geo::Point vertex(fileVector.x, fileVector.y, fileVector.z);
		mVertices[i] = vertex;

		u = texInfo.textureVecs[0][0] * vertex.x() + texInfo.textureVecs[0][1] * vertex.y() + texInfo.textureVecs[0][2] * vertex.z() + texInfo.textureVecs[0][3];
		v = texInfo.textureVecs[1][0] * vertex.x() + texInfo.textureVecs[1][1] * vertex.y() + texInfo.textureVecs[1][2] * vertex.z() + texInfo.textureVecs[1][3];

		if(mMaterial && mMaterial->texture()) {
			mTextureCoordinates[i] = Geo::Coordinate(u / mMaterial->texture()->width(), v / mMaterial->texture()->height());
		}

		if(fileFace.lightOfs != -1) {
			u = texInfo.lightmapVecs[0][0] * vertex.x() + texInfo.lightmapVecs[0][1] * vertex.y() + texInfo.lightmapVecs[0][2] * vertex.z() + texInfo.lightmapVecs[0][3];
			v = texInfo.lightmapVecs[1][0] * vertex.x() + texInfo.lightmapVecs[1][1] * vertex.y() + texInfo.lightmapVecs[1][2] * vertex.z() + texInfo.lightmapVecs[1][3];

			u -= fileFace.lightmapTextureMinsInLuxels[0];
			v -= fileFace.lightmapTextureMinsInLuxels[1];
			mLightmapCoordinates[i] = Geo::Coordinate(u / textureWidth, v / textureHeight);
		}
	}
}

}