#ifndef FORMAT_VHV_HPP
#define FORMAT_VHV_HPP

#include "File/Space.hpp"

#include <string>

namespace Format {

class VHV {
public:
	struct RGBA {
		char r;
		char g;
		char b;
		char a;
	};

	struct Mesh {
		int lod;
		size_t numVertices;
		RGBA *vertices;
	};

	VHV(File::File *file);

	size_t numMeshes() { return mNumMeshes; }
	const Mesh &mesh(int mesh) { return mMeshes[mesh]; }

	static VHV *open(File::Space *space, const std::string &filename);

private:
	size_t mNumMeshes;
	Mesh *mMeshes;
};

}

#endif