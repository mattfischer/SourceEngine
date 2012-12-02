#ifndef MAP_HPP
#define MAP_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"
#include "File/VTF.hpp"

#include "Geo/Vector.hpp"

#include <string>

class Map {
public:
	Map(File::IReaderFactory *factory, const std::string &name);

	struct Face {
		int numVertices;
		Geo::Vector *vertices;
	};

	struct Texture {
		File::VTF *texture;
	};

	const Face &face(int face) { return mFaces[face]; }
	int numFaces() { return mNumFaces; }

private:
	File::BSP *mBSP;

	Face *mFaces;
	int mNumFaces;

	Texture *mTextures;
	int mNumTextures;
};

#endif