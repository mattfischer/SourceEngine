#include "File/VVD.hpp"

namespace File {

#define MAX_NUM_LODS 8

struct Header {
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

struct Vector {
	float x;
	float y;
	float z;
};

struct Vector2D {
	float u;
	float v;
};

#define MAX_NUM_BONES_PER_VERT 3

struct BoneWeight
{
	float	weight[MAX_NUM_BONES_PER_VERT];
	char	bone[MAX_NUM_BONES_PER_VERT];
	unsigned char numBones;
};

struct Vertex
{
	BoneWeight boneWeights;
	Vector position;
	Vector normal;
	Vector2D texCoord;
};

VVD::VVD(IReader *reader)
{
	Header header;

	reader->read(&header, sizeof(header));

	if(header.numFixups == 0) {
		Vertex *vertices = new Vertex[header.numLODVertexes[0]];
		reader->seek(header.vertexDataStart);
		reader->read(vertices, header.numLODVertexes[0] * sizeof(Vertex));
	} else {
		// TODO: Handle fixups
	}
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