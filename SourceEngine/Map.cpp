#include "Map.hpp"

Map::Map(File::IReaderFactory *factory, const std::string &name)
{
	mBSP = new File::BSP(factory, name);

	const File::BSP::Model &model = mBSP->model(0);

	mNumFaces = model.numFaces;;
	mFaces = new Face[mNumFaces];
	for(int i=0; i<mNumFaces; i++) {
		const File::BSP::Face &bspFace = mBSP->face(model.firstFace + i);
		Face &face = mFaces[i];

		face.numVertices = bspFace.numEdges;
		face.vertices = new Geo::Vector[face.numVertices];

		for(int j=0; j<face.numVertices; j++) {
			int surfEdge = mBSP->surfEdge(bspFace.firstEdge + j);
			int vertex;
			if(surfEdge > 0) {
				vertex = mBSP->edge(surfEdge).v[0];
			} else {
				vertex = mBSP->edge(-surfEdge).v[1];
			}
			const File::BSP::Vector &bspVector = mBSP->vertex(vertex);

			face.vertices[j] = Geo::Vector(bspVector.x, bspVector.y, bspVector.z);
		}
	}
}