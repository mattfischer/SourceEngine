#ifndef WORLD_FACE_HPP
#define WORLD_FACE_HPP

#include "File/BSP.hpp"

#include "Geo/Point.hpp"
#include "Geo/Coordinate.hpp"
#include "Geo/Plane.hpp"

#include "World/Material.hpp"

namespace World {

class Face {
public:
	Face(File::BSP *file, int number, Material *materials[]);

	const Geo::Plane &plane() { return mPlane; }

	Material *material() { return mMaterial; }

	int numVertices() { return mNumVertices; }
	const Geo::Point &vertex(int vertex) { return mVertices[vertex]; }
	const Geo::Coordinate &textureCoordinate(int vertex) { return mTextureCoordinates[vertex]; }
	const Geo::Coordinate &lightmapCoordinate(int vertex) { return mLightmapCoordinates[vertex]; }

	float gray() { return mGray; }

	GLuint lightmapTex() { return mLightMap; }

private:
	int mNumVertices;
	Geo::Point *mVertices;
	Geo::Coordinate *mTextureCoordinates;
	Geo::Coordinate *mLightmapCoordinates;

	float mGray;
	Geo::Plane mPlane;

	Material *mMaterial;
	GLuint mLightMap;
};

}

#endif