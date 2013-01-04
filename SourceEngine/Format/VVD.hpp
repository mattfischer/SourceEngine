#ifndef FORMAT_VVD_HPP
#define FORMAT_VVD_HPP

#include "File/File.hpp"
#include "File/Space.hpp"

#include <string>

namespace Format {

#define MAX_NUM_BONES_PER_VERT 3

class VVD {
public:
	struct Vector {
		float x;
		float y;
		float z;
	};

	struct Vector2D {
		float u;
		float v;
	};

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

	struct Lod
	{
		int numVertices;
		Vertex *vertices;
	};

	VVD(File::File *file);

	int numLods() { return mNumLods; }
	const Lod &lod(int lod) { return mLods[lod]; }

	static VVD *open(File::Space *space, const std::string &name);

private:
	int mNumLods;
	Lod *mLods;
};

}
#endif