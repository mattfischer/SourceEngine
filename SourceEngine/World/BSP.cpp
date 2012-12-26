#include "World/BSP.hpp"

namespace World {

BSP::BSP(File::BSP *file, Face **faces)
{
	const File::BSP::Model &model = file->model(0);

	mNumLeaves = file->numLeaves();
	mLeaves = new Leaf[mNumLeaves];
	for(unsigned int i=0; i<mNumLeaves; i++) {
		const File::BSP::Leaf &fileLeaf = file->leaf(i);
		Leaf &leaf = mLeaves[i];

		leaf.number = fileLeaf.cluster;
		Geo::Vector minPoint = Geo::Vector(fileLeaf.mins[0], fileLeaf.mins[1], fileLeaf.mins[2]);
		Geo::Vector maxPoint = Geo::Vector(fileLeaf.maxs[0], fileLeaf.maxs[1], fileLeaf.maxs[2]);
		leaf.bbox = Geo::Box(minPoint, maxPoint);
		leaf.numFaces = fileLeaf.numLeafFaces;
		leaf.faces = new Face*[leaf.numFaces];
		for(int j=0; j<leaf.numFaces; j++) {
			int leafFace = fileLeaf.firstLeafFace + j;
			leaf.faces[j] = faces[file->leafFace(leafFace)];
		}

		if(fileLeaf.cluster == -1) {
			leaf.visibleLeaves = 0;
		} else {
			leaf.visibleLeaves = new bool[file->numClusters()];
			for(unsigned int j=0; j<file->numClusters(); j++) {
				leaf.visibleLeaves[j] = file->clusterVisibleFrom(fileLeaf.cluster, j);
			}
		}
	}

	mNumNodes = file->numNodes();
	mNodes = new Node[mNumNodes];
	for(unsigned int i=0; i<mNumNodes; i++) {
		const File::BSP::Node &fileNode = file->node(i);
		Node &node = mNodes[i];

		const File::BSP::Plane &filePlane = file->plane(fileNode.planeNum);
		node.plane = Geo::Plane(Geo::Vector(filePlane.normal.x, filePlane.normal.y, filePlane.normal.z), filePlane.dist);

		Geo::Vector minPoint = Geo::Vector(fileNode.mins[0], fileNode.mins[1], fileNode.mins[2]);
		Geo::Vector maxPoint = Geo::Vector(fileNode.maxs[0], fileNode.maxs[1], fileNode.maxs[2]);
		node.bbox = Geo::Box(minPoint, maxPoint);

		for(int j=0; j<2; j++) {
			int child = fileNode.children[j];
			if(child > 0) {
				node.children[j] = &mNodes[child];
			} else {
				node.children[j] = &mLeaves[-child - 1];
			}
		}
	}

	mRootNode = &mNodes[model.headNode];
}

bool BSP::leafVisibleFrom(const Leaf *leaf, const Leaf *cameraLeaf)
{
	if(!cameraLeaf->visibleLeaves) {
		return true;
	}

	return cameraLeaf->visibleLeaves[leaf->number];
}

}