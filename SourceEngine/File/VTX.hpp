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

	static VTX *open(IReaderFactory *factory, const std::string &name);

private:
	int mNumBodyParts;
	BodyPart *mBodyParts;
};

}
#endif