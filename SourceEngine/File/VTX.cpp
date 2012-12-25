#include "File/VTX.hpp"

namespace File {

#pragma pack(push, 1)
struct VTXHeader
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
};

struct BodyPartHeader {
	int numModels;
	int modelOffset;
};

struct ModelHeader {
	int numLods;
	int lodOffset;
};

struct LodHeader {
	int numMeshes;
	int meshOffset;
	float switchPoint;
};

struct MeshHeader {
	int numStripGroups;
	int stripGroupHeaderOffset;
	unsigned char flags;
};

struct StripGroupHeader {
	int numVertices;
	int vertexOffset;

	int numIndices;
	int indexOffset;

	int numStrips;
	int stripOffset;
};

struct StripHeader {
	int numIndices;
	int indexOffset;

	int numVertices;
	int vertexOffset;

	short numBones;
	unsigned char flags;

	int numBoneStateChanges;
	int boneStateChangeOffset;
};

struct Vertex {
	unsigned char boneWeightIndex[3];
	unsigned char numBones;

	unsigned short vertex;
	char boneID[3];
};

#pragma pack(pop)

static void parseStrip(VTX::Strip *strip, StripHeader *header, Vertex *vertices, unsigned short *indices)
{
	strip->flags = header->flags;
	strip->numVertices = header->numIndices;
	strip->vertices = new int[strip->numVertices];
	for(int i=0; i<strip->numVertices; i++) {
		int index = header->indexOffset + i;
		if(index >= header->numIndices) {
			index = 0;
		}

		int vertex = header->vertexOffset + indices[index];
		if(vertex >= header->numVertices) {
			vertex = 0;
		}
		strip->vertices[i] = vertices[vertex].vertex;
	}
}

static void parseStripGroup(VTX::StripGroup *stripGroup, StripGroupHeader *header)
{
	Vertex *vertices = (Vertex*)((char*)header + header->vertexOffset);
	unsigned short *indices = (unsigned short*)((char*)header + header->indexOffset);

	stripGroup->numStrips = header->numStrips;
	stripGroup->strips = new VTX::Strip[stripGroup->numStrips];
	for(int i=0; i<stripGroup->numStrips; i++) {
		StripHeader *sh = (StripHeader*)((char*)header + header->stripOffset + i * sizeof(StripHeader));
		parseStrip(&stripGroup->strips[i], sh, vertices, indices);
	}
}

static void parseMesh(VTX::Mesh *mesh, MeshHeader *header)
{
	mesh->numStripGroups = header->numStripGroups;
	mesh->stripGroups = new VTX::StripGroup[mesh->numStripGroups];
	for(int i=0; i<mesh->numStripGroups; i++) {
		StripGroupHeader *sgh = (StripGroupHeader*)((char*)header + header->stripGroupHeaderOffset + i * sizeof(StripGroupHeader));
		parseStripGroup(&mesh->stripGroups[i], sgh);
	}
}

static void parseLod(VTX::Lod *lod, LodHeader *header)
{
	lod->numMeshes = header->numMeshes;
	lod->meshes = new VTX::Mesh[lod->numMeshes];
	for(int i=0; i<lod->numMeshes; i++) {
		MeshHeader *mh = (MeshHeader*)((char*)header + header->meshOffset + i * sizeof(MeshHeader));
		parseMesh(&lod->meshes[i], mh);
	}
}

static void parseModel(VTX::Model *model, ModelHeader *header)
{
	model->numLods = header->numLods;
	model->lods = new VTX::Lod[model->numLods];
	for(int i=0; i<model->numLods; i++) {
		LodHeader *lh = (LodHeader*)((char*)header + header->lodOffset + i * sizeof(LodHeader));
		parseLod(&model->lods[i], lh);
	}
}

static void parseBodyPart(VTX::BodyPart *bodyPart, BodyPartHeader *header)
{
	bodyPart->numModels = header->numModels;
	bodyPart->models = new VTX::Model[bodyPart->numModels];
	for(int i=0; i<bodyPart->numModels; i++) {
		ModelHeader *mh = (ModelHeader*)((char*)header + header->modelOffset + i * sizeof(ModelHeader));
		parseModel(&bodyPart->models[i], mh);
	}
}

VTX::VTX(IReader *reader)
{
	char *data = new char[reader->size()];
	reader->read(data, reader->size());

	VTXHeader *header = (VTXHeader*)data;
	mNumBodyParts = header->numBodyParts;
	mBodyParts = new BodyPart[mNumBodyParts];
	for(int i=0; i<mNumBodyParts; i++) {
		BodyPartHeader *bph = (BodyPartHeader*)((char*)header + header->bodyPartOffset + i * sizeof(BodyPartHeader));
		parseBodyPart(&mBodyParts[i], bph);
	}

	delete[] data;
}

VTX *VTX::open(IReaderFactory *factory, const std::string &name)
{
	VTX *ret = 0;
	IReader *reader = factory->open(name);

	if(reader) {
		ret = new VTX(reader);
		delete reader;
	}

	return ret;
}

}