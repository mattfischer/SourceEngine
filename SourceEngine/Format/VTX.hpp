#ifndef FORMAT_VTX_HPP
#define FORMAT_VTX_HPP

#include "File/File.hpp"
#include "File/Space.hpp"

#include <string>

namespace Format {

class VTX {
public:
	struct Strip {
		int numVertices;
		unsigned short *vertices;
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

	VTX(File::File *file);

	int numBodyParts() { return mNumBodyParts; }
	const BodyPart &bodyPart(int bodyPart) { return mBodyParts[bodyPart]; }

	static VTX *open(File::Space *space, const std::string &name);

private:
	int mNumBodyParts;
	BodyPart *mBodyParts;
};

}
#endif