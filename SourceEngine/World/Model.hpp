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
	Model(Format::MDL *mdl, Format::VVD *vvd, Format::VTX *vtx, File::Space *space, const std::string &modelPath);

	Format::MDL *mdl() { return mMdl; }
	Format::VVD *vvd() { return mVvd; }
	Format::VTX *vtx() { return mVtx; }

	int numMaterials() { return mNumMaterials; }
	Material *material(int material) { return mMaterials[material]; }

	const Geo::BoxOriented &box() { return mBox; }

private:
	Format::MDL *mMdl;
	Format::VVD *mVvd;
	Format::VTX *mVtx;

	int mNumMaterials;
	Material **mMaterials;

	Geo::BoxOriented mBox;
};

}

#endif