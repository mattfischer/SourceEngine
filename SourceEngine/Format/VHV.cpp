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
			mesh.vertices[j].r = vertices[j].b;
			mesh.vertices[j].g = vertices[j].g;
			mesh.vertices[j].b = vertices[j].r;
			mesh.vertices[j].a = vertices[j].a;

			if(mesh.vertices[j].r >= 0x40) {
				mesh.vertices[j].r = 0xff;
			} else {
				mesh.vertices[j].r *= 4;
			}

			if(mesh.vertices[j].g >= 0x40) {
				mesh.vertices[j].g = 0xff;
			} else {
				mesh.vertices[j].g *= 4;
			}

			if(mesh.vertices[j].b >= 0x40) {
				mesh.vertices[j].b = 0xff;
			} else {
				mesh.vertices[j].b *= 4;
			}
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