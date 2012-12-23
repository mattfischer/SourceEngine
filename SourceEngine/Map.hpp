#ifndef MAP_HPP
#define MAP_HPP

#include "File/IReaderFactory.hpp"
#include "File/BSP.hpp"
#include "File/VTF.hpp"
#include "File/VVD.hpp"
#include "File/MDL.hpp"
#include "File/VTX.hpp"
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
		int lightMapWidth;
		int lightMapHeight;
		int lightMapMinU;
		int lightMapMinV;
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

	struct Model {
		File::MDL *mdl;
		File::VVD *vvd;
		File::VTX *vtx;
	};

	struct Entity {
		Model *model;
		Geo::Point position;
	};

	const Leaf &leaf(int leaf) { return mLeaves[leaf]; }
	size_t numLeaves() { return mNumLeaves; }

	Node *rootNode();
	File::BSP *bsp() { return mBSP; }

	const Entity &entity(int entity) { return mEntities[entity]; }
	size_t numEntities() { return mNumEntities; }

private:
	File::BSP *mBSP;

	Face *mFaces;
	size_t mNumFaces;

	Texture *mTextures;
	size_t mNumTextures;

	Node *mNodes;
	size_t mNumNodes;

	Leaf *mLeaves;
	size_t mNumLeaves;

	Entity *mEntities;
	size_t mNumEntities;
};

#endif