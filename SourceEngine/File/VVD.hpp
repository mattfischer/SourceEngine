#ifndef FILE_VVD_HPP
#define FILE_VVD_HPP

#include "File/IReader.hpp"
#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

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

	VVD(IReader *reader);

	int numLods() { return mNumLods; }
	const Lod &lod(int lod) { return mLods[lod]; }

	static VVD *open(IReaderFactory *factory, const std::string &name);

private:
	int mNumLods;
	Lod *mLods;
};

}
#endif