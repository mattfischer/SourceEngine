#ifndef FORMAT_VHV_HPP
#define FORMAT_VHV_HPP

#include "File/Space.hpp"

#include <string>

namespace Format {
namespace VHV {

struct RGBA {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct Mesh
{
	unsigned int lod;
	unsigned int numVertices;
	unsigned int vertexOffset;
	unsigned int unused[4];
};

struct Header
{
	int version;
	unsigned int checksum;
	unsigned int vertexFlags;
	unsigned int vertexSize;
	unsigned int numVertices;
	int numMeshes;
	unsigned int unused[4];

	Mesh *mesh(int n) { return (Mesh*)((char*)this + sizeof(Header)) + n; }
	RGBA *vertices(int m) { return (RGBA*)((char*)this + mesh(m)->vertexOffset); }
};

static Header *open(File::Space *space, const std::string &name)
{
	return (Header*)space->read(name);
}

}
}

#endif