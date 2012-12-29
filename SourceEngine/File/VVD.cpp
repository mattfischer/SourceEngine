#include "File/VVD.hpp"

namespace File {

#define MAX_NUM_LODS 8

struct VVDHeader {
	int	id;
	int	version;
	long	checksum;
	int	numLODs;
	int	numLODVertexes[MAX_NUM_LODS];
	int	numFixups;
	int	fixupTableStart;
	int	vertexDataStart;
	int	tangentDataStart;
};

struct Fixup {
	int lod;
	int sourceVertexID;
	int numVertices;
};

VVD::VVD(IReader *reader)
{
	char *data = new char[reader->size()];
	reader->read(data, reader->size());

	VVDHeader *header = (VVDHeader*)data;
	Vertex *vertices = (Vertex*)(data + header->vertexDataStart);
	Fixup *fixups = (Fixup*)(data + header->fixupTableStart);

	mNumLods = header->numLODs;
	mLods = new Lod[mNumLods];
	for(int lod=0; lod<mNumLods; lod++) {
		mLods[lod].numVertices = header->numLODVertexes[lod];
		mLods[lod].vertices = new Vertex[mLods[lod].numVertices];
		if(header->numFixups == 0) {
			for(int v=0; v<mLods[lod].numVertices; v++) {
				mLods[lod].vertices[v] = vertices[v];
			}
		} else {
			int v = 0;
			for(int i=0; i<header->numFixups; i++) {
				if(lod <= fixups[i].lod) {
					for(int j=0; j<fixups[i].numVertices; j++) {
						mLods[lod].vertices[v++] = vertices[fixups[i].sourceVertexID + j];
					}
				}
			}
		}
	}

	delete[] data;
}

VVD *VVD::open(IReaderFactory *factory, const std::string &name)
{
	VVD *ret = 0;
	IReader *reader = factory->open(name);

	if(reader) {
		ret = new VVD(reader);
		delete reader;
	}

	return ret;
}

}