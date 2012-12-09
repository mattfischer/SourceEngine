#ifndef MAP_HPP
#define MAP_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"
#include "File/VTF.hpp"
#include "Geo/Vector.hpp"

#include <windows.h>
#include <GL/gl.h>

#include <string>

class Map {
public:
	Map(File::IReaderFactory *factory, const std::string &name);

	struct Texture {
		File::VTF *vtf;
		GLuint tex;
	};

	struct Face {
		int numVertices;
		Geo::Vector *vertices;
		float textureVertices[2][4];
		Texture *texture;
	};

	const Face &face(int face) { return mFaces[face]; }
	int numFaces() { return mNumFaces; }

	File::BSP *bsp() { return mBSP; }

private:
	File::BSP *mBSP;

	Face *mFaces;
	int mNumFaces;

	Texture *mTextures;
	int mNumTextures;
};

#endif