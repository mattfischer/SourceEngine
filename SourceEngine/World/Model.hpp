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

	Format::VVD::Vertex *vertices(int lod) { return mVertices[lod]; }

	const Geo::BoxOriented &box() { return mBox; }

	struct Lod {
		int numIndices;
		unsigned short *indices;
	};

	const Lod &lod(int lod) { return mLods[lod]; }

private:
	Format::VVD::Vertex **mVertices;

	int mNumMaterials;
	Material **mMaterials;

	int mNumLods;
	Lod *mLods;

	Geo::BoxOriented mBox;
};

}

#endif