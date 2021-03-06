#ifndef WORLD_MODEL_HPP
#define WORLD_MODEL_HPP

#include "File/Space.hpp"

#include "Format/MDL.hpp"
#include "Format/VTX.hpp"
#include "Format/VVD.hpp"

#include "Geo/BoxOriented.hpp"

#include "World/Material.hpp"

namespace World {

class Model {
public:
	Model(Format::MDL::Header *mdl, Format::VVD::Header *vvd, Format::VTX::Header *vtx, File::Space *space, const std::string &modelPath);

	int numMaterials() { return mNumMaterials; }
	Material *material(int material) { return mMaterials[material]; }

	Format::VVD::Vertex *vertices() { return mVertices; }

	const Geo::BoxOriented &box() { return mBox; }

	struct Strip {
		int numIndices;
		unsigned short *indices;
	};

	struct StripGroup {
		int numStrips;
		Strip *strips;
	};

	struct Mesh {
		int numStripGroups;
		StripGroup *stripGroups;
		int material;
	};

	struct Lod {
		float switchPoint;
		int numMeshes;
		Mesh *meshes;
	};

	struct _Model {
		int numLods;
		Lod *lods;
	};

	struct BodyPart {
		int numModels;
		_Model *models;
	};

	int numBodyParts() { return mNumBodyParts; }
	const BodyPart &bodyPart(int bodyPart) { return mBodyParts[bodyPart]; }

	int skin(int family, int material) { return mSkins[family][material]; }

private:
	Format::VVD::Vertex *mVertices;

	int mNumMaterials;
	Material **mMaterials;

	int mNumBodyParts;
	BodyPart *mBodyParts;

	Geo::BoxOriented mBox;

	int **mSkins;
};

}

#endif