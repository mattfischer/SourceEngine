#ifndef FORMAT_VTX_HPP
#define FORMAT_VTX_HPP

#include "File/Space.hpp"

#include <string>

namespace Format {
namespace VTX {

#pragma pack(push, 1)

struct Vertex {
	unsigned char boneWeightIndex[3];
	unsigned char numBones;

	unsigned short vertex;
	char boneID[3];
};

struct Strip {
	int numIndices;
	int indexOffset;

	int numVertices;
	int vertexOffset;

	short numBones;
	unsigned char flags;

	int numBoneStateChanges;
	int boneStateChangeOffset;
};

struct StripGroup {
	int numVertices;
	int vertexOffset;

	int numIndices;
	int indexOffset;

	int numStrips;
	int stripOffset;

	Strip *strip(int n) { return (Strip*)((char*)this + stripOffset + n * sizeof(Strip)); }
	Vertex *vertex(int n) { return (Vertex*)((char*)this + vertexOffset + n * sizeof(Vertex)); }
	unsigned short *index(int n) { return (unsigned short*)((char*)this + indexOffset + n * sizeof(unsigned short)); }
};

struct Mesh {
	int numStripGroups;
	int stripGroupOffset;
	unsigned char flags;

	StripGroup *stripGroup(int n) { return (StripGroup*)((char*)this + stripGroupOffset + n * sizeof(StripGroup)); }
};

struct Lod {
	int numMeshes;
	int meshOffset;
	float switchPoint;

	Mesh *mesh(int n) { return (Mesh*)((char*)this + meshOffset + n * sizeof(Mesh)); }
};

struct Model {
	int numLods;
	int lodOffset;

	Lod *lod(int n) { return (Lod*)((char*)this + lodOffset + n * sizeof(Lod)); }
};

struct BodyPart {
	int numModels;
	int modelOffset;

	Model *model(int n) { return (Model*)((char*)this + modelOffset + n * sizeof(Model)); }
};

struct Header
{
	int version;

	int vertCacheSize;
	unsigned short maxBonesPerStrip;
	unsigned short maxBonesPerFace;
	int maxBonesPerVert;

	long checkSum;

	int numLODs;

	int materialReplacementListOffset;

	int numBodyParts;
	int bodyPartOffset;
	BodyPart *bodyPart(int n) { return (BodyPart*)((char*)this + bodyPartOffset + n * sizeof(BodyPart)); }
};

#pragma pack(pop)

static Header *open(File::Space *space, const std::string &name)
{
	return (Header*)space->read(name);
}

}
}
#endif