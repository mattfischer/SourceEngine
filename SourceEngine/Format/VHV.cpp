#include "Format/VHV.hpp"

namespace Format {

struct VHVHeader
{
	int version;
	unsigned int checksum;
	unsigned int vertexFlags;
	unsigned int vertexSize;
	unsigned int numVertices;
	int numMeshes;
	unsigned int unused[4];
};

struct VHVMeshHeader
{
	unsigned int lod;
	unsigned int numVertices;
	unsigned int vertexOffset;
	unsigned int unused[4];
};

VHV::VHV(File::File *file)
{
	unsigned char *data = new unsigned char[file->size()];
	file->read(data, file->size());

	VHVHeader *fileHeader = (VHVHeader*)data;

	mNumMeshes = fileHeader->numMeshes;
	mMeshes = new Mesh[mNumMeshes];
	for(unsigned int i=0; i<mNumMeshes; i++) {
		VHVMeshHeader *meshHeader = (VHVMeshHeader*)(data + sizeof(VHVHeader) + i * sizeof(VHVMeshHeader));
		Mesh &mesh = mMeshes[i];

		mesh.lod = meshHeader->lod;
		mesh.numVertices = meshHeader->numVertices;
		mesh.vertices = new RGBA[mesh.numVertices];

		RGBA *vertices = (RGBA*)(data + meshHeader->vertexOffset);
		for(unsigned int j=0; j<mesh.numVertices; j++) {
			mesh.vertices[j] = vertices[j];
		}
	}

	delete[] data;
}

VHV *VHV::open(File::Space *space, const std::string &filename)
{
	VHV *ret = 0;
	File::File *file = space->open(filename);
	if(file) {
		ret = new VHV(file);
		delete file;
	}

	return ret;
}

}