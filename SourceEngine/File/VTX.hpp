#ifndef FILE_VTX_HPP
#define FILE_VTX_HPP

#include "File/IReader.hpp"
#include "File/IReaderFactory.hpp"

#include <string>

namespace File {

class VTX {
public:
	struct Strip {
		int numVertices;
		int *vertices;
		int flags;
	};

	struct StripGroup {
		int numStrips;
		Strip *strips;
	};

	struct Mesh {
		int numStripGroups;
		StripGroup *stripGroups;
	};

	struct Lod {
		int numMeshes;
		Mesh *meshes;
	};

	struct Model {
		int numLods;
		Lod *lods;
	};

	struct BodyPart {
		int numModels;
		Model *models;
	};

	VTX(IReader *reader);

	int numBodyParts() { return mNumBodyParts; }
	const BodyPart &bodyPart(int bodyPart) { return mBodyParts[bodyPart]; }

	static VTX *open(IReaderFactory *factory, const std::string &name);

private:
	int mNumBodyParts;
	BodyPart *mBodyParts;
};

}
#endif