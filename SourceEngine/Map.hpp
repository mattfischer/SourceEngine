#ifndef MAP_HPP
#define MAP_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"
#include "File/VTF.hpp"
#include "Geo/Point.hpp"
#include "Geo/Plane.hpp"
#include "Geo/Box.hpp"

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
		float gray;
		Geo::Plane plane;
		Geo::Point *vertices;
		float textureVertices[2][4];
		Texture *texture;
		GLuint lightMap;
		float lightMapVertices[2][4];
		float lightMapWidth;
		float lightMapHeight;
		float lightMapMinU;
		float lightMapMinV;
	};

	struct BSPBase {
		enum Type {
			TypeNode,
			TypeLeaf
		};

		BSPBase(Type _type) : type(_type) {}

		Type type;
		Geo::Box bbox;
	};

	struct Node : public BSPBase {
		Node() : BSPBase(TypeNode) {}

		Geo::Plane plane;
		BSPBase *children[2];
	};

	struct Leaf : public BSPBase {
		Leaf() : BSPBase(TypeLeaf) {}

		int numFaces;
		Face **faces;
		int number;
		bool *visibleLeaves;
	};

	const Leaf &leaf(int leaf) { return mLeaves[leaf]; }
	int numLeaves() { return mNumLeaves; }

	Node *rootNode();
	File::BSP *bsp() { return mBSP; }

private:
	File::BSP *mBSP;

	Face *mFaces;
	int mNumFaces;

	Texture *mTextures;
	int mNumTextures;

	Node *mNodes;
	int mNumNodes;

	Leaf *mLeaves;
	int mNumLeaves;
};

#endif