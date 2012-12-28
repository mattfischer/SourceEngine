#ifndef WORLD_BSP_HPP
#define WORLD_BSP_HPP

#include "File/BSP.hpp"

#include "World/Face.hpp"

#include "Geo/BoxAligned.hpp"
#include "Geo/Plane.hpp"

namespace World {

class BSP {
public:
	struct TreeItem {
		enum Type {
			TypeNode,
			TypeLeaf
		};

		TreeItem(Type t) : type(t) {}

		Type type;
		Geo::BoxAligned bbox;
	};

	struct Node : public TreeItem {
		Node() : TreeItem(TypeNode) {}

		Geo::Plane plane;
		TreeItem *children[2];
	};

	struct Leaf : public TreeItem {
		Leaf() : TreeItem(TypeLeaf) {}

		int numFaces;
		Face **faces;
		int number;
		bool *visibleLeaves;
		int frameTag;
	};

	BSP(File::BSP *file, Face **faces);

	Node *rootNode() { return mRootNode; }

	bool leafVisibleFrom(const Leaf *leaf, const Leaf *cameraLeaf);
	Leaf *leafForPoint(const Geo::Point &point);

private:
	Node *mRootNode;

	size_t mNumNodes;
	Node *mNodes;

	size_t mNumLeaves;
	Leaf *mLeaves;
};

}

#endif