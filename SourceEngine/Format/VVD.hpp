#ifndef FORMAT_VVD_HPP
#define FORMAT_VVD_HPP

#include "File/Space.hpp"

#include <string>

namespace Format {
namespace VVD {

#pragma pack(push, 1)
struct Vector {
	float x;
	float y;
	float z;
};

struct Vector2D {
	float u;
	float v;
};

const int MaxBonesPerVert = 3;
struct BoneWeight
{
	float	weight[MaxBonesPerVert];
	char	bone[MaxBonesPerVert];
	unsigned char numBones;
};

struct Vertex
{
	BoneWeight boneWeights;
	Vector position;
	Vector normal;
	Vector2D texCoord;
};

struct Fixup {
	int lod;
	int sourceVertexId;
	int numVertices;
};

const int MaxNumLods = 8;
struct Header {
	int	id;
	int	version;
	long checksum;
	int	numLods;
	int	numLodVertices[MaxNumLods];

	int	numFixups;
	int	fixupTableStart;
	Fixup *fixup(int n) { return (Fixup*)((char*)this + fixupTableStart + n * sizeof(Fixup)); }

	int	vertexDataStart;
	Vertex *vertex(int n) { return (Vertex*)((char*)this + vertexDataStart + n * sizeof(Vertex)); }

	int	tangentDataStart;
};
#pragma pack(pop)

static Header *open(File::Space *space, const std::string &name)
{
	return (Header*)space->read(name);
}

}
}
#endif