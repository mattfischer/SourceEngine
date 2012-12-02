#include "Map.hpp"

Map::Map(File::IReaderFactory *factory, const std::string &name)
{
	mBSP = new File::BSP(factory, name);

	File::BSP::Model &model = mBSP->models()[0];

	mNumFaces = model.numFaces;;
	mFaces = new Face[mNumFaces];
	for(int i=0; i<mNumFaces; i++) {
		File::BSP::Face &bspFace = mBSP->faces()[model.firstFace + i];
		Face &face = mFaces[i];

		face.numVertices = bspFace.numEdges;
		face.vertices = new Geo::Vector[face.numVertices];

		for(int j=0; j<face.numVertices; j++) {
			int surfEdge = mBSP->surfEdges()[bspFace.firstEdge + j];
			int vertex;
			if(surfEdge > 0) {
				vertex = mBSP->edges()[surfEdge].v[0];
			} else {
				vertex = mBSP->edges()[-surfEdge].v[1];
			}
			File::BSP::Vector &bspVector = mBSP->vertices()[vertex];

			face.vertices[j] = Geo::Vector(bspVector.x, bspVector.y, bspVector.z);
		}
	}
}